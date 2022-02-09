#pragma once

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>


#include "utils.h"
#include "esp_wifi.h"
#include "esp_eeprom.h"
#include "local_server.h"


void handleRoot();
void handleConnect();
void handleApple();
boolean captivePortal();
void handleSaveConfig();
void handleNotFound();
