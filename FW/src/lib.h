#include <Arduino.h>
#include <SmartLeds.h>
#include <Color.h>
#include <cstdlib>
#include <map>

#define SET_POWER_LEDS_1_TO_4 13 
#define SET_POWER_LEDS_5_TO_7 4
#define SET_POWER_LEDS_8_TO_10 18

#define LED_COUNT_GAME 40
#define LED_PIN_GAME 23

#define LED_COUNT_TASK 4
#define LED_PIN_TASK 21

#define LED_COUNT_EVAL 40
#define LED_PIN_EVAL 22

#define SW_YELLOW 14 
#define SW_BLUE  34
#define SW_GREEN 19
#define SW_RED 10
#define SW_PURPLE 35
#define SW_ORANGE 9

#define SW_ENTER 25
#define SW_RIGHT 33 
#define SW_LEFT 32 
#define SW_END 27 
#define SW_NEW_GAME 26

#define SW_13 5 
#define SW_14 15 
#define SW_15 2 

#define POWER_ON 1
#define POWER_OFF 0

#define LINE_LENGTH 4
#define NUM_OF_COLORS 9

enum Colors {YELLOW = 0, BLUE, GREEN, RED, PURPLE, ORANGE, BLACK, WHITE, BROWN};

enum Direct {RIGHT, LEFT};

struct led_t {
  led_t(const int COUNT, const int PIN, const int CHANNEL) : leds(LED_WS2812, COUNT, PIN, CHANNEL) {}

  SmartLed leds;
  int pos;
};

struct leds_t{
    led_t play;
    led_t task;
    led_t eval;
    Colors play_array;
    Colors task_array;
    Colors eval_array;
};

void _init_ (){
  pinMode(LED_PIN_GAME, OUTPUT);
  pinMode(LED_PIN_TASK, OUTPUT);
  pinMode(LED_PIN_EVAL, OUTPUT);

  pinMode(SET_POWER_LEDS_1_TO_4, OUTPUT);
  pinMode(SET_POWER_LEDS_5_TO_7, OUTPUT);
  pinMode(SET_POWER_LEDS_8_TO_10, OUTPUT);

  pinMode(SW_ENTER, INPUT_PULLUP);
  pinMode(SW_RIGHT, INPUT_PULLUP);
  pinMode(SW_LEFT, INPUT_PULLUP);
  pinMode(SW_END, INPUT_PULLUP);
  pinMode(SW_NEW_GAME, INPUT_PULLUP);
  pinMode(SW_YELLOW, INPUT_PULLUP);
  pinMode(SW_ORANGE, INPUT_PULLUP);
  pinMode(SW_RED, INPUT_PULLUP);
  pinMode(SW_PURPLE, INPUT_PULLUP);
  pinMode(SW_BLUE, INPUT_PULLUP);
  pinMode(SW_GREEN, INPUT_PULLUP);

  digitalWrite(SET_POWER_LEDS_1_TO_4, POWER_OFF); 
  digitalWrite(SET_POWER_LEDS_5_TO_7, POWER_OFF);
  digitalWrite(SET_POWER_LEDS_8_TO_10, POWER_OFF);
}

void set_color (led_t &LED, Colors COLOR){
  const int num_max = 32;

  switch (COLOR){
    case YELLOW:
      LED.leds[LED.pos] = Rgb{num_max, num_max, 0}; //kod zlute
      break;
    case RED:
      LED.leds[LED.pos] = Rgb{num_max, 0, 0}; //kod cervene
      break;
    case BLUE:
      LED.leds[LED.pos] = Rgb{0, 0, num_max}; //kod modre
      break;
    case GREEN:
      LED.leds[LED.pos] = Rgb{0, num_max, 0}; //kod zelene
      break;
    case PURPLE:
      LED.leds[LED.pos] = Rgb{24, 0, num_max}; //kod fialove
      break;
    case ORANGE:
      LED.leds[LED.pos] = Rgb{num_max, 16, 0}; //kod oranzove
      break;
    case BLACK:
      LED.leds[LED.pos] = Rgb{0, 0, 0}; //kod cerne
      break;
    case WHITE:
      LED.leds[LED.pos] = Rgb{num_max, num_max, num_max}; //kod bile
      break;
    case BROWN:
      LED.leds[LED.pos] = Rgb{12, 9, 0}; //kod hnede
      break;
  } 
}

void toggle_cursor (led_t &LED, Colors color_for_blick, int tog){
  if(tog == 2)
    set_color(LED, color_for_blick);
  else
    set_color(LED, BLACK);
}

void clear(led_t &LED, const int COUNT){
  for (int i = 0; i < COUNT; i++){
    LED.pos = i;
    set_color(LED, BLACK);
  }
  LED.pos = 0;
}

void clear_array(Colors* array, int length){
	for(int i = 0; i < length; ++i){
		array[i] = BROWN;
	}
}

void shift_cursor (led_t &LED, Direct DIR, int length){
  if((DIR == RIGHT) && !((LED.pos + 1 + LINE_LENGTH - length) % LINE_LENGTH))
    LED.pos -= LINE_LENGTH - (LINE_LENGTH - length) - 1;
  else if(DIR == RIGHT)
    LED.pos += 1;
  else if((DIR == LEFT) && (LED.pos == 0 || !(LED.pos % LINE_LENGTH)))
    LED.pos += LINE_LENGTH - (LINE_LENGTH - length) - 1;
  else if(DIR == LEFT)
    LED.pos -= 1;
}

void new_line (led_t &LED){
  int line = LED.pos / LINE_LENGTH;
  LED.pos = LINE_LENGTH * (line + 1);
}

void generate_task (Colors *task, int length, int diff){
  for(int i = 0; i < length; ++i)
    task[i] = Colors(esp_random() % (NUM_OF_COLORS - diff));
}

void set_task(led_t &leds, Colors* array_task, int length){
	for (int i = 0; i < length; ++i){
		leds.pos = i;
		set_color(leds, array_task[i]); 
	} 
}

void convert_brown_to_black(Colors *playing, int led_pos_nul, int length){
  for(int i = 0; i < length; ++i){
    if(playing[i + led_pos_nul] == BROWN)
      playing[i + led_pos_nul] = BLACK;
  }
}

void evaluate (int led_pos, Colors *playing, Colors *task, int *black, int *white, int length){ 
  int led_pos_nul = led_pos - (led_pos % LINE_LENGTH);

  int pos[length] = {0};
  int pos_play[length] = {0};
  int k = 0;
  bool similar = false;
  int l = 0;
  bool end = false;

  *black = 0;
  *white = 0;

  convert_brown_to_black(playing, led_pos_nul, length);

  for(int i = 0; i < length; ++i){
    if(task[i] == playing[i + led_pos_nul]){
      ++*black;
      pos[k] = i;
      ++k;
    }
    if(*black == length){
      end = true;
    }
  }

  if(!end){
    for(int i = 0; i < length; ++i){
      similar = false;
      if(k != 0){
        for(int a = 0; a < k; ++a){
          if(i == pos[a]){
            similar = true;
            break;
          }
        }
      }
      if(similar){
        continue;
      }
      for(int j = 0; j < length; ++j){
        similar = false;
        if(k != 0){
          for(int a = 0; a < k; ++a){
            if(j == pos[a]){
              similar = true;
              break;
            }
          }
        }
        if(similar){
          continue;
        }
        if(task[i] == playing[j + led_pos_nul]){
          similar = false;
          if(l != 0){
            for(int a = 0; a < l; ++a){
              if(j == pos_play[a]){
                similar = true;
                break;
              }
            }
          }
          if(similar){
            continue;
          }
          pos_play[l] = j;
          ++l;
          ++*white;
          break;
        }
      }
    }
  }
}

void set_color_and_go_right(Colors color,Colors* array,led_t &leds, int length){
	if(array[leds.pos] == color){
		array[leds.pos] = BROWN;
		color = BLACK;
	}
	else{
		array[leds.pos] = color;
	}
	set_color(leds, color);
	shift_cursor(leds, RIGHT, length);
}

void set_last_color(Colors* array, led_t &leds){
  if(array[leds.pos] != BROWN)
		set_color(leds, array[leds.pos]);
	else
		set_color(leds, BLACK);
}

void save_col_and_arrow(Direct dir, Colors* array, led_t &leds, int length){
	set_last_color(array, leds);
	shift_cursor(leds, dir, length);
}

void wait_for_btn_release(int btn){
  while(!digitalRead(btn))
        delay(1);
}

void wait_for_btn_press(int btn){
  while(digitalRead(btn))
        delay(1);
}

void wait_for_btns_press(int btn1, int btn2){
  while (digitalRead(btn1) && digitalRead(btn2))
    delay(1);
}

bool is_pressed(int btn){
  if(!digitalRead(btn))
    return true;
  return false;
}

bool is_odd(int num){
  if(num % 2)
    return true;
  return false;
}

bool is_even(int num){
  if(!(num & 2))
    return true;
  return false;
}

void set_power_leds(int pos, int state){
  if(state == POWER_ON){
    if(pos == 0)
      digitalWrite(SET_POWER_LEDS_1_TO_4, state);
    if(pos == (LINE_LENGTH * 4))
      digitalWrite(SET_POWER_LEDS_5_TO_7, state);
    if(pos == (LINE_LENGTH * 7))
      digitalWrite(SET_POWER_LEDS_8_TO_10, state);
  }
  else{
    digitalWrite(SET_POWER_LEDS_1_TO_4, state);
    digitalWrite(SET_POWER_LEDS_5_TO_7, state);
    digitalWrite(SET_POWER_LEDS_8_TO_10, state);
  }
}

bool is_end(int pos){
	if(pos > (10 * LINE_LENGTH) - LINE_LENGTH - 1)
		return true;
	return false; 
}

void set_evaluated_black(int num_of_black, led_t& leds, int length){
	for(int i = 0; i < num_of_black; ++i){		
		set_color(leds, RED);
		if(i != (length - 1))
			++leds.pos;	
	}
}

void set_evaluated_white(int num_of_black, int num_of_white, led_t& leds){
	for(int i = 0; i < num_of_white; ++i){		
		set_color(leds, YELLOW);
		if(i + num_of_black != (LINE_LENGTH - 1))
			++leds.pos;	
	}
}

void show_leds(led_t& leds1, led_t& leds2){
    leds1.leds.show();
    leds2.leds.show();
}

void show_leds(led_t& leds1, led_t& leds2, led_t& leds3){
    leds1.leds.show();
    leds2.leds.show();
    leds3.leds.show();
}

void wait_leds(led_t& leds1, led_t& leds2){
    leds1.leds.wait();
    leds2.leds.wait();
}

void wait_leds(led_t& leds1, led_t& leds2, led_t& leds3){
    leds1.leds.wait();
    leds2.leds.wait();
    leds3.leds.wait();
}

void clear_all_leds(led_t& leds1, int cnt1, led_t& leds2, int cnt2, led_t& leds3, int cnt3){
    clear(leds1, cnt1);
    clear(leds2, cnt2);
    clear(leds3, cnt3);
}

void set_led_pos_null(led_t& led1, led_t& led2){
    led1.pos = 0;
    led2.pos = 0;
}

void set_led_pos_null(led_t& led1, led_t& led2, led_t& led3){
    led1.pos = 0;
    led2.pos = 0;
    led3.pos = 0;
}

bool is_game_with_space(){
  return digitalRead(SW_14);
}

bool is_game_for_2_players(){
  return digitalRead(SW_15);
}

bool is_game_on_3_colors(){
  return digitalRead(SW_13);
}
