#include <web.h>
#include <Arduino.h>

//WiFiAPClass WiFiAP;

void wifi_enable_connect(){
    WiFi.begin(wifi_ssid, wifi_password);
}

void wifi_disable(){
    WiFi.disconnect(true);
    //WiFi.disconnect();
}

void wifi_ap_enable(){
    wifi_disable(); //vypnuti WiFi, ktera se k necemu pripoji a zapinam WiFi, ktera se vytvori a jde k ni pripojit
    //WiFiAP.softAPConfig(wifi_IP, wifi_IP, net_mask); //konfigurace hlavni WiFi
    WiFi.softAPConfig((192, 168, 1, 1), (192, 168, 1, 1), (255, 255, 255, 0)); //konfigurace hlavni WiFi
    WiFi.softAP(wifi_ssid, wifi_password); //vytvoreni hlavni WiFi - tim se take zapina server
    delay(500);
}

void wifi_ap_disable(){
    WiFi.softAPdisconnect(true);
}
