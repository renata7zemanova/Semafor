#include <Arduino.h>
#include <iostream>

#include "lib.h"

void setup() {
  Serial.begin(115200);
  int counter = 0;
  int time_counter = 0;

  _init_();

  States state = PLAY; 
  download_permanently_pref();
  ArduinoMetronome settings_delay(300000); //tohle je 5 minut

  //tohle vsechno bude nastaveno z webu
  s_vect.game = ODPOCITAVADLO;
  s_vect.odpocitavadlo_timeout = 2; //cas v minutach

  while(true){
    //if(is_touched_enter())
      //Serial.println("stisk enteru");
    //if(is_touched_up())
      //Serial.println("stisk up");
    if(state == PLAY){
      //Serial.print("herni mod ");
      //Serial.println(s_vect.game);
      switch(s_vect.game){
        case VABNICKA:
          Serial.println("vabnicka");
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

    if(share_delay.loopMs()){
      Serial.println("cas vyprsel");
    }

    if(((is_touched_up() && is_touched_down()) || (state == CONFIGURATION)) && (time_counter == 0)){
      Serial.println("configuration mode");
      if(!share_delay.loopMs()){
        if(counter == 0){
          Serial.println("stisk tlacitek, zapnut server");
          state = CONFIGURATION;
          //to co se ma provest pouze jedou - zapnuti sdileni
          start_server(); //tuhle cast kodu bude vykonavat pouze jeden Semafor - pouze ten, ke kteremu se pripojim 
          delay(200);
          ++counter;
          vibrate_motor_off();
        }
        server.handleClient(); //dokud neskonci konfigurace a nevypnu WiFi, tak to musim periodicky volat
        share_settings();
      }
      else{
        time_counter = 1;
        wifi_disable();
        state = PLAY;
      }
        

      if(LED_delay.loopMs()){
        LEDs_all_toggle(BLUE);
      }
    }
    
    if(!share_delay.loopMs()){
      if(receive_settings()){
        LEDs_all_on(BLUE); //potvrzeni LEDkama
        delay(500);
        LEDs_all_off();
        wifi_disable();
        state = PLAY;
      }
    }

    tick_for_buttons();
    delay(10); 
  }
}

void loop() {}
