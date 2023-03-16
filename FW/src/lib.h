#include <Arduino.h>
#include <Wire.h>

#include <SmartLeds.h>
#include <Color.h>
#include <AtTouch.h>

#include <LoRa_E22.h>

//#include <cstdlib>
//#include <map>


#define INTERRUPT_CAP_BTN 3
#define ADC_BATTERY_PIN 4
#define SWITCH_VOLTAGE_PERIFERIES 5
#define MOTOR_PIN 6
#define PIEZO_PIN 7
#define SCL_CAP_BTN 8
#define SDA_CAP_BTN 9
#define LED_PIN_TOP 10

#define NUM_OF_COLORS 11

#define ST_OFF 0
#define ST_ON 1

LoRa_E22 LoRa = LoRa_E22(&Serial1); //mozna Serial0

enum Colors {RED, BLUE, GREEN, YELLOW, BROWN, PURPLE, PINK, ORANGE, AZURO, BLACK, WHITE};
enum States {CONFIGURATION, PLAY};

struct led_t {
  led_t(const int COUNT, const int PIN, const int CHANNEL) : leds(LED_WS2812, COUNT, PIN, CHANNEL) {}
  SmartLed leds;
};

struct leds_t{
    led_t top;
    led_t side;
};

Rgb colors(Colors COLOR){
  switch(COLOR){
    case RED:
      return Rgb{255, 0, 0};
    case BLUE:
      return Rgb{0, 0, 255};
    case GREEN:
      return Rgb{0, 255, 0};
    case YELLOW:
      return Rgb{255, 255, 0};
    case BROWN:
      return Rgb{102, 51, 0};
    case PURPLE:
      return Rgb{255, 0, 255};
    case PINK:
      return Rgb{255, 102, 178};
    case ORANGE:
      return Rgb{255, 128, 0};
    case AZURO:
      return Rgb{0, 255, 255};
    case BLACK:
      return Rgb{0, 0, 0};
    case  WHITE:
      return Rgb{255, 255, 255};
  }
}

void LED_light(led_t &LED, Colors COLOR, const int FIRST_LED, const int LAST_LED){ 
  for(int pos = FIRST_LED; pos <= LAST_LED; ++pos){
    LED.leds[pos] = colors(COLOR);
  }
}

//upravit, aby nebylo blokujici 
void LED_toggle(led_t &LED, Colors COLOR, const int FIRST_LED, const int LAST_LED, const int NUM_OF_REPEAT = 1){
  for(int i = 0; i < NUM_OF_REPEAT; ++i){
    LED_light(LED, COLOR, FIRST_LED, LAST_LED);
    delay(200);
    LED_light(LED, BLACK, FIRST_LED, LAST_LED);
    delay(200);
  }
}
  
void LED_off(led_t &LED, const int TYPE, Colors COLOR, const int FIRST_LED, const int LAST_LED){
  LED_light(LED, BLACK, FIRST_LED, LAST_LED);
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

void read_cap_but(AtTouch &CapBtn){ // otestovat, jestli funguje (cist jednou za cas - jak dlouho chci, aby motor vibroval)
  CapBtn.begin(INTERRUPT_CAP_BTN);
  if(CapBtn.hit()){ //dotyk
    vibrate_motor_on();
    int hit_button = CapBtn.readActiveKey(); //asi vraci cislo tlacitka 0 az 4 asi 
  }
  if(CapBtn.hold()){
    vibrate_motor_on();
    int hold_button = CapBtn.getKey(); // zjistit, co to dela
  } //asi to pak bude rovnou vracet cislo tlacitka, ktere bylo zmacknuto
  if(!CapBtn.hit() || !CapBtn.hold()){
    vibrate_motor_off();
  } 
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
  pinMode(SWITCH_VOLTAGE_PERIFERIES, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(LED_PIN_TOP, OUTPUT);


  digitalWrite(SWITCH_VOLTAGE_PERIFERIES, ST_ON); 
  vibrate_motor_off(); 
  piezo_off(); 
  digitalWrite(LED_PIN_TOP, ST_OFF); 

}