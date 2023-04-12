#include <Arduino.h>
#include <Wire.h>

class AT42QT1070Touch{
    public:
    AT42QT1070Touch(gpio_num_t sda_pin, gpio_num_t scl_pin) : 
    sda_pin(sda_pin), scl_pin(scl_pin) {}
    
    private:
    gpio_num_t sda_pin;
    gpio_num_t scl_pin;
    gpio_num_t interrupt_pin;

    static constexpr int NUM_OF_BUTTONS = 5;
    int default_buttons[NUM_OF_BUTTONS] = {0}; 
    int touched_buttons[NUM_OF_BUTTONS] = {0};

    public:
    
    void begin(bool disable_auto_calibration = false){
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
        Wire.write(0); //nepouzito - deaktivace prumerovani
        Wire.write(0);
        Wire.endTransmission();

        calibration();
    }

    void find_active_keys(){ 
        int addr_of_button = 0;
        int data_reg[10] = {0};
        int data_buttons_new[5] = {0}; 

        for(int i = 4; i <= 13; ++i)
            data_reg[i - 4] = read_reg(i); 

        for(int i = 0; i < NUM_OF_BUTTONS; ++i)
            data_buttons_new[i] = 0;

        for(int i = 0; i < NUM_OF_BUTTONS * 2; ++i){
            if(i == 0)
                data_buttons_new[i] += data_reg[i] << 8;
            if(i % 2 == 0)
                data_buttons_new[i / 2] += data_reg[i] << 8;
            else
                data_buttons_new[(i - 1) / 2] += data_reg[i];
        }

        for(int i = 0; i < NUM_OF_BUTTONS; ++i)
            touched_buttons[i] = 0;

        for(int i = 0; i < NUM_OF_BUTTONS; ++i){
            if((i == 0) && abs(data_buttons_new[i] - default_buttons[i]) > 32)
                touched_buttons[i] = 1;
            if(abs(data_buttons_new[i] - default_buttons[i]) > 270)
                touched_buttons[i] = 1;
        }
    }

    void calibration(){
        int data_reg[NUM_OF_BUTTONS * 2] = {0};

        for(int i = 4; i <= (NUM_OF_BUTTONS * 2) + 3; ++i){
            data_reg[i - 4] = read_reg(i); 
        }

        for(int i = 0; i < NUM_OF_BUTTONS; ++i){
            default_buttons[i] = 0;
        }
        for(int i = 0; i < NUM_OF_BUTTONS * 2; ++i){
            if(i == 0)
                default_buttons[i] += data_reg[i] << 8;
            if(i % 2 == 0)
                default_buttons[i / 2] += data_reg[i] << 8;
            else
                default_buttons[(i - 1) / 2] += data_reg[i];
        }
    }

    int read_reg(int num_of_reg){
        Wire.beginTransmission(0x1B);
        Wire.write(num_of_reg);
        Wire.endTransmission();
        Wire.requestFrom(0x1B, 1);
        return Wire.read(); 
    }

    bool is_some_btn_touched(){
        int sum = 0;
        for(int i = 0; i < NUM_OF_BUTTONS; ++i){
            if(touched_buttons[i] == 1)
                return true;
        }
        return false; 
    }

    bool is_touched_btn_0(){
        if(touched_buttons[0] == 1)
            return true;
        return false;
    }

    bool is_touched_btn_1(){
        if(touched_buttons[1] == 1)
            return true;
        return false;
    }

    bool is_touched_btn_2(){
        if(touched_buttons[2] == 1)
            return true;
        return false;
    }

    bool is_touched_btn_3(){
        if(touched_buttons[3] == 1)
            return true;
        return false;
    }

    bool is_touched_btn_4(){
        if(touched_buttons[4] == 1)
            return true;
        return false;
    }

};
