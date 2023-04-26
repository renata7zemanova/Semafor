#include <Arduino.h>
#include <iostream>

#include "lib.h"

void setup() {
  Serial.begin(115200);

  _init_();

  States state = CONFIGURATION_DOWNLOAD; //na zacatku musi byt PLAY
  download_permanently_pref();

  ArduinoMetronome settings_delay(20000); //pak budou 2 minuty

  while(true){
    
    if(!settings_delay.loopMs()){
      if(state == CONFIGURATION_DOWNLOAD){
        if(receive_settings()){
          //potvrzeni LEDkama
          LEDs_all_on(GREEN);
          delay(200);
          LEDs_all_off();
          
          state = PLAY;
          continue;
        }

        Serial.println("cekam na stisk");
        if(is_touched_up() && is_touched_down()){
          Serial.println("stisk tlacitek");
          state = CONFIGURATION_SHARE;
          //to co se ma provest pouze jedou - zapnuti sdileni
          start_server(); //tuhle cast kodu bude vykonavat pouze jeden Semafor - pouze ten, ke kteremu se pripojim 
          delay(200);
          vibrate_motor_off();
          continue;
        }
      }

      else if(state == CONFIGURATION_SHARE){
        if(share_delay.loopMs()){
          Serial.println("share mode");
          server.handleClient(); //dokud neskonci konfigurace a nevypnu WiFi, tak to musim periodicky volat
          share_settings();
        }
        if(LED_delay.loopMs()){
          LEDs_all_toggle(BLUE);
        }
      }
    }
  
  //az zmacknu tlacitko, tak zacne hra 
  //kazdy case bude cekat na stisk tlacitka enter (asi)
  //vypnout wifi
    else
      state = PLAY;
      
    if(state == PLAY){
      //pak bude stav hra, CONFIGURATION_DOWNLOAD nebude existovat
      //konfigurace bude mozna po stisku danych tlacitek 
      //pouze po nejakou dobu 
      //pak vypnout wifi
      Serial.println("jsem ve hre");
      switch(s_vect.game){
        case VABNICKA1:
          play_vabnicka1(); //nedelat blokujici 
          break;
        case VABNICKA2:
          play_vabnicka2();
          break;
        case PAN_HORY:
          play_pan_hory(); //static veci, abych pokracovala ve hre
          break;
        case SEMAFOR:
          play_semafor();
          break;
        case ODPOCITAVADLO:
          play_odpocitavadlo();
          break;
      }
    }
    tick_for_buttons();
    delay(10);
  }
}

void loop() {}
