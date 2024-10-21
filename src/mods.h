//
// Created by Robin Derenty on 08/10/2024.
//

#ifndef MODS_H
#define MODS_H
#define NB_VAL 10
#include <Arduino.h>

#endif //MODS_H

extern byte actualMod;

struct Captor{
    float values[NB_VAL]{-41 , -41, -41 , -41, -41 , -41, -41 , -41, -41 , -41,};
    byte error{};
};

struct Captors
{
    Captor lum;
    Captor humidity;
    Captor pression;
    Captor temp;
};

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

void standardMod();
void configMod();
void maintenanceMod();
void ecoMod();