#ifndef _AT42QT_
#define _AT42QT_

#include <Arduino.h>
#include <Wire.h>
#include <vector>

class MovingAverage{
    // std::array<int, 100> raw_data; //vytvori pole, kde je .size, iterace apod
    std::vector<int> raw_data;
    int index = 0;
    int sum = 0;

    public:
    int get_average(){
        return sum / raw_data.size();
    }

    void push_sample(int sample){
        index = (index + 1) % raw_data.size();
        sum -= raw_data[index];
        raw_data[index] = sample;
        sum += raw_data[index];
    }

    void setup(int size, int initial_value){
        raw_data.resize(size);
        for(int i = 0; i < raw_data.size(); ++i){
            raw_data[i] = initial_value; 
        }
        sum = initial_value * raw_data.size();
        index = 0;
    }

};

class TouchButton{
    MovingAverage average;
    bool is_pressed = false;
    bool detected_press = false; 
    int counter = 0;
    int detection_threshold; 

    public:
    bool is_btn_pressed(){
        return is_pressed;
    }

    void tick(int new_value){
        if(abs(new_value - average.get_average()) > detection_threshold){
            detected_press = true;
            counter++;
        }
        else{
            average.push_sample(new_value);
            detected_press = false; 
            is_pressed = false;
            counter = 0;
        }
        if(detected_press && (counter > 3)){
            is_pressed = true; 
            counter = 0;
        }
    }

    void setup(int threshold, int average_sample_count, int initial_value){
        detection_threshold = threshold; 
        average.setup(average_sample_count, initial_value);
    }
};

class AT42QT1070Touch{
    public:
    AT42QT1070Touch(gpio_num_t sda_pin, gpio_num_t scl_pin) : 
    sda_pin(sda_pin), scl_pin(scl_pin) {}
    
    private:
    gpio_num_t sda_pin;
    gpio_num_t scl_pin;
    
    static constexpr int NUM_OF_BUTTONS = 7;  

    TouchButton Touch_buttons[NUM_OF_BUTTONS];

    public:
    
    void begin(){
        Wire.begin(sda_pin, scl_pin);

        Wire.beginTransmission(0x1B);
        Wire.write(0x39); // sets register pointer to the reset register (0x39)
        Wire.write(0xFF); // send non-zero value to initiate a reset
        Wire.endTransmission();

        Wire.beginTransmission(0x1B);
        Wire.write(53); // sets register pointer 53 - adresa registru
        Wire.write(15); // deaktivace guard key (je na poz. 0) - co chci zapsat do registru
        Wire.endTransmission();

        delay(250); // wait for device to restart

        //rozdeleni tlacitek do skupin, abych mohla cist stisk vice tlacitek
        //nelze cist soucasne tlacitka ve stejne skupine
        //kazde tlacitko v jine skupine
        Wire.beginTransmission(0x1B); 
        Wire.write(39); //od registru 39 az po registr 45
        Wire.write(32); 
        Wire.write(32); 
        Wire.write(32); 
        Wire.write(32); 
        Wire.write(32); 
        Wire.write(32); 
        Wire.write(0); //nepouzito - deaktivace prumerovani
        Wire.endTransmission();

        for(int i = 0; i < NUM_OF_BUTTONS; ++i){
            int threshold = 100; 
            if(i == 0 || i == 1)
                threshold = 3;
            Touch_buttons[i].setup(threshold, 10, get_raw_data_btn(i)); 
        }
    }

    void tick(){ 
        for(int i = 0; i < NUM_OF_BUTTONS; ++i)
            Touch_buttons[i].tick(get_raw_data_btn(i));
    }

    int get_raw_data_btn(int index){ 
        int data_reg[2] = {0};
        int k = 0;
        for(int i = (4 + (index * 2)); k < 2; ++i){
            data_reg[k] = read_reg(i); 
            ++k;
        }
        return (data_reg[0] << 8) + data_reg[1];
    }

    int read_reg(int num_of_reg){
        Wire.beginTransmission(0x1B);
        Wire.write(num_of_reg);
        Wire.endTransmission();
        Wire.requestFrom(0x1B, 1);
        return Wire.read(); 
    }

    bool is_touched_btn(int index){
        return Touch_buttons[index].is_btn_pressed();
    }
};

#endif
