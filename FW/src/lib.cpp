#include <lib.h>

#include <Arduino.h>
#include <AtTouch.h> //pridat pres GitHub odkaz do platformio 
#include <Adafruit_NeoPixel.h>
#include <LoRa_E22.h>

#include <iostream>



LoRa_E22 LoRa(RX, TX, &Serial1, UART_BPS_RATE_9600);
led_t LED(NUM_OF_LEDS, LED_PIN_TOP);
AtTouch CapBtn;


void play_vabnicka(){

}

void play_pan_hory(){

}

void play_semafor(){

}

void play_odpocitavadlo(HardwareSerial &Serial){
    int timeout = 10; //cas v minutach - pak se bude nacitat z konfiguracniho webu
    double time_for_1_LED = timeout / NUM_OF_LEDS;
    double actual_time;
    Colors color = BLACK; //jak nastavovat barvu?
    //LEDs_all_on(LED, color);
    LEDs_all_on(color);

    //pro testovani tlacitek
    Serial.println("pred ctenim kap. tlacitek");
    while(true){
        read_cap_but(CapBtn, Serial);
        
        delay(1000);
    }
    //upravit vypocet actual time a pozici menit pouze za podminky, ze je to cele cislo 
    //LED.pos = NUM_OF_LEDS - (actual_time / time_for_1_LED); //blbost
    //LED_light(LED, BLACK);

}

void set_brightness(led_t &LED){
  uint8_t brightness = 255; //vycteni hodnot z fototranzistoru 
  LED.leds.setBrightness(brightness);
}

uint32_t colors(led_t &LED, Colors COLOR){ 
  switch(COLOR){
    case RED:
      return LED.leds.Color(255, 0, 0); 
    case BLUE:
      return LED.leds.Color(0, 0, 255); 
    case GREEN:
      return LED.leds.Color(0, 255, 0);
    case YELLOW: 
      return LED.leds.Color(255, 255, 0); 
    case BROWN:
      return LED.leds.Color(102, 51, 0); 
    case PURPLE:
      return LED.leds.Color(255, 0, 255);
    case PINK:
      return LED.leds.Color(255, 102, 178);
    case ORANGE:
      return LED.leds.Color(255, 128, 0);
    case AZURO:
      return LED.leds.Color(0, 255, 255);
    case BLACK:
      return LED.leds.Color(0, 0, 0);
    case  WHITE:
      return LED.leds.Color(255, 255, 255);
  }
  return 0;
}

//??
Colors get_color(led_t &LED){ 
  std::cout << LED.leds.getPixelColor(LED.pos) << "\n";
  /*switch(LED.leds.getPixelColor(LED.pos)){ //zjistit, co ta funkce vraci - v jakem formatu 
    case RED:
      return RED; 
    case BLUE:
      return BLUE;
    case GREEN:
      return GREEN;
    case YELLOW:
      return YELLOW;
    case BROWN:
      return BROWN;
    case PURPLE:
      return PURPLE;
    case PINK:
      return PINK;
    case ORANGE:
      return ORANGE;
    case AZURO:
      return AZURO;
    case BLACK:
      return BLACK;
    case  WHITE:
      return WHITE;
  }*/
}

void LED_light(led_t &LED, Colors COLOR){
  set_brightness(LED);
  LED.leds.setPixelColor(LED.pos, colors(LED, COLOR));
  LED.leds.show();
}
 
void LED_toggle(led_t &LED, Colors COLOR){
  if(get_color(LED) == BLACK)
    LED_light(LED, COLOR);
  else
    LED_light(LED, BLACK);
}
  
void LED_off(led_t &LED, Colors COLOR){
  LED_light(LED, BLACK);
}

void LEDs_all_off(led_t &LED){
  LED.leds.clear();
}

//void LEDs_all_on(led_t &LED, Colors COLOR)
void LEDs_all_on(Colors COLOR){
    for(int i = 0; i < NUM_OF_LEDS; ++i){
        LED.pos = i;
        LED_light(LED, COLOR);
    }
}

int measure_battery_voltage(){
  return float(ADC_BATTERY_PIN); //vzorec
}

bool is_battery_voltage_ok(){
  if(measure_battery_voltage() < 10) //vymyslet konstantu
    return true;
  return false;
}

void switch_off_voltage_periferies(){
  digitalWrite(SWITCH_VOLTAGE_PERIFERIES, ST_OFF);
}

void LoRa_on(){
  LoRa.begin();
}

void LoRa_write(const String msg){
  LoRa.sendMessage(msg);
}

ResponseContainer LoRa_read(){
  ResponseContainer msg;
  if(LoRa.available()){
    //oboje jde zkompilovat
    //msg = LoRa.receiveMessageComplete(true);
    msg = LoRa.receiveMessage();
  }
  return msg; 
}

void vibrate_motor_on(){
  digitalWrite(MOTOR_PIN, ST_ON);
}

void vibrate_motor_off(){
  digitalWrite(MOTOR_PIN, ST_OFF);
}

void piezo_on(){
  digitalWrite(PIEZO_PIN, ST_ON);
}

void piezo_off(){
  digitalWrite(PIEZO_PIN, ST_OFF);
}

void read_cap_but(AtTouch &CapBtn, HardwareSerial &Serial){ // otestovat, jestli funguje (cist jednou za cas - jak dlouho chci, aby motor vibroval)
  int hit_button = 10;
  int hold_button = 10;
  if(CapBtn.hit()){ //dotyk
    //vibrate_motor_on();
    Serial.print("hit: ");
    Serial.println(CapBtn.readActiveKey());
    //hit_button = CapBtn.readActiveKey(); //asi vraci cislo tlacitka 0 az 4 asi 
  }
  if(CapBtn.hold()){
    //vibrate_motor_on();
    Serial.print(" hold: ");
    Serial.println(CapBtn.getKey());
    //hold_button = CapBtn.getKey(); // zjistit, co to dela
  } //asi to pak bude rovnou vracet cislo tlacitka, ktere bylo zmacknuto
  //if(!CapBtn.hit() || !CapBtn.hold()){
    //vibrate_motor_off();
  //} 
  Serial.print("read act key ");
  Serial.println(CapBtn.readActiveKey());
  Serial.print(digitalRead(INTERRUPT_CAP_BTN));
  Serial.print("  ");
  Serial.println("nebyla splnena ani jedna podminka");
  Serial.print("Napeti na baterii ");
  Serial.println(analogRead(ADC_BATTERY_PIN)); //plne nabita baterie ma 936 mV
}

bool is_configuration_on(){
  //if(){
    //stisk kombinace tlacitek
    return true;
  //}
  //return false; 
}

bool is_configuration_end(){
  //if(){
    //kontrola ulozeni konfigurace a nastaveni hry
    return true;
  //}
  //return false; 
}

void _init_ (){ 
  pinMode(INTERRUPT_CAP_BTN, INPUT);
  pinMode(ADC_BATTERY_PIN, INPUT);
  
  Wire.begin(GPIO_SDA, GPIO_SCL);
  CapBtn.begin(INTERRUPT_CAP_BTN + 2); //protoze na Arduinu se interrupt piny cisluji od nuly a ne podle pinu jako na esp

  pinMode(SWITCH_VOLTAGE_PERIFERIES, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(LED_PIN_TOP, OUTPUT);


  digitalWrite(SWITCH_VOLTAGE_PERIFERIES, ST_ON); 
  vibrate_motor_off(); 
  piezo_off(); 
  digitalWrite(LED_PIN_TOP, ST_OFF); 

  LED.leds.begin(); 
  //CapBtn.begin(INTERRUPT_CAP_BTN, true, 9, 8); //vratila jsem zpet, ale nevim, jestli to bude ok 
}