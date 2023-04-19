#include <lib.h>

#include <iostream>

const char wifi_ssid[] = "Semafor";
const char wifi_password[] = "adminadmin";

//int promenna_web2 = 4;
//IPAddress wifi_IP(192, 168, 1, 1);
//IPAddress net_mask(255, 255, 255, 0);

LoRa_E22 LoRa(RX, TX, &Serial1, UART_BPS_RATE_9600);
led_t LED(NUM_OF_LEDS, LED_PIN_TOP);
AT42QT1070Touch Touch_AT42(GPIO_SDA, GPIO_SCL);
WebServer server(80); //port 80
state_vector_t s_vect;

Colors LED_state[NUM_OF_LEDS] = {BLACK};
Buttons touched_buttons[NUM_OF_BUTTONS] = {NONE};

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

Colors get_color(led_t &LED){ 
  return LED_state[LED.pos];
}

void LED_light(led_t &LED, Colors COLOR){
  set_brightness(LED);
  LED.leds.setPixelColor(LED.pos, colors(LED, COLOR));
  LED_state[LED.pos] = COLOR;
  LED.leds.show();
}
 
void LED_toggle(led_t &LED, Colors COLOR){
  if(get_color(LED) == BLACK){
    LED_state[LED.pos] = COLOR;
    LED_light(LED, COLOR);
  }
  else{
    LED_state[LED.pos] = BLACK;
    LED_light(LED, BLACK);
  }
}
  
void LED_off(led_t &LED, Colors COLOR){
  LED_state[LED.pos] = BLACK;
  LED_light(LED, BLACK);
}

void LEDs_all_off(led_t &LED){
  for(int i = 0; i < NUM_OF_LEDS; ++i){
    LED_state[i] = BLACK;
  }
  LED.leds.clear();
}

//void LEDs_all_on(led_t &LED, Colors COLOR)
void LEDs_all_on(Colors COLOR){
    for(int i = 0; i < NUM_OF_LEDS; ++i){
      LED_state[LED.pos] = COLOR; 
      LED.pos = i;
      LED_light(LED, COLOR);
    }
}

double measure_battery_voltage(){
  return analogRead(ADC_BATTERY_PIN); //vzorec
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

void read_cap_but(AT42QT1070Touch &Touch_AT42){ 
  Touch_AT42.find_active_keys();
/*
  if(Touch_AT42.is_touched_btn_0()){
    vibrate_motor_on();
    Serial.println("enter");
    touched_buttons[0] = BTN_ENTER;
  }

  if(Touch_AT42.is_touched_btn_1()){
    vibrate_motor_on();
    Serial.println("up");
    touched_buttons[1] = BTN_UP;
  }

  if(Touch_AT42.is_touched_btn_2()){
    vibrate_motor_on();
    Serial.println("down");
    touched_buttons[2] = BTN_DOWN;
  }

  if(Touch_AT42.is_touched_btn_3()){
    vibrate_motor_on();
    Serial.println("right");
    touched_buttons[3] = BTN_RIGHT;
  }

  if(Touch_AT42.is_touched_btn_4()){
    vibrate_motor_on();
    Serial.println("left");
    touched_buttons[4] = BTN_LEFT;
  }

  if(!Touch_AT42.is_touched_btn_0() && !Touch_AT42.is_touched_btn_1() && !Touch_AT42.is_touched_btn_2() && !Touch_AT42.is_touched_btn_3() && !Touch_AT42.is_touched_btn_4()){
    vibrate_motor_off();
  }
  */
}

bool is_touched_enter(){
  return Touch_AT42.is_touched_btn(0);
}

bool is_touched_up(){
  return Touch_AT42.is_touched_btn(1);
}

bool is_touched_down(){
  return Touch_AT42.is_touched_btn(2);
}

bool is_touched_right(){
  return Touch_AT42.is_touched_btn(3);
}

bool is_touched_left(){
  return Touch_AT42.is_touched_btn(4);
}

bool is_configuration_end(){
  //if(){
    //kontrola ulozeni konfigurace a nastaveni hry
    return true;
  //}
  //return false; 
}

void _init_ (){ 
  pinMode(ADC_BATTERY_PIN, INPUT);
  pinMode(SWITCH_VOLTAGE_PERIFERIES, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(LED_PIN_TOP, OUTPUT);

  digitalWrite(SWITCH_VOLTAGE_PERIFERIES, ST_ON); 
  vibrate_motor_off(); 
  piezo_off(); 
  digitalWrite(LED_PIN_TOP, ST_OFF); 

  LED.leds.begin(); 
  Touch_AT42.begin();
}

void play_vabnicka(){

}

void play_pan_hory(){

}

void play_semafor(){

}

void play_odpocitavadlo(){
  //while(!is_touched_enter()){ //cekani na stisk enter
   // continue;
  //}
  read_cap_but(Touch_AT42);
  LEDs_all_on(GREEN);
  //tady zacatek odpocitavani casu (musim ho vycist z nastaveni)
  

  int timeout = 10; //cas v minutach - pak se bude nacitat z konfiguracniho webu
  double time_for_1_LED = timeout / NUM_OF_LEDS;
  double actual_time;
  Colors color = BLACK; //jak nastavovat barvu?
  //LEDs_all_on(LED, color);
}


void start_server(){
    wifi_ap_enable();

    server.on("/", handleRoot); //otevru v prohlizeci tuto stranku - lomitko 
    //server.on("/admin", handleAdmin);
    //server.on("/adminsave", handleAdminSave);
    server.on("/datasave", handleDataSave); //webovka s nazvem datasave
    server.on("/upload", handleUpload);//
    //server.on("/addparam", handleAddParam);
    server.onNotFound(handleRoot);
    //server.on("/style.css", handleStyle); //spusteni serveru a na nem je webovka

    server.begin();

}

void wifi_enable_connect(){
    WiFi.begin(wifi_ssid, wifi_password);
}

void wifi_disable(){ //vypnuti WiFi i serveru - musim udelat pred zacatkem hry
    WiFi.disconnect(true);
    //WiFi.disconnect();
}

void wifi_ap_enable(){
    wifi_disable(); //vypnuti WiFi, ktera se k necemu pripoji a zapinam WiFi, ktera se vytvori a jde k ni pripojit
    //WiFiAP.softAPConfig(wifi_IP, wifi_IP, net_mask); //konfigurace hlavni WiFi
    // WiFi.softAPConfig((192, 168, 1, 1), (192, 168, 1, 1), (255, 255, 255, 0)); //konfigurace hlavni WiFi
    WiFi.softAP(wifi_ssid, wifi_password); //vytvoreni hlavni WiFi - tim se take zapina server
    // WiFi.softAP("Semafor", "12345678");
    delay(500);
}

void wifi_ap_disable(){
    WiFi.softAPdisconnect(true);
}
