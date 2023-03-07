#include <Arduino.h>
#include <SmartLeds.h>
#include <Color.h>
#include <AtTouch.h>
//#include <cstdlib>
//#include <map>

#include "lib.h"

void setup() {
  AtTouch CapBtn; 


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
  //nachystat prechod do modu configuration
  if(is_configuration_on()){
    state = CONFIGURATION;
  }
  if(is_configuration_end()){
    state = PLAY;
  }
}

void loop() {}
