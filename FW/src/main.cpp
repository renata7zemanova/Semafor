#include <Arduino.h>
#include <iostream>

#include "lib.h"

void setup() {
  int counter = 0;
  int time_counter = 0;

  _init_();

  States state = PLAY; 
  download_permanently_pref();
  delay(100);

  ArduinoMetronome settings_delay(300000); //5 minut
  ArduinoMetronome LED_delay(800);

  while(true){

    if(state == PLAY){
      switch(s_vect.game){
        case VABNICKA:
          play_vabnicka(); 
          break;
        case SEMAFOR:
          play_semafor();
          break;
        case ODPOCITAVADLO:
          play_odpocitavadlo();
          break;
      }
    }

    if((!settings_delay.loopMs()) && (time_counter == 0)){
      if((is_touched_up() && is_touched_down()) || (state == CONFIGURATION)){   
        if(counter == 0){
          state = CONFIGURATION;
          start_server(); 
          delay(200);
          counter = 1;
          vibrate_motor_off();
        }
        server.handleClient(); 
        share_settings();
        
        if(LED_delay.loopMs())
          LEDs_all_toggle(BLUE);
      }
    }

    else if(time_counter == 0){ 
      time_counter = 1;
      wifi_disable();
      LEDs_all_off();
      state = PLAY;
    }
    
    if(receive_settings()){
      LEDs_all_on(BLUE); //potvrzeni prijeti
      delay(500);
      LEDs_all_off();
      delay(500);
    }

    tick_for_buttons();
    set_LED_brightness();
    delay(10); 
  }
}

void loop() {}