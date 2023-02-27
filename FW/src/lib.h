//#include <Arduino.h>
#include <SmartLeds.h>
#include <Color.h>
//#include <cstdlib>
//#include <map>

#define SET_POWER_LEDS_TOP 13 
#define LED_PIN_TOP 10

//#define SET_POWER_LEDS_SIDE 4
//#define LED_PIN_SIDE 3

#define NUM_OF_COLORS 11

#define POWER_OFF 0
#define POWER_ON 1

enum Colors {RED = 0, BLUE, GREEN, YELLOW, BROWN, PURPLE, PINK, ORANGE, AZURO, BLACK, WHITE};

//enum Direct {RIGHT, LEFT};

struct led_t {
  led_t(const int COUNT, const int PIN, const int CHANNEL) : leds(LED_WS2812, COUNT, PIN, CHANNEL) {}

  SmartLed leds;
  int pos;
};

//tuto strukturu asi nebudu potrebovat
struct leds_t{
    led_t top;
    led_t task;
    led_t eval;
    Colors play_array;
    Colors task_array;
    Colors eval_array;
};

void _init_ (){
  //pinMode(LED_PIN_TOP, OUTPUT);

  //pinMode(SET_POWER_LEDS_TOP, OUTPUT);


  //digitalWrite(SET_POWER_LEDS_TOP, POWER_OFF); 
}

void LED_leght(const int TYPE, const int COLOR, const int FIRST_LED, const int LAST_LED){

}

//zjistit, jak udelat v C++ delay
void LED_toggle(const int TYPE, const int COLOR, const int FIRST_LED, const int LAST_LED, const int NUM_OF_REPEAT = 1){
  for(int i = 0; i < NUM_OF_REPEAT; ++i){
    LED_leght(TYPE, COLOR, FIRST_LED, LAST_LED);
    //delay(200);
    LED_leght(TYPE, BLACK, FIRST_LED, LAST_LED);
    //delay(200);
  }
}
  
void LED_off(const int TYPE, const int COLOR, const int FIRST_LED, const int LAST_LED){
  LED_leght(TYPE, BLACK, FIRST_LED, LAST_LED);
}

Rgb colors(Colors COLOR){
  switch(COLOR){
    case RED:
      return Rgb{255, 0, 0};
    case BLUE:
      return Rgb{0, 0, 255};
    case GREEN:
      return Rgb{0, 255, 0};
    case YELLOW:
      return Rgb{255, 255, 0};
    case BROWN:
      return Rgb{102, 51, 0};
    case PURPLE:
      return Rgb{255, 0, 255};
    case PINK:
      return Rgb{255, 102, 178};
    case ORANGE:
      return Rgb{255, 128, 0};
    case AZURO:
      return Rgb{0, 255, 255};
    case BLACK:
      return Rgb{0, 0, 0};
    case  WHITE:
      return Rgb{255, 255, 255};
  }
}
