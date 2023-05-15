#include <lib.h>

#include <iostream>

const char wifi_ssid[] = "UniverzalniModul";
const char wifi_password[] = "univerzalnimodul";

LoRa_E22 LoRa(RX, TX, &Serial1, UART_BPS_RATE_9600);
led_t LED(NUM_OF_LEDS, LED_PIN_TOP);
AT42QT1070Touch Touch_AT42(GPIO_SDA, GPIO_SCL);
WebServer server(80); //port 80
WiFiUDP udpSett;
state_vector_t s_vect;
Preferences pref;

ArduinoMetronome share_delay(60000); //minuta
ArduinoMetronome LED_delay(800);

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

bool is_battery_voltage_ok(double threshold){
  if(measure_battery_voltage() < threshold){ 
    return true;
  }
  return false;
}

void warn_if_battery_discharge(){
  if(!is_battery_voltage_ok(2.6)){
    switch_off_voltage_periferies();
  }
  if(!is_battery_voltage_ok(2.8)){
    LEDs_all_toggle(RED);
  }

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
  return Touch_AT42.is_touched_btn(1);
}

bool is_touched_up(){
  return Touch_AT42.is_touched_btn(2);
}

bool is_touched_down(){
  return Touch_AT42.is_touched_btn(3);
}

bool is_touched_right(){
  return Touch_AT42.is_touched_btn(4); 
}
 
bool is_touched_left(){
  return Touch_AT42.is_touched_btn(5);
}

bool is_touched_some_btn(){
  for(int i = 0; i < NUM_OF_BUTTONS; ++i){
    if(Touch_AT42.is_touched_btn(i))
      return true;
  }
  return false; 
}

void handle_btn_vibration(std::vector<Buttons> button){
  bool is_touched_some_btn = false; 
  for(int i = 0; i < button.size(); ++i){
    if(Touch_AT42.is_touched_btn(button[i])){
      is_touched_some_btn = true;
    }
  }
  if(is_touched_some_btn){
    vibrate_motor_on();
  }
  else{
    vibrate_motor_off();
  }
}

void tick_for_buttons(){ //musim volat pravidelne a casto
  Touch_AT42.tick();
}

void init_expander(){
  Wire.begin(GPIO_SDA, GPIO_SCL);
  Wire.beginTransmission(0x41); 
  Wire.write(3);
  //1011
  Wire.write(11); //nastaveni 0, 1, 3 na vstupni a 2 na vystupni
  Wire.endTransmission();

  Wire.beginTransmission(0x41); 
  Wire.write(1);
  Wire.write(0); //nastaveni log 0 na pin 2
  Wire.endTransmission();
}

void _init_ (){ 
  pinMode(ADC_BATTERY_PIN, INPUT);
  pinMode(SWITCH_VOLTAGE_PERIFERIES, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(LED_PIN_TOP, OUTPUT);
  pinMode(LED_PIN_SIDE, OUTPUT);
  pinMode(PHOTOTRANSISTOR_PIN, INPUT);

  //off je zapnuto 
  digitalWrite(SWITCH_VOLTAGE_PERIFERIES, ST_OFF); 
  vibrate_motor_off(); 
  piezo_off(); 
  digitalWrite(LED_PIN_TOP, ST_OFF); 

  LED.leds.begin(); 
  Touch_AT42.begin();
  init_expander();  
  LEDs_all_on(BLACK);
}

void play_vabnicka(){
  handle_btn_vibration({{BTN_ENTER}});
  static Colors color = Colors(1); 
  static bool old_touch = false; 

  if(old_touch && !is_touched_enter()){
    if(s_vect.vabnicka_is_random == 1){
      if(s_vect.vabnicka_is_black == 1){
        while(get_color(0) == color)
          color = Colors(random(0, s_vect.vabnicka_num_of_colors + 1));
      }
      else{
        while(get_color(0) == color)
          color = Colors(random(1, s_vect.vabnicka_num_of_colors + 1));
      }   
    }
    else{
      int index = int(color); 
      if(index < s_vect.vabnicka_num_of_colors)
        color = Colors(index + 1);
      if(index == s_vect.vabnicka_num_of_colors){
        if(s_vect.vabnicka_is_black == 1)
          color = Colors(0);
        else
          color = Colors(1);
      }
    }  
    LEDs_all_on(color);
  }
  old_touch = is_touched_enter();
}

void play_semafor(){
  handle_btn_vibration({{BTN_ENTER}});
  static int counter_of_pressed = 0;
  static ArduinoMetronome timeout(random(s_vect.semafor_min_timeout * 1000, s_vect.semafor_max_timeout * 1000));

  if(!is_touched_enter() && counter_of_pressed == 0){
    LED_light(0, WHITE);
    return;
  }
  
  if(is_touched_enter() && counter_of_pressed == 0){
    if(random(1,3) == 1)
      LEDs_all_on(RED);
    else
      LEDs_all_on(GREEN);
  } 
  counter_of_pressed = 1;

  if(timeout.loopMs()){ 
    if((get_color(1) == RED) || (get_color(1) == GREEN)){
      timeout.intervalSet(2000); //2 sekundy
      for(int i = 0; i <= 6; ++i){
        LED.pos = i;
        LED_light(LED.pos, ORANGE);
      }
    }
    else{
      timeout.intervalSet(random(s_vect.semafor_min_timeout * 1000, s_vect.semafor_max_timeout * 1000));
      if(get_color(NUM_OF_LEDS - 1) == RED)
        LEDs_all_on(GREEN);
      else
        LEDs_all_on(RED);
    }
  }
}

void play_odpocitavadlo(){
  handle_btn_vibration({{BTN_ENTER}});
  static bool time_is_over = true; 
  static int counter_of_pressed = 0;
  static double start_time = 0; 
  static ArduinoMetronome timeout_for_toggle(1000);
  
  if(!is_touched_enter() && counter_of_pressed == 0){
    LED_light(0, WHITE);
    return; 
  }
  if(is_touched_enter() && time_is_over){
    LEDs_all_on(GREEN);
    start_time = millis();
    time_is_over = false;
  }
  counter_of_pressed = 1;

  if(!time_is_over){
    double elapsed_time_sec = (double(millis() - start_time) / 1000.0);
    if((((s_vect.odpocitavadlo_timeout * 60) - elapsed_time_sec) < 60)){
      if(timeout_for_toggle.loopMs()){
        for(int i = 0; i < NUM_OF_LEDS; ++i){
          if(get_color(i) != RED)
            LED_toggle(i, YELLOW);
        }
      }
    }
    double part_of_leds = double((elapsed_time_sec / (s_vect.odpocitavadlo_timeout * 60)) * NUM_OF_LEDS);
    if(part_of_leds <= 12.01){
      for(int i = 0; i <= part_of_leds; ++i)
        LED_light(NUM_OF_LEDS - i, RED);
    }
    else 
      time_is_over = true; 
  }
}

void upload_permanently_pref(){
  pref.begin("init_Semafor", false);
  pref.putBytes("settings", &s_vect, sizeof(s_vect));
  pref.end();

}

void download_permanently_pref(){
  s_vect.game = ODPOCITAVADLO;
  s_vect.odpocitavadlo_timeout = 2;
  s_vect.semafor_min_timeout = 5;
  s_vect.semafor_max_timeout = 10;
  s_vect.vabnicka_num_of_colors = 4; 
  s_vect.vabnicka_is_black = 1;
  s_vect.vabnicka_is_random = 1;

  pref.begin("init_Semafor", false);
  int counter = pref.getUInt("counter", 0);
  if(counter == 0){
    counter++;
    pref.putUInt("counter", counter);
    Serial.println("zadna data jeste nebyla zapsana jako settings");
    pref.putBytes("settings", &s_vect, sizeof(s_vect));
  }
  else
    Serial.println("data uz byla zapsana");

  pref.getBytes("settings", &s_vect, sizeof(s_vect));
  Serial.print("hra ");
  Serial.print(s_vect.game);
  Serial.print(" ");
  Serial.print(s_vect.odpocitavadlo_timeout);
  Serial.print(" ");
  Serial.print(s_vect.semafor_min_timeout);
  Serial.print(" ");
  Serial.print(s_vect.semafor_max_timeout);
  Serial.print(" ");
  Serial.print(s_vect.vabnicka_num_of_colors);
  Serial.print(" ");
  Serial.print(s_vect.vabnicka_is_black);
  Serial.print(" ");
  Serial.println(s_vect.vabnicka_is_random);
  pref.end();
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
    server.on("/style.css", handleStyle); //spusteni serveru a na nem je webovka

    server.begin();

//udp protokol - pro sdileni nastaveni ostatnim semaforum 
    udpSett.begin(1111);

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

void share_settings(){
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;
  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  for (uint8_t i = 0; i < adapter_sta_list.num; i++){
    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
    IPAddress clientIP((uint32_t) station.ip.addr);
    Serial.print("Clients IP: ");
    Serial.println(clientIP);

    //udpSett.beginPacket(clientIP, 1111);
    udpSett.write((const uint8_t *) &s_vect, sizeof(s_vect)); //odeslani 
    udpSett.println("Ahoj svete"); //potom smazat a nechat to nahore 
    udpSett.endPacket();   
  }  
}

bool receive_settings(){
  static int counter = 0;
  static bool connected = false; 
  if(counter == 0){
    wifi_enable_connect();
  }
  counter = 1;

  if(counter == 1 && WiFi.status() == WL_CONNECTED){
    udpSett.begin(1111);
    counter = 2;
    connected = true;
  }

  if(connected){
    state_vector_t receive_vector;

    int packetSize = udpSett.parsePacket();
    if (packetSize) { 
      int len = udpSett.read((char *) &receive_vector, sizeof(receive_vector)); //cteni dat + kam chci nacist data (receive_vector)
      if(len > 0) { //jestli mi vubec neco prislo
        s_vect = receive_vector;
        upload_permanently_pref(); //ulozeni do permanentni pameti
        Serial.printf("New settings recieved and updated from Semafor ID: %d", receive_vector.game);
        wifi_disable();
        return true; 
      }
      else {
        Serial.printf("Error receive new settings from Semafor ID: %d", receive_vector.game);
      } 
    }
  }
  return false;
}

