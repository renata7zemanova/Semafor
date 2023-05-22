#ifndef _LIB_
#define _LIB_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoMetronome.hpp>
#include <Preferences.h>

#include "AT42QT1070.h"

#include <WiFi.h>
#include "esp_wifi.h"
#include <WiFiUdp.h>
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
static constexpr gpio_num_t SWITCH_VOLTAGE_PERIFERIES = GPIO_NUM_2;
static constexpr gpio_num_t MOTOR_PIN = GPIO_NUM_6;
static constexpr gpio_num_t PIEZO_PIN = GPIO_NUM_7;
static constexpr gpio_num_t LED_PIN_TOP = GPIO_NUM_10;
static constexpr gpio_num_t LED_PIN_SIDE = GPIO_NUM_3;
static constexpr gpio_num_t PHOTOTRANSISTOR_PIN = GPIO_NUM_1;

enum Colors {BLACK, RED, BLUE, GREEN, YELLOW, WHITE, ORANGE, BROWN, PURPLE, PINK, AZURO};
enum States {CONFIGURATION, PLAY};
enum Games {VABNICKA, SEMAFOR, ODPOCITAVADLO};
enum Buttons {BTN_ENTER, BTN_UP, BTN_DOWN, BTN_RIGHT, BTN_LEFT, NONE};

struct state_vector_t { //hlavni struktura
  Games game; 
  int vabnicka_num_of_colors;
  int vabnicka_is_black;
  int vabnicka_is_random;
  int odpocitavadlo_timeout;
  int semafor_min_timeout;
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

void play_vabnicka();
void play_semafor();
void play_odpocitavadlo();

int read_phototransistor();
void set_LED_brightness();
uint32_t colors(Colors COLOR);
Colors get_color(int index);

void LED_light(int index, Colors COLOR);
void LED_toggle(int index, Colors COLOR);
void LED_off(int index);
void LEDs_all_off();
void LEDs_all_on(Colors COLOR);
void LEDs_all_toggle(Colors COLOR);

double measure_battery_voltage();
bool is_battery_voltage_ok(double threshold);
void warn_if_battery_discharge();

void switch_off_voltage_periferies();
void switch_off_voltage_side_LEDs();

void vibrate_motor_on();
void vibrate_motor_off();

void piezo_on();
void piezo_off();

bool is_touched_enter();
bool is_touched_up();
bool is_touched_down();
bool is_touched_right();
bool is_touched_left();
bool is_touched_some_btn();
void handle_btn_vibration(std::vector<Buttons> button);

void tick_for_buttons();
void init_expander();
void _init_ ();

void upload_permanently_pref();
void download_permanently_pref();

void start_server();
void wifi_enable_connect();
void wifi_disable();
void wifi_ap_enable();
void wifi_ap_disable();

void share_settings();
bool receive_settings();

#endif
