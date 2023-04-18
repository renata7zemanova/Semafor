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

extern int odpocitavadlo_timeout;
extern int vabnicka_num_of_teams;
extern int pan_hory_num_of_teams;
//extern int promenna_web2;

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
enum States {CONFIGURATION, PLAY};
enum Games {VABNICKA, PAN_HORY, SEMAFOR, ODPOCITAVADLO};
enum Buttons {BTN_ENTER, BTN_UP, BTN_DOWN, BTN_RIGHT, BTN_LEFT, NONE};

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
void play_pan_hory();
void play_semafor();
void play_odpocitavadlo();

void set_brightness(led_t &LED);
uint32_t colors(led_t &LED, Colors COLOR);
Colors get_color(led_t &LED);

void LED_light(led_t &LED, Colors COLOR);
//void LED_light(Colors COLOR);
void LED_toggle(led_t &LED, Colors COLOR);
void LED_off(led_t &LED, Colors COLOR);
void LEDs_all_off(led_t &LED);
//void LEDs_all_on(led_t &LED, Colors COLOR);
void LEDs_all_on(Colors COLOR);

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

void read_cap_but(AT42QT1070Touch &Touch_AT42, Buttons* touched_buttons);

bool is_configuration_end();

void _init_ ();

void start_server();
void wifi_enable_connect();
void wifi_disable();
void wifi_ap_enable();
void wifi_ap_disable();

#endif
