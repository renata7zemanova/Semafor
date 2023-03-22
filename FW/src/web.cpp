#include <web.h>
#include <WiFi.h>
#include <Arduino.h>

void wifi_enable_connect(){
    WiFi.begin(wifi_ssid, wifi_password);
}

void wifi_disable(){
    WiFi.disconnect(true);
}

void wifi_ap_enable(){
    wifi_disable();
    WiFi.softAPConfig(wifi_IP, wifi_IP, net_mask);
    WiFi.softAP(wifi_ssid, wifi_password);
}

void wifi_ap_disable(){
    WiFi.softAPdisconnect(true);
}
