#include <Arduino.h>
#include <Wire.h>
#include <AtTouch.h> 
#include <Adafruit_NeoPixel.h>

#include <LoRa_E22.h>

#define INTERRUPT_CAP_BTN 3
#define ADC_BATTERY_PIN 4
#define SWITCH_VOLTAGE_PERIFERIES 5
#define MOTOR_PIN 6
#define PIEZO_PIN 7
#define SCL_CAP_BTN 8
#define SDA_CAP_BTN 9
#define LED_PIN_TOP 10

#define NUM_OF_COLORS 11
#define NUM_OF_LEDS 12

#define ST_OFF 0
#define ST_ON 1


enum Colors {RED, BLUE, GREEN, YELLOW, BROWN, PURPLE, PINK, ORANGE, AZURO, BLACK, WHITE};
enum States {CONFIGURATION, PLAY};

struct led_t {
  led_t(const int COUNT, const int PIN) : leds(COUNT, PIN/*, LED_WS2812*/), pos(0) {} //typ LEDek? jak nastavit a co nastavit?
  Adafruit_NeoPixel leds;
  uint8_t pos; 
};

struct leds_t{
    led_t top;
    led_t side;
};


void set_brightness(led_t &LED);
uint32_t colors(led_t &LED, Colors COLOR);
Colors get_color(led_t &LED);

void LED_light(led_t &LED, Colors COLOR);
void LED_toggle(led_t &LED, Colors COLOR);
void LED_off(led_t &LED, Colors COLOR);
void LEDs_all_off(led_t &LED);

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

void read_cap_but(AtTouch &CapBtn);

bool is_configuration_on();
bool is_configuration_end();

void _init_ ();