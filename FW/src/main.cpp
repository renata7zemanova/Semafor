#include <Arduino.h>

#include <iostream>

#include "lib.h"


#define PIN_AUX_LORA 5


void setup() {
  AtTouch CapBtn; 
  pinMode(PIN_AUX_LORA, INPUT_PULLDOWN);


  _init_();

 States state = PLAY;
 Games game = VABNICKA;
 
  while(true){
    switch(state){
      case PLAY:
        break;
      case CONFIGURATION:
        break;
    }
  }
  if(state = PLAY){
    switch(game){
      case VABNICKA:
        break;
      case PAN_HORY:
        break;

    }
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
