#include <Arduino.h>
#include "mods.h"

#define RED_BUTTON_PIN 3
#define GREEN_BUTTON_PIN 2

byte actualMod = 0;
byte lastMode = 0;
volatile bool isCounting = false; // Flag pour le compteur de 5 secondes
volatile byte varCompteur = 0;
volatile bool firstLaunch = false;

void changeMod()
{
    isCounting = true;
    varCompteur = 0;
    firstLaunch = true;
}

void setup() {

    PORTD |= (1 << PD2); // Set pullUp resistance on pin 2 (PORTD, bit 2, at 1)
    PORTD |= (1 << PD3); // Set pullUp resistance on pin 3 (PORTD, bit 3, at 1)

    cli(); // Stop interrupts
    bitClear (TCCR2A, WGM20); // WGM20 = 0
    bitClear (TCCR2A, WGM21); // WGM21 = 0
    TCCR2B = 0b00000110; // Clock / 256 so 16 micro-s and WGM22 = 0
    TIMSK2 = 0b00000001; // authorize interrupt with TOIE2
    sei(); // Restart Interrupts


    if(digitalRead(3) == LOW)
    {
        configMod();
    }
    else
    {
        standardMod();
    }
    attachInterrupt(digitalPinToInterrupt(2), changeMod, FALLING);
    attachInterrupt(digitalPinToInterrupt(3), changeMod, FALLING);
}

void loop() {
}

ISR(TIMER2_OVF_vect) {
    static byte secondCounter = 0;
    TCNT2 = 256 - 250; // 250 x 16 µS = 4 ms
    if (isCounting && firstLaunch) {secondCounter = 0; firstLaunch = false; } // When we press one of the buttons we set secondCounter to 0
    if (isCounting)
    {
        if (varCompteur++ > 250) { // 250 * 4 ms = 1 s

            varCompteur = 0;
            secondCounter++;

            if (secondCounter >= 5)
            {
                if (digitalRead(GREEN_BUTTON_PIN) == LOW)
                {
                    if (actualMod == 0)
                    {
                        ecoMod();
                    }
                    else if (actualMod == 3)
                    {
                        standardMod();
                    }
                }
                else if (digitalRead(RED_BUTTON_PIN) == LOW)
                {
                    if (actualMod == 0 || actualMod == 3)
                    {
                        lastMode = actualMod;
                        maintenanceMod();
                    }
                    else if (actualMod == 2)
                    {
                        if (lastMode == 0)
                        {
                            standardMod();
                        }
                        else
                        {
                            ecoMod();
                        }
                    }
                }
            }
        }
    }
}