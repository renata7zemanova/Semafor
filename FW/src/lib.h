#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "AT42QT1070.h"

#include <LoRa_E22.h>

#define ADC_BATTERY_PIN 4
#define SWITCH_VOLTAGE_PERIFERIES 5
#define MOTOR_PIN 6
#define PIEZO_PIN 7
#define LED_PIN_TOP 10

#define NUM_OF_COLORS 11
#define NUM_OF_LEDS 12
#define NUM_OF_BUTTONS 5

#define ST_OFF 0
#define ST_ON 1

static constexpr gpio_num_t GPIO_SCL = GPIO_NUM_8;
static constexpr gpio_num_t GPIO_SDA = GPIO_NUM_9;

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
void play_odpocitavadlo(HardwareSerial &Serial);

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

int measure_battery_voltage();

bool is_battery_voltage_ok();

void switch_off_voltage_periferies();

void LoRa_on();
void LoRa_write(const String msg);
ResponseContainer LoRa_read();

void vibrate_motor_on();
void vibrate_motor_off();

void piezo_on();
void piezo_off();

void read_cap_but(AT42QT1070Touch &Touch_AT42, Buttons* touched_buttons, HardwareSerial &Serial);

bool is_configuration_on();
bool is_configuration_end();

void _init_ (HardwareSerial &Serial);