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

  States state = PLAY; //na zacatku musi byt CONFIGURATION
  //pokud jsem ho jeste nenaprogramovala, tak ulozit defaultni hodnoty (treba prvni hru s nejakymi parametry)
  //vyrobit state vector default
  //getBytes("dentificator", state default) //kdyz neexistuje 
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
    if(state = CONFIGURATION){
      wifi_enable_connect();
      start_server(); //tuhle cast kodu bude vykonavat pouze jeden Semafor - pouze ten, ke kteremu se pripojim 
      while(true){ //dat nejakou casovou konstantu (konfigurace bude mozna pouze po nejaky cas)
        server.handleClient(); //dokud neskonci konfigurace a nevypnu WiFi, tak to musim periodicky volat
        delay(1);
      }
      //tady si musim nacist vsechny promenne dane hry
      //tady musim nastavit hru a ulozit ji do promenne game
      //tyhle informace musim pres WiFi (klasickou, ne AP) odeslat ostatnim Semaforum 
      wifi_disable();
      state = PLAY;
    }
  
  //az zmacknu tlacitko, tak zacne hra 
  //kazdy case bude cekat na stisk tlacitka enter (asi)
    if(state = PLAY){
      switch(s_vect.game){
        case VABNICKA:
          play_vabnicka(); //nedelat blokujici 
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
  }
}

void loop() {}
