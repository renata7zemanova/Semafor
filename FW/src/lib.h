#include <Arduino.h>
#include <Wire.h>

#include <SmartLeds.h>
#include <Color.h>
#include <AtTouch.h>

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

enum Colors {RED = 0, BLUE, GREEN, YELLOW, BROWN, PURPLE, PINK, ORANGE, AZURO, BLACK, WHITE};

//enum Direct {RIGHT, LEFT};

struct led_t {
  led_t(const int COUNT, const int PIN, const int CHANNEL) : leds(LED_WS2812, COUNT, PIN, CHANNEL) {}

  SmartLed leds;
  int pos;
};

//tuto strukturu asi nebudu potrebovat
struct leds_t{
    led_t top;
    led_t task;
    led_t eval;
    Colors play_array;
    Colors task_array;
    Colors eval_array;
};

AtTouch CapBtn; 

void _init_ (){
  pinMode(INTERRUPT_CAP_BTN, INPUT);
  pinMode(ADC_BATTERY_PIN, INPUT);
  pinMode(SWITCH_VOLTAGE_PERIFERIES, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(LED_PIN_TOP, OUTPUT);


  digitalWrite(SWITCH_VOLTAGE_PERIFERIES, ST_ON); 
  digitalWrite(MOTOR_PIN, ST_OFF); 
  digitalWrite(PIEZO_PIN, ST_OFF); 
  digitalWrite(LED_PIN_TOP, ST_OFF); 
}

void LED_light(const int TYPE, const int COLOR, const int FIRST_LED, const int LAST_LED){

}

//zjistit, jak udelat v C++ delay
void LED_toggle(const int TYPE, const int COLOR, const int FIRST_LED, const int LAST_LED, const int NUM_OF_REPEAT = 1){
  for(int i = 0; i < NUM_OF_REPEAT; ++i){
    LED_light(TYPE, COLOR, FIRST_LED, LAST_LED);
    delay(200);
    LED_light(TYPE, BLACK, FIRST_LED, LAST_LED);
    delay(200);
  }
}
  
void LED_off(const int TYPE, const int COLOR, const int FIRST_LED, const int LAST_LED){
  LED_light(TYPE, BLACK, FIRST_LED, LAST_LED);
}

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

int measure_battery_voltage(){
  
  return float(ADC_BATTERY_PIN); //vzorec
}

bool is_battery_voltage_ok(){
  if(measure_battery_voltage() < 10) //vymyslet konstantu
    return true;
  return false;
}

void switch_off_voltage_periferies(){
  if(!is_battery_voltage_ok()){
    //SWITCH_VOLTAGE_PERIFERIES //vypnout napajeni LEDkam 
  }
}

bool is_some_btn_press(){
  if(digitalRead(INTERRUPT_CAP_BTN))
    return true;
  return false; 
}

//zkusit najit knihovnu pro ten cip
void read_cap_btn(){ //komunikace po I2C s prevodnikem pro kapacitni tlacitka - pouze hruby naznak struktury
  if(is_some_btn_press()){
    Wire.begin();
    Wire.requestFrom(8, 6); //request 6 bytes from peripheral device #8
    while(Wire.available()){
      int x = Wire.read();
      delay(200);
    }
    Wire.end();
  }
}

//LoRa - UART 
void LoRa_write(){
  Serial.begin(9600);
  if(Serial.availableForWrite()){
    Serial.write("abcd"); //zprava
  }
  Serial.end();
}

char LoRa_read_bit(){
  char c = 'x';
  //const int LENGTH = 8;
  //int array[LENGTH] = {0};
  Serial.begin(9600);
  if(Serial.available()){
    c = Serial.read();
    //Serial.readBytes(array, 8);
  }
  Serial.end();
  return c;
}

void read_cap_but(){ // otestovat, jestli funguje
  CapBtn.begin(INTERRUPT_CAP_BTN);
  if(CapBtn.hit()){ //dotyk
    int hit_button = CapBtn.readActiveKey(); //asi vraci cislo tlacitka 0 az 4 asi 
  }
  if(CapBtn.hold()){
    int hold_button = CapBtn.getKey(); // zjistit, co to dela
  }
}


