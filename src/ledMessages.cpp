#include "ledMessages.h"
#include <ChainableLEDDIY.h>

ChainableLED led(8, 9);

void standardModLed(){
    led.setColorRGB(0, 255, 0); // Green
}

void configModLed()
{
    //led.setColorRGB(255, 255, 0); // Yellow
    led.setColorRGB(255, 0, 0);//Parce que Robin ne voit rien
}

void ecoModLed()
{
    led.setColorRGB(0, 0, 255); // Blue
}

void maintenanceModLed()
{
    led.setColorRGB(255, 60, 0); // Orange

}

void errorAccessRTC()
{
    while (1)
    {
        led.setColorRGB(255, 0, 0);
        delay(500);
        led.setColorRGB(0, 0, 255);
        delay(500);
    }

}

void errorAccessGPS()
{
    while (1)
    {
        led.setColorRGB(255, 0, 0);
        delay(500);
        led.setColorRGB(255, 255, 0);
        delay(500);
    }
}

void errorAccessCaptor()
{
    while(1)
    {
        led.setColorRGB(255, 0, 0);
        delay(500);
        led.setColorRGB(0, 255, 0);
        delay(500);

    }
}

void errorDataCaptorIllogical()
{
    while (1)
    {
        led.setColorRGB(255, 0, 0);
        delay(333);
        led.setColorRGB(0, 255, 0);
        delay(667);
    }
}

void errorSDFull()
{
    while (1)
    {
        led.setColorRGB(255, 0, 0);
        delay(500);
        led.setColorRGB(255, 255, 255);
        delay(500);
    }
}

void errorAccessOrWriteSD()
{
    while (1)
    {
        led.setColorRGB(255, 0, 0);
        delay(333);
        led.setColorRGB(255, 255, 255);
        delay(667);
    }
}
