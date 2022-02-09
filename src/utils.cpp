#include "utils.h"

void UTILS_byteArrayToHexString(const uint8_t array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

int UTILS_hexStringToByteArray(char *input, uint8_t *output) {
    int length = strlen(input) / 2;
    int tmp;

    for (int i = 0; i < length; i++) {
        sscanf(input + 2*i, "%02x", &tmp);
        output[i] = (uint8_t)tmp;
    }

    return length;
}

/** Is this an IP? */
boolean isIp(String str) {
    for (size_t i = 0; i < str.length(); i++) {
        int c = str.charAt(i);
        if (c != '.' && (c < '0' || c > '9')) {
            return false;
        }
    }
    return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
    String res = "";
    for (int i = 0; i < 3; i++) {
        res += String((ip >> (8 * i)) & 0xFF) + ".";
    }
    res += String(((ip >> 8 * 3)) & 0xFF);
    return res;
}


String UTILS_rssiToString(int32_t rssi) {
    if (rssi > -40) {
        return String(F("Excellent"));
    } else if (rssi > -55) {
        return String(F("Good"));
    } else if (rssi > -70) {
        return String(F("Fair"));
    }

    return String(F("Poor"));
}

String UTILS_encryptionTypeToString(int8_t encryptionType) {
    switch (encryptionType) {
        case WIFI_AUTH_OPEN:
            return String(F("Open"));
        case WIFI_AUTH_WEP:
            return String(F("WEP"));
        case WIFI_AUTH_WPA_PSK:
            return String(F("WPA-TKIP"));
        case WIFI_AUTH_WPA2_PSK:
            return String(F("WPA2-AES"));
        case WIFI_AUTH_WPA_WPA2_PSK:
            return String(F("WPA-WPA2-MIXED"));
        default:
            return String(F("Open"));
    }
}

int8_t UTILS_encryptionTypeToInt(int8_t encryptionType) {
    switch (encryptionType) {
        case WIFI_AUTH_OPEN:
            return 0;
        case WIFI_AUTH_WEP:
            return 1;
        case WIFI_AUTH_WPA_PSK:
            return 2;
        case WIFI_AUTH_WPA2_PSK:
            return 4;
        case WIFI_AUTH_WPA_WPA2_PSK:
            return 6;
        default:
            return 0;
    }
}

