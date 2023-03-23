#include <Arduino.h>

#include <iostream>

#include "lib.h"

//neni HW
#define PIN_AUX_LORA 5


void setup() {
  //neni HW 
  pinMode(PIN_AUX_LORA, INPUT_PULLDOWN);

  _init_();

 States state = PLAY;
 Games game = VABNICKA;
 
  while(true){
    if(state = CONFIGURATION){
      if(is_configuration_on()){
        state = CONFIGURATION;
      }
      if(is_configuration_end()){
        state = PLAY;
      }
    }
  
    if(state = PLAY){
      switch(game){
        case VABNICKA:
          play_vabnicka();
          break;
        case PAN_HORY:
          play_pan_hory();
          break;
        case SEMAFOR:
          play_semafor();
          break;
        case ODPOCITAVADLO:
          play_odpocitavadlo();
          break;
      }
    }
  }
}

void loop() {}
