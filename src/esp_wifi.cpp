#include "esp_wifi.h"

extern char wifiSSID[33];
extern char wifiPassword[33];

/* Soft AP network parameters */
IPAddress apIP(192, 168, 10, 1);
IPAddress netMsk(255, 255, 255, 0);

void setupAP() {
    Serial.print("WIFI AP: ");

    const char *defaultSsid = "MBV Device";
    WiFi.softAP(defaultSsid);

    Serial.println(defaultSsid);
    delay(2000);
    WiFi.softAPConfig(apIP, apIP, netMsk);
}

void connectToSsid() {
    if (wifiSSID[0] != 0 && wifiPassword[0] != 0) {
        Serial.println(WiFi.begin(wifiSSID, wifiPassword));
    }
}

void disconnectSsid() {
    WiFi.disconnect(false);
}