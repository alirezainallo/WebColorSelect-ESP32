#ifndef _SYS_VARIABLES_H
#define _SYS_VARIABLES_H

#define ServerVersion "1.0"
// String webpage = "";

#ifdef ESP8266
#define SD_CS_pin           15         // The pin on Wemos D1 Mini for SD Card Chip-Select
#else
#define SD_CS_pin           5        // Use pin 5 on MH-T Live ESP32 version of Wemos D1 Mini for SD Card Chip-Select
#endif                               // Use pin 13 on Wemos ESP32 Pro

#endif //_SYS_VARIABLES_H
