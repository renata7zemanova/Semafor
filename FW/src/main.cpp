#include <Arduino.h>

#include <iostream>
//#include <USBCDC.h>

#include "lib.h"

//neni HW
//#define PIN_AUX_LORA 5


void setup() {
  Serial.begin(115200);
  //neni HW 
  //pinMode(PIN_AUX_LORA, INPUT_PULLDOWN);

  _init_(Serial);

 States state = PLAY;
 Games game = ODPOCITAVADLO;
 
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
          play_odpocitavadlo(Serial);
          break;
      }
    }
  }
}

void loop() {}
