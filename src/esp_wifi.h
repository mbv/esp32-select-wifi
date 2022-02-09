#pragma once

#include <Arduino.h>

#include <IPv6Address.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#include "utils.h"

void setupMAC();
void setupAP();
void connectToSsid();
void disconnectSsid();
