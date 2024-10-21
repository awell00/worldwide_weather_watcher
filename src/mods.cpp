#include "mods.h"
#include "ledMessages.h"
#include <RTClib.h>
#include "Seeed_BME280.h"
#include <SD.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

#define LUMINOSITY_CAPTOR 0
#define NB_CAPTORS 5
#define NB_VAL 10
#define CHIP_SELECT 4
#define TIME_FOR_STOP_CONFIG_MOD 1800000
#define MAGIC_WORD 123
#define VERSION 00001

void standardMod()
{
    Serial.flush();
    Serial.end();

    standardModLed();
    actualMod = 0;


    BME280 bme280;
    RTC_DS1307 rtc;

    if(!bme280.init() || analogRead(LUMINOSITY_CAPTOR)<0 || analogRead(LUMINOSITY_CAPTOR) > 1023){
        errorAccesCaptor();
    }

    if (!rtc.begin())
    {
        errorAccesRTC();
    }

    if (!SD.begin(CHIP_SELECT)) {
        errorAccesOrWriteSD();
    }

    //-- Captors Acquisition --\\


    //-- Acquisition Writing -- \\

}

void configMod()
{
    configModLed();
    actualMod = 1;

    unsigned long lastActivityTime = millis();

    struct Parameters {
        uint8_t LOG_INTERVALL = 10;
        int FILE_MAX_SIZE = 4096;

        bool LUMIN = true;
        int LUMIN_LOW = 255;
        int LUMIN_HIGH = 768;

        bool TEMP_AIR = true;
        int MIN_TEMP_AIR = -10;
        int MAX_TEMP_AIR = 60;

        bool HYGR = true;
        int HYGR_MINT = 0;
        int HYGR_MAXT = 50;

        bool PRESSURE = true;
        int PRESSURE_MIN = 850;
        int PRESSURE_MAX = 1080;

        uint8_t TIMEOUT = 30;
    };

    Parameters params;

    Serial.begin(9600);

    while (lastActivityTime - millis() < TIME_FOR_STOP_CONFIG_MOD)
    {
        Parameters defaultParams;

        if (EEPROM.read(0) != MAGIC_WORD)
        {
            EEPROM.put(1, defaultParams);
            EEPROM.write(0, MAGIC_WORD);
        }

        Serial.println("Config Mod");

        if (Serial.available() > 0)
        {
            String input = Serial.readStringUntil('\n');
            String paramName = input.substring(0, input.indexOf('='));
            int value = input.substring(input.indexOf('=') + 1).toInt();

            uint8_t hash = 0;

            for (int i = 0; i < paramName.length(); i++) {
                hash += paramName[i];
            }

            switch (hash) {
                case 'L' + 'U' + 'M' + 'I' + 'N':  // "LUMIN"
                    if (value == 0 || value == 1) {
                        params.LUMIN = value;
                        Serial.print(params.LUMIN);
                    }
                    lastActivityTime = millis();
                    break;

                case 'L' + 'U' + 'M' + 'I' + 'N' + '_' + 'L' + 'O' + 'W':  // "LUMIN_LOW"
                    if (value >= 0 && value <= 1023) {
                        params.LUMIN_LOW = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'L' + 'U' + 'M' + 'I' + 'N' + '_' + 'H' + 'I' + 'G' + 'H':  // "LUMIN_HIGH"
                    if (value >= 0 && value <= 1023) {
                        params.LUMIN_HIGH = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'T' + 'E' + 'M' + 'P' + '_' + 'A' + 'I' + 'R':  // "TEMP_AIR"
                    if (value == 0 || value == 1) {
                        params.TEMP_AIR = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'M' + 'I' + 'N' + '_' + 'T' + 'E' + 'M' + 'P' + '_' + 'A' + 'I' + 'R':  // "MIN_TEMP_AIR"
                    if (value >= -40 && value <= 85) {
                        params.MIN_TEMP_AIR = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'M' + 'A' + 'X' + '_' + 'T' + 'E' + 'M' + 'P' + '_' + 'A' + 'I' + 'R':  // "MAX_TEMP_AIR"
                    if (value >= -40 && value <= 85) {
                        params.MAX_TEMP_AIR = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'H' + 'Y' + 'G' + 'R':  // "HYGR"
                    if (value == 0 || value == 1) {
                        params.HYGR = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'H' + 'Y' + 'G' + 'R' + '_' + 'M' + 'I' + 'N' + 'T':  // "HYGR_MINT"
                    if (value >= -40 && value <= 85) {
                        params.HYGR_MINT = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'H' + 'Y' + 'G' + 'R' + '_' + 'M' + 'A' + 'X' + 'T':  // "HYGR_MAXT"
                    if (value >= -40 && value <= 85) {
                        params.HYGR_MAXT = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'P' + 'R' + 'E' + 'S' + 'S' + 'U' + 'R' + 'E':  // "PRESSURE"
                    if (value == 0 || value == 1) {
                        params.PRESSURE = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'P' + 'R' + 'E' + 'S' + 'S' + 'U' + 'R' + 'E' + '_' + 'M' + 'I' + 'N':  // "PRESSURE_MIN"
                    if (value >= 300 && value <= 1100) {
                        params.PRESSURE_MIN = value;
                    }
                    lastActivityTime = millis();
                    break;

                case 'P' + 'R' + 'E' + 'S' + 'S' + 'U' + 'R' + 'E' + '_' + 'M' + 'A' + 'X':  // "PRESSURE_MAX"
                    if (value >= 300 && value <= 1100) {
                        params.PRESSURE_MAX = value;
                    }
                    lastActivityTime = millis();
                    break;

                default:
                    Serial.println("Unrecognised Command");
                    lastActivityTime = millis();
                    break;
            }
            delay(200);
        }
        Serial.flush();
    }
    EEPROM.put(1, params);

}

void maintenanceMod()
{
    maintenanceModLed();
    actualMod = 2;

    Serial.begin(9600);
    Serial.flush();

    SD.end();

    BME280 bme280;
    RTC_DS1307 rtc;

    if(!bme280.init() || analogRead(LUMINOSITY_CAPTOR)<0 || analogRead(LUMINOSITY_CAPTOR) > 1023){
        errorAccessCaptor();
    }

    if (!rtc.begin())
    {
        errorAccessRTC();
    }

    Serial.println(rtc.now().timestamp(DateTime::TIMESTAMP_DATE) + ':' + bme280.getHumidity() + ':' + bme280.getPressure() + ':' + bme280.getTemperature());
    Serial.flush();
}

void ecoMod()
{
    ecoModLed();
    actualMod = 3;
}

