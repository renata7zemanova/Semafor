#include <Arduino.h>
#include <SmartLeds.h>
#include <Color.h>
#include <AtTouch.h>

//#include <cstdlib>
//#include <map>

#include "lib.h"


#define PIN_AUX_LORA 5


void setup() {
  AtTouch CapBtn; 
  pinMode(PIN_AUX_LORA, INPUT_PULLDOWN);


  _init_();

 States state = PLAY;
  while(true){
    switch(state){
      case PLAY:
        break;
      case CONFIGURATION:
        break;
    }
  }
  if(state = PLAY){
    LoRa_on();
  }

  //nachystat prechod do modu configuration
  if(is_configuration_on()){
    state = CONFIGURATION;
  }
  if(is_configuration_end()){
    state = PLAY;
  }
}

void loop() {}
