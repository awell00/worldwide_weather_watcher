#include <Arduino.h>
#include "mods.h"

#define RED_BUTTON_PIN 3
#define GREEN_BUTTON_PIN 2

byte actualMod = 0;
byte lastMod = 0;
byte nbOfMinutes = 1;
volatile byte varCompteur = 0;
volatile byte secondCounter = 0;

void changeMod()
{
    cli();
    TIMSK2 = 0b00000001;      // Autoriser l'interruption locale du timer
    bitClear (TCCR2A, WGM20); // WGM20 = 0
    bitClear (TCCR2A, WGM21); // WGM21 = 0
    sei();
    varCompteur = 0;
    secondCounter = 0;
}

void setup() {

    PORTD |= (1 << PD2); // Set pullUp resistance on pin 2 (PORTD, bit 2, at 1)
    PORTD |= (1 << PD3); // Set pullUp resistance on pin 3 (PORTD, bit 3, at 1)

    cli(); // Stop interrupts

    TCCR2B = 0b00000110; // Clock / 256 soit 16 micro-s et WGM22 = 0

    TCCR1A = 0;              // Mode normal
    TCCR1B = (1 << CS12) | (1 << CS10); // Prescaler de 1024
    TCNT1 = 0;               // Compteur à 0
    TIMSK1 = (1 << TOIE1);   // Active l'interruption de débordement pour Timer1

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
    TCNT2 = 256 - 250; // 250 x 16 µS = 4 ms
    if (varCompteur++ > 250)
    {

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
                    maintenanceMod();
                }
                else if (actualMod == 2)
                {
                    if (lastMod == 0)
                    {
                        standardMod();
                    }
                    else
                    {
                        ecoMod();
                    }
                }
            }
            cli();
            TIMSK2 = 0b00000000;
            sei();
            secondCounter=0;
        }
    }
}

ISR(TIMER1_OVF_vect) {
    static uint16_t overflowCounter;
    overflowCounter = overflowCounter == NULL ? 0 : overflowCounter;
    overflowCounter++;
    if (overflowCounter >= 375 * nbOfMinutes) // On attends x min
    {
        switch (actualMod)
        {
        case 0:
            standardMod();
            break;
        case 2:
            maintenanceMod();
            break;
        case 3:
            ecoMod();
            break;
        default:
            break;
        }
        overflowCounter = 0;
    }
}