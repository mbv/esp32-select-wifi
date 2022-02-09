#pragma once

#include <Arduino.h>
#include <EEPROM.h>

#include "crc8.h"
#include "esp_wifi.h"

#define EEPROM_INDEX_WIFI_CONFIGURATION 0

void clearLastFieldInEEPROM();
void clearDeviceIdInEEPROM();
void loadEEPROM_lazy(bool force = false); 
void loadEEPROM();
void saveEEPROM();