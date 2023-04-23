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

void set_brightness(){
  uint8_t brightness = 255; //vycteni hodnot z fototranzistoru 
  LED.leds.setBrightness(brightness);
}

uint32_t colors(Colors COLOR){ 
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

Colors get_color(int index){ 
  LED.pos = index;
  return LED_state[LED.pos];
}

void LED_light(int index, Colors COLOR){
  LED.pos = index; 
  set_brightness();
  LED.leds.setPixelColor(LED.pos, colors(COLOR));
  LED_state[LED.pos] = COLOR;
  LED.leds.show();
}
 
void LED_toggle(int index, Colors COLOR){
  LED.pos = index;
  if(get_color(LED.pos) != BLACK)
    COLOR = BLACK; 
  LED_state[LED.pos] = COLOR;
  LED_light(LED.pos, COLOR);
}
  
void LED_off(int index){
  LED.pos = index;
  LED_state[LED.pos] = BLACK;
  LED_light(LED.pos, BLACK);
}

void LEDs_all_off(){
  for(int i = 0; i < NUM_OF_LEDS; ++i){
    LED_state[i] = BLACK;
  }
  LED.leds.clear();
}

void LEDs_all_on(Colors COLOR){
    for(int i = 0; i < NUM_OF_LEDS; ++i){
      LED_state[LED.pos] = COLOR; 
      LED.pos = i;
      LED_light(LED.pos, COLOR);
    }
}

void LEDs_all_toggle(Colors COLOR){
  if(get_color(0) != BLACK)
    COLOR = BLACK;
  for(int i = 0; i < NUM_OF_LEDS; ++i){
    LED.pos = i;
    LED_state[LED.pos] = COLOR;
    LED_light(LED.pos, COLOR);
  }
}

double measure_battery_voltage(){
  return analogRead(ADC_BATTERY_PIN); //vzorec aby byl ve voltech
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

bool is_touched_enter(){
  if(Touch_AT42.is_touched_btn(0)){
    vibrate_motor_on();
    return true;
  }
  vibrate_motor_off();
  return false; 
}

bool is_touched_up(){
  if(Touch_AT42.is_touched_btn(1)){
    vibrate_motor_on();
    return true;
  }
  vibrate_motor_off();
  return false; 
}

bool is_touched_down(){
  if(Touch_AT42.is_touched_btn(2)){
    vibrate_motor_on();
    return true;
  }
  vibrate_motor_off();
  return false; 
}

bool is_touched_right(){
  if(Touch_AT42.is_touched_btn(3)){
    vibrate_motor_on();
    return true;
  }
  vibrate_motor_off();
  return false; 
}

bool is_touched_left(){
  if(Touch_AT42.is_touched_btn(4)){
    vibrate_motor_on();
    return true;
  }
  vibrate_motor_off();
  return false; 
}

bool is_touched_some_btn(){
  for(int i = 0; i < NUM_OF_BUTTONS; ++i){
    if(Touch_AT42.is_touched_btn(i)){
      vibrate_motor_on();
      return true;
    }
  }
  vibrate_motor_off();
  return false; 
}

void tick_for_buttons(){ //musim volat pravidelne a casto
  Touch_AT42.tick();
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
  
  LEDs_all_on(BLACK);
}

void play_vabnicka1(){//nahodne prepinani barvy po stisku enteru 
  static int num_of_team = 1;
  s_vect.vabnicka_num_of_teams = 4;  
  static int counter = 0;
  static int old_counter = 0;
  if(is_touched_enter() && (counter == old_counter)){
    num_of_team = random(s_vect.vabnicka_num_of_teams);
    if(get_color(1) != Colors(num_of_team)){
      counter ++;
      LEDs_all_on(Colors(num_of_team));
    }
  }
  if(!is_touched_enter())
    old_counter = counter; 
}

void play_vabnicka2(){
  if(is_touched_right()){
    LEDs_all_on(RED);
  }
  if(is_touched_left()){
    LEDs_all_on(BLUE);
  }
  if(is_touched_up()){
    LEDs_all_on(GREEN);
  }
  if(is_touched_down()){
    LEDs_all_on(WHITE);
  }

}

void play_pan_hory(){

}

void play_semafor(){
  static int counter_of_pressed = 0;
  static double start_time = 0;
  s_vect.semafor_max_timeout = 20; //potom bude nastaveno z webu
  static int timeout = random(1, s_vect.semafor_max_timeout); 
  LED.pos = 1;
  if(!is_touched_enter() && counter_of_pressed == 0){
    return;
  }
  if(is_touched_enter() && counter_of_pressed == 0){
    LEDs_all_on(RED);
    start_time = millis();
    Serial.print("timeout ");
    Serial.println(timeout);

  } 
  counter_of_pressed = 1;
  double elapsed_time = double(millis() - start_time) / 1000.0;

  if(elapsed_time >= timeout){
    timeout = random(1, s_vect.semafor_max_timeout);
    start_time = millis();
    Serial.print("timeout ");
    Serial.print(timeout);
    Serial.print(" ");
    Serial.println(get_color(1));

    if(get_color(1) == RED)
      LEDs_all_on(ORANGE);
    else if(get_color(1) == ORANGE)
      LEDs_all_on(GREEN);
    else if(get_color(1) == GREEN)
      LEDs_all_on(RED);
  }
}

void play_odpocitavadlo(){
  bool time_is_over = false; 
  static int counter_of_pressed = 0;
  static double start_time = 0;
  s_vect.odpocitavadlo_timeout = 20; 
  
  if(!is_touched_enter() && counter_of_pressed == 0){
    //LED_toggle(WHITE);
    return; 
  }
  if(is_touched_enter()){ //&& counter_of_pressed == 0){
    LEDs_all_on(GREEN);
    start_time = millis();
  }
  counter_of_pressed = 1;
  
  double elapsed_time = double(millis() - start_time) / 1000.0;

  if(!time_is_over){
    for(int i = 0; i < double((elapsed_time / s_vect.odpocitavadlo_timeout) * NUM_OF_LEDS); ++i){
      LED_light(12 - i, BLACK);
    }
  }
  if(elapsed_time >= s_vect.odpocitavadlo_timeout){
    time_is_over = true;
  }
}


void start_server(){
    wifi_ap_enable();

    server.on("/", handleRoot); //otevru v prohlizeci tuto stranku - lomitko 
    //server.on("/admin", handleAdmin);
    //server.on("/adminsave", handleAdminSave);
    server.on("/datasave", handleDataSave); //webovka s nazvem datasave
    //server.on("/upload", handleUpload);//
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
