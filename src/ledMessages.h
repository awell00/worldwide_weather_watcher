//
// Created by Robin Derenty on 08/10/2024.
//

#ifndef LEDMESSAGES_H
#define LEDMESSAGES_H

#endif //LEDMESSAGES_H


void standardModLed();
void configModLed();
void ecoModLed();
void maintenanceModLed();

void errorAccessRTC();
void errorAccessGPS();
void errorAccessCaptor();
void errorDataCaptorIllogical();
void errorSDFull();
void errorAccessOrWriteSD();

void initLed();