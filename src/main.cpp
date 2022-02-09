#include <Arduino.h>

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#include <WebServer.h>
#include <DNSServer.h>

#include <HTTPClient.h>

#include <CStringBuilder.h>

#include "esp_eeprom.h"
#include "esp_wifi.h"

#include "debug.h"

#include "local_server.h"

HTTPClient http;

char wifiSSID[33] = "";
char wifiPassword[33] = "";

unsigned long requestToggle = 0;
const long requestToggleInterval = 10000;

uint8_t wifiNotConnectedTimes = 0;

bool firstRequestGetWifi = true;


void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    pinMode(LED_BUILTIN, OUTPUT);

    WiFi.persistent(false);

    Serial.println((uint32_t)(&wifiPassword));
    loadEEPROM_lazy();
    Serial.println(String(wifiPassword));

    debugStartup();

    randomSeed(analogRead(0));

    WiFi.mode(WIFI_AP_STA);

    setupAP();

    connectToSsid();

    startHttpServer();

    http.setReuse(true);
    http.setTimeout(7000);
}

void watchdog_fix() {
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
}

void loop() {
    watchdog_fix();

    unsigned long currentMillis = millis();
    if (currentMillis - requestToggle >= requestToggleInterval) {
        requestToggle = currentMillis;
        if ((WiFi.status() == WL_CONNECTED)) {
           digitalWrite(LED_BUILTIN, HIGH);

            Serial.println("! urlHost not setup");

        } else {
          digitalWrite(LED_BUILTIN, LOW);
            Serial.printf("[WIFI} not connect\n");
            if (wifiSSID[0] != 0 && wifiPassword[0] != 0) {
                wifiNotConnectedTimes++;
                if (wifiNotConnectedTimes > 5) {
                    Serial.printf("TRY Restart, wifi not connect 5 times");
                    ESP.restart();
                }
            }
        }
    }

    loopHttpServer();
}