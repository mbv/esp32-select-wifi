#pragma once

#include <Arduino.h>
#include <IPAddress.h>
#include <WiFi.h>

void UTILS_byteArrayToHexString(const uint8_t array[], unsigned int len, char buffer[]);
int UTILS_hexStringToByteArray(char *input, uint8_t *output);
String UTILS_rssiToString(int32_t rssi);
String UTILS_encryptionTypeToString(int8_t encryptionType);
int8_t UTILS_encryptionTypeToInt(int8_t encryptionType);

boolean isIp(String str);
String toStringIp(IPAddress ip);
