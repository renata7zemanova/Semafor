#include <Arduino.h>
#include <SmartLeds.h>
#include <Color.h>
#include <cstdlib>
#include <map>

#include "lib.h"

void setup() {
  _init_();

  led_t playing (LED_COUNT_GAME, LED_PIN_GAME, 2);
  led_t assignment (LED_COUNT_TASK, LED_PIN_TASK, 0);
  led_t evaluated (LED_COUNT_EVAL, LED_PIN_EVAL, 1);

  Colors playing_array[LINE_LENGTH * 10];
  Colors evaluated_array[LINE_LENGTH * 10];
  Colors task_array[LINE_LENGTH];

  clear_all_leds(playing, LED_COUNT_GAME, assignment, LED_COUNT_TASK, evaluated, LED_COUNT_EVAL);
  show_leds(playing, assignment, evaluated);
  wait_leds(playing, assignment, evaluated);
  set_led_pos_null(playing, assignment, evaluated);

  Colors task[LINE_LENGTH];
  int tog = 0;
  int num_of_black = 0;
  int num_of_white = 0;
  bool end = false;
  int count_enter = 0;

  int GAME_LINE_LENGTH;
  int DIFF;
  bool GAME_FOR_2_PLAYERS;

  wait_for_btn_press(SW_NEW_GAME);

  while(true){  
    if(is_pressed(SW_NEW_GAME)){
      wait_for_btn_release(SW_NEW_GAME);
      DIFF = 3; 
      GAME_LINE_LENGTH = 4;
      GAME_FOR_2_PLAYERS = is_game_for_2_players();
      
      if(is_game_with_space())
        DIFF = 2;
      if(is_game_on_3_colors())
        GAME_LINE_LENGTH = 3;
      clear_all_leds(playing, LED_COUNT_GAME, assignment, LED_COUNT_TASK, evaluated, LED_COUNT_EVAL);
      wait_leds(playing, evaluated, assignment);
      show_leds(playing, evaluated, assignment);
      wait_leds(playing, evaluated, assignment);
      if(!GAME_FOR_2_PLAYERS){
        generate_task(task, GAME_LINE_LENGTH, DIFF);
        set_task(assignment, task, GAME_LINE_LENGTH);
      }
      if(GAME_FOR_2_PLAYERS){
        clear_array(task_array, LINE_LENGTH);
        clear_array(evaluated_array, LINE_LENGTH * 10);
        set_led_pos_null(evaluated, assignment);
        count_enter = 0;
      }
      clear_array(playing_array, LINE_LENGTH * 10);
      playing.pos = 0;
      end = false; 
      set_power_leds(playing.pos, POWER_OFF);
      set_power_leds(playing.pos, POWER_ON);
    }

    if(is_pressed(SW_YELLOW)){
      wait_for_btn_release(SW_YELLOW);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        set_color_and_go_right(YELLOW, task_array, assignment, GAME_LINE_LENGTH);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        set_color_and_go_right(YELLOW, playing_array, playing, GAME_LINE_LENGTH);
      else if(GAME_FOR_2_PLAYERS)
        set_color_and_go_right(YELLOW, evaluated_array, evaluated, GAME_LINE_LENGTH); 
    }

    else if(is_pressed(SW_ORANGE)){
      wait_for_btn_release(SW_ORANGE);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        set_color_and_go_right(ORANGE, task_array, assignment, GAME_LINE_LENGTH);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        set_color_and_go_right(ORANGE, playing_array, playing, GAME_LINE_LENGTH);
    }

    else if(is_pressed(SW_RED)){
      wait_for_btn_release(SW_RED);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        set_color_and_go_right(RED, task_array, assignment, GAME_LINE_LENGTH);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        set_color_and_go_right(RED, playing_array, playing, GAME_LINE_LENGTH);
      else if(GAME_FOR_2_PLAYERS)
        set_color_and_go_right(RED, evaluated_array, evaluated, GAME_LINE_LENGTH);
    }

    else if(is_pressed(SW_PURPLE)){
      wait_for_btn_release(SW_PURPLE);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        set_color_and_go_right(PURPLE, task_array, assignment, GAME_LINE_LENGTH);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS){
        set_color_and_go_right(PURPLE, playing_array, playing, GAME_LINE_LENGTH);
      }
    }

    else if(is_pressed(SW_BLUE)){
      wait_for_btn_release(SW_BLUE);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        set_color_and_go_right(BLUE, task_array, assignment, GAME_LINE_LENGTH);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        set_color_and_go_right(BLUE, playing_array, playing, GAME_LINE_LENGTH);
    }

    else if(is_pressed(SW_GREEN)){
      wait_for_btn_release(SW_GREEN);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        set_color_and_go_right(GREEN, task_array, assignment, GAME_LINE_LENGTH);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        set_color_and_go_right(GREEN, playing_array, playing, GAME_LINE_LENGTH);
    }

    else if(is_pressed(SW_LEFT)){
      wait_for_btn_release(SW_LEFT);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        save_col_and_arrow(LEFT, task_array, assignment, GAME_LINE_LENGTH);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        save_col_and_arrow(LEFT, playing_array, playing, GAME_LINE_LENGTH);
      else if(GAME_FOR_2_PLAYERS)
        save_col_and_arrow(LEFT, evaluated_array, evaluated, GAME_LINE_LENGTH);
    } 

    else if(is_pressed(SW_RIGHT)){
      wait_for_btn_release(SW_RIGHT);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        save_col_and_arrow(RIGHT, task_array, assignment, GAME_LINE_LENGTH);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        save_col_and_arrow(RIGHT, playing_array, playing, GAME_LINE_LENGTH);
      else if(GAME_FOR_2_PLAYERS)
        save_col_and_arrow(RIGHT, evaluated_array, evaluated, GAME_LINE_LENGTH);
    } 

    else if(is_pressed(SW_ENTER)){
      wait_for_btn_release(SW_ENTER);
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        set_last_color(task_array, assignment);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        set_last_color(playing_array, playing);
      else if(GAME_FOR_2_PLAYERS)
        set_last_color(evaluated_array, evaluated);

      if(!GAME_FOR_2_PLAYERS){  
        evaluate(playing.pos, playing_array, task, &num_of_black, &num_of_white, GAME_LINE_LENGTH);
        set_evaluated_black(num_of_black, evaluated, GAME_LINE_LENGTH);

        if(num_of_black == GAME_LINE_LENGTH){
          wait_leds(playing, assignment, evaluated);
          show_leds(playing, assignment, evaluated);

          wait_for_btns_press(SW_NEW_GAME, SW_END);
          continue;
        }
        set_evaluated_white(num_of_black, num_of_white, evaluated);

        if(is_end(playing.pos)){
          wait_leds(assignment, evaluated);
          show_leds(assignment, evaluated);

          wait_for_btns_press(SW_NEW_GAME, SW_END);
          continue;
        }
      }

      if(GAME_FOR_2_PLAYERS)
        ++count_enter;
        
      if((((count_enter != 1) && is_odd(count_enter)) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS){
        new_line(playing);
        new_line(evaluated);
      }
      set_power_leds(playing.pos, POWER_ON);
    }

    else if(is_pressed(SW_END)){
      wait_for_btn_release(SW_END);
      
      clear_all_leds(playing, LED_COUNT_GAME, assignment, LED_COUNT_TASK, evaluated, LED_COUNT_EVAL);
      wait_leds(playing, assignment, evaluated);
      show_leds(playing, assignment, evaluated);
      end = true;
      count_enter = 0;
      set_power_leds(playing.pos, POWER_OFF);
      wait_for_btn_press(SW_NEW_GAME);
    }  

     ++tog;
    if(tog == 3)
      tog = 0;

    if(!end && (tog == 0 || tog == 2)){ 
      if(count_enter == 0 && GAME_FOR_2_PLAYERS)
        toggle_cursor(assignment, task_array[assignment.pos], tog);
      else if((is_odd(count_enter) && GAME_FOR_2_PLAYERS) || !GAME_FOR_2_PLAYERS)
        toggle_cursor(playing, playing_array[playing.pos], tog);
      else if(GAME_FOR_2_PLAYERS)
        toggle_cursor(evaluated, evaluated_array[evaluated.pos], tog);
    }

    wait_leds(playing, assignment, evaluated);
    show_leds(playing, evaluated);
    if(GAME_FOR_2_PLAYERS)
      assignment.leds.show();

    delay(200);
  } 
  
}

void loop() {}
