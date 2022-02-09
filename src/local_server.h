#pragma once

#include <Arduino.h>


#include <WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <WebServer.h>

#include "server_configuration_handlers.h"

void startHttpServer();
void loopHttpServer();

void setNoCacheHeaders();
