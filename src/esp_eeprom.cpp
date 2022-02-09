#include "esp_eeprom.h"

extern char wifiSSID[33];
extern char wifiPassword[33];


bool eepromLoaded = false;


void clearLastFieldInEEPROM() {
    if (wifiSSID[0] != 0 
        || wifiPassword[0] != 0) 
    {
        wifiSSID[0] = 0;
        wifiPassword[0] = 0;
        saveEEPROM();

        disconnectSsid();
        Serial.println("so clearing saved wifiSSID and wifiPassword");
        return;
    }

    Serial.println("All is cleared");
}


uint8_t readEEPROM_variable(uint32_t index, uint8_t max_length, uint8_t *result) {
    uint8_t crc8_length[2] = {0, 0};
    EEPROM.get(index, crc8_length);

#ifdef DEBUG_FAKER
    Serial.print("Read-- "); 
    Serial.print(index); 
    Serial.print(" - crc: ");   
    Serial.print(crc8_length[0]);   
    Serial.print("| length: ");
    Serial.println(crc8_length[1]);
#endif

    if (crc8_length[1] > max_length) {
        return 0;
    }

    const uint8_t* c_ptr = EEPROM.getDataPtr();

    memcpy(result, c_ptr + index + 2, crc8_length[1]);

    uint8_t crc = crc8(result, crc8_length[1]);

#ifdef DEBUG_FAKER
    Serial.print("| got crc: ");
    Serial.println(crc);
    Serial.print("data:");
    for (int i = 0; i < crc8_length[1]; i++) {
      Serial.print(result[i]);
      Serial.print(", ");
    }
    Serial.println();
#endif

    if (crc != crc8_length[0]
       || crc8_length[1] == 0) {
      return 0;
    }

    return crc8_length[1];
}


uint8_t readEEPROM_string(uint32_t index, uint8_t max_length, uint8_t *result) {
    uint8_t result_code = readEEPROM_variable(index, max_length, result);

    if (result_code != 0) {
        result[result_code + 1] = 0;  
    } else {
        result[0] = 0;  
    }   

    return result_code + 2;
}

void readWiFiConfiguration() {
    uint8_t index = EEPROM_INDEX_WIFI_CONFIGURATION;
    uint8_t length;

    length = readEEPROM_string(index, 32, (uint8_t *)wifiSSID);
    if (length == 2) {
        return;
    }
    index += length;

    readEEPROM_string(index, 32, (uint8_t *)wifiPassword);
}

void readEEPROMVariables() {
    readWiFiConfiguration();
}

void loadEEPROM_lazy(bool forced) {
    if (!eepromLoaded || forced) {
        loadEEPROM();
    }
}

void loadEEPROM() {
    EEPROM.begin(512);
    readEEPROMVariables();
    EEPROM.end();

    eepromLoaded = true;
}

void writeEEPROM_variable(uint32_t index, uint8_t length, uint8_t *input) {
    uint8_t crc = crc8(input, length);

    const uint8_t* c_ptr = EEPROM.getDataPtr();

    if (c_ptr[index] != crc
         || c_ptr[index + 1] != length
         || memcmp(c_ptr + index + 2, input, length) != 0) 
    {
        uint8_t* ptr = EEPROM.getDataPtr();
        ptr[index] = crc;
        ptr[index + 1] = length;
        memcpy(ptr + index + 2, input, length);

#ifdef DEBUG_FAKER
       Serial.print(index); 
       Serial.print(" - crc: ");   
       Serial.print(crc);   
       Serial.print("| length: ");
       Serial.println(length);
       Serial.print("data:");
        for (int i = 0; i < length; i++) {
          Serial.print(input[i]);
          Serial.print(", ");
        }
        Serial.println();
#endif       

        Serial.println("WRITE EEPROM!!!"); 
    }
}

void writeEEPROMVariables() {
    uint8_t index = 0;
    uint8_t length;

    index = EEPROM_INDEX_WIFI_CONFIGURATION;
    length = strlen(wifiSSID);
    writeEEPROM_variable(index, length, (uint8_t *)wifiSSID);
    index += length + 2;

    length = strlen(wifiPassword);
    writeEEPROM_variable(index, length, (uint8_t *)wifiPassword);
    index += length + 2;
}

void saveEEPROM() {
    EEPROM.begin(512);
    writeEEPROMVariables();
    EEPROM.commit();
    EEPROM.end();
}