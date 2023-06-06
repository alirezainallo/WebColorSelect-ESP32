#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <Arduino.h>
#include <stdint.h>
#include <WiFi.h>


#define DB_FIRST_RUN_KEY            0xcc

#define WIFI_SSID "NiraRecorder"
#define WIFI_PASS "12345678"
#define SAMPLE_RATE 48000
#define RECORD_TIME 5 //min
#define WiFi_MODE WIFI_MODE_AP

#define MAX_BUFF_LEN             4*8 //32 Bytes

typedef struct {
  uint32_t    sampleRate;
  uint32_t    recordTime;
  char        WifiSSID[MAX_BUFF_LEN];
  char        WifiPASS[MAX_BUFF_LEN];
  wifi_mode_t WifiMode;
  // ------------------- Fixed ---------------
  uint8_t         FirstRunKey;
} Database;


void Database_init(void);

bool Database_isFirstRun(void);

void Database_setSampleRate(uint32_t sampleRate);
uint32_t Database_getSampleRate(void);

void Database_setRecordTime(uint32_t recordTime);
uint32_t Database_getRecordTime(void);

void Database_setWifiSSID(char* ssid);
const char* Database_getWifiSSID(void);

void Database_setWifiPASS(char* pass);
const char* Database_getWifiPASS(void);

void Database_setWifiMode(wifi_mode_t WifiMode);
wifi_mode_t Database_getWifiMode(void);


bool Database_needUpdate(void);
void Database_update(void);

#endif /* _DATABASE_H_ */
