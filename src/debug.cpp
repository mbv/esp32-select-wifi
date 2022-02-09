#include "debug.h"

extern char wifiSSID[33];
extern char wifiPassword[33];

void debugStartup() {
    if (wifiSSID[0] == 0) {
        Serial.println("wifiSSID not set up");
    } else {
        Serial.print("wifiSSID: ");
        Serial.println(String(wifiSSID));
    }

        Serial.println(wifiPassword);
    if (wifiPassword[0] == 0) {
        Serial.println("wifiPassword not set up");
    } else {
        Serial.print("wifiPassword: ");
        Serial.println(String(wifiPassword));
    }
    
}
