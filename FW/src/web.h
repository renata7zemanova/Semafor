#include <Arduino.h>

const char wifi_ssid[] = "Semafor";
const char wifi_password[] = "admin";

IPAddress wifi_IP(192, 168, 1, 1);
IPAddress net_mask(255, 255, 255, 0);

void wifi_enable_connect();
void wifi_disable();
void wifi_ap_enable();
void wifi_ap_disable();