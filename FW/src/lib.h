#ifndef _LIB_
#define _LIB_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "AT42QT1070.h"

#include <WiFi.h>
#include <WebServer.h>
#include <LoRa_E22.h>

#include <handleHttp.h>

extern LoRa_E22 LoRa;
extern AT42QT1070Touch Touch_AT42;
extern WebServer server;

#define NUM_OF_COLORS 11
#define NUM_OF_LEDS 12
#define NUM_OF_BUTTONS 5

#define ST_OFF 0
#define ST_ON 1

static constexpr gpio_num_t GPIO_SCL = GPIO_NUM_8;
static constexpr gpio_num_t GPIO_SDA = GPIO_NUM_9;
static constexpr gpio_num_t ADC_BATTERY_PIN = GPIO_NUM_4;
static constexpr gpio_num_t SWITCH_VOLTAGE_PERIFERIES = GPIO_NUM_5;
static constexpr gpio_num_t MOTOR_PIN = GPIO_NUM_6;
static constexpr gpio_num_t PIEZO_PIN = GPIO_NUM_7;
static constexpr gpio_num_t LED_PIN_TOP = GPIO_NUM_10;

enum Colors {RED, BLUE, GREEN, YELLOW, BROWN, PURPLE, PINK, ORANGE, AZURO, BLACK, WHITE};
enum States {CONFIGURATION, PLAY}; //konfiguraci prijimam od Semaforu, nebo z webu
//upload, download
//vsechny startuji v download a jeden pak prepnu do upload
enum Games {VABNICKA1, VABNICKA2, PAN_HORY, SEMAFOR, ODPOCITAVADLO};
enum Buttons {BTN_ENTER, BTN_UP, BTN_DOWN, BTN_RIGHT, BTN_LEFT, NONE};

struct state_vector_t { //hlavni struktura
  Games game; 
  int vabnicka_num_of_teams;
  int pan_hory_num_of_teams;
  int odpocitavadlo_timeout;
  int semafor_max_timeout; 
};

extern state_vector_t s_vect;

struct led_t {
  led_t(const int COUNT, const int PIN) : leds(COUNT, PIN), pos(0) {} 
  Adafruit_NeoPixel leds;
  uint8_t pos; 
};
 
struct leds_t{
    led_t top;
    led_t side;
};

void play_vabnicka1();
void play_vabnicka2();
void play_pan_hory();
void play_semafor();
void play_odpocitavadlo();

void set_brightness();
uint32_t colors(Colors COLOR);
Colors get_color(int index);

void LED_light(int index, Colors COLOR);
void LED_toggle(int index, Colors COLOR);
void LED_off(int index);
void LEDs_all_off();
void LEDs_all_on(Colors COLOR);
void LEDs_all_toggle(Colors COLOR);

double measure_battery_voltage();
bool is_battery_voltage_ok();

void switch_off_voltage_periferies();

void LoRa_on();
void LoRa_write(const String msg);
ResponseContainer LoRa_read();

void vibrate_motor_on();
void vibrate_motor_off();

void piezo_on();
void piezo_off();

void tick_for_buttons();
void _init_ ();

void start_server();
void wifi_enable_connect();
void wifi_disable();
void wifi_ap_enable();
void wifi_ap_disable();

#endif
