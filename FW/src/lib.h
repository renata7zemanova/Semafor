#include <Arduino.h>
#include <SmartLeds.h>
#include <Color.h>
#include <cstdlib>
#include <map>

#define SET_POWER_LEDS_TOP 13 
#define LED_PIN_TOP 10

//#define SET_POWER_LEDS_SIDE 4
//#define LED_PIN_SIDE 3

#define NUM_OF_COLORS 10

#define POWER_OFF 0
#define POWER_ON 1

enum Colors {RED = 0, BLUE, GREEN, YELLOW, BROWN, PURPLE, PINK, ORANGE, BLACK, WHITE};

//enum Direct {RIGHT, LEFT};

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
  pinMode(LED_PIN_TOP, OUTPUT);

  pinMode(SET_POWER_LEDS_TOP, OUTPUT);


  digitalWrite(SET_POWER_LEDS_TOP, POWER_OFF); 
}

