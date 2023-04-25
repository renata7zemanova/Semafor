#include <Arduino.h>
#include <iostream>
#include <Preferences.h>

#include "lib.h"

//neni HW
//#define PIN_AUX_LORA 5


void setup() {


  Serial.begin(115200);
  //neni HW 
  //pinMode(PIN_AUX_LORA, INPUT_PULLDOWN);

  _init_();

  States state = CONFIGURATION_DOWNLOAD; //na zacatku musi byt CONFIGURATION_DOWNLOAD
  s_vect.game = VABNICKA2; //pak nebude definovano tady
  //pokud jsem ho jeste nenaprogramovala, tak ulozit defaultni hodnoty (treba prvni hru s nejakymi parametry)
  //vyrobit state vector default
  //getBytes("identificator", state default) //kdyz neexistuje 
  //vycist RAMku

  /*while(true){
    tick_for_buttons();
    for(int i = 0; i < NUM_OF_BUTTONS; ++i){
      //Serial.print(Touch_AT42.read_button_raw_value(i));
      Serial.print(Touch_AT42.is_touched_btn(i));
      Serial.print(" ");
    }
    Serial.println();
    delay(10);
  }*/

  while(true){
    

    if(state == CONFIGURATION_DOWNLOAD){
      if(receive_settings()){
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

      //tady si musim nacist vsechny promenne dane hry
      //tady musim nastavit hru a ulozit ji do promenne game
      //tyhle informace musim pres WiFi (klasickou, ne AP) odeslat ostatnim Semaforum 
      //wifi_disable();
      
      //state = PLAY;
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
      //dalsi odpocet pro signalizaci na ledkach / bude jiny cas
  
      //pokud neco, tak zmenit stav na PLAY
    }
  
  //az zmacknu tlacitko, tak zacne hra 
  //kazdy case bude cekat na stisk tlacitka enter (asi)
  //vypnout wifi
    else if(state == PLAY){
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
