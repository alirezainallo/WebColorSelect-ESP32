#include "Database.h"
#include <EEPROM.h>


static bool isFirstRun = false;
static bool needUpdate = false;


void Database_init(void) {
  EEPROM.begin(sizeof(Database));
  Database* db = (Database*) EEPROM.getDataPtr();

  if (DB_FIRST_RUN_KEY != db->FirstRunKey) {
    db->FirstRunKey = DB_FIRST_RUN_KEY;
    // -------------- Default Config --------------
    db->sampleRate = SAMPLE_RATE;
    db->recordTime = RECORD_TIME;
    strcpy(db->WifiSSID, WIFI_SSID);
    strcpy(db->WifiPASS, WIFI_PASS);
    db->WifiMode = WiFi_MODE;
    // --------------------------------------------
    needUpdate = true;
    isFirstRun = true;
  }
}

bool Database_isFirstRun(void) {
  return isFirstRun;
}

void Database_setSampleRate(uint32_t sampleRate) {
  Database* db = (Database*) EEPROM.getDataPtr();
  db->sampleRate = sampleRate;
  needUpdate = true;
}
uint32_t Database_getSampleRate(void) {
  return ((Database*) EEPROM.getDataPtr())->sampleRate;
}

void Database_setRecordTime(uint32_t recordTime) {
  Database* db = (Database*) EEPROM.getDataPtr();
  db->recordTime = recordTime;
  needUpdate = true;
}
uint32_t Database_getRecordTime(void){
  return ((Database*) EEPROM.getDataPtr())->recordTime;
}

void Database_setWifiSSID(char* ssid) {
  Database* db = (Database*) EEPROM.getDataPtr();
  strcpy(db->WifiSSID, ssid);
  needUpdate = true;
}
const char* Database_getWifiSSID(void) {
  return ((Database*) EEPROM.getDataPtr())->WifiSSID;
}

void Database_setWifiPASS(char* pass) {
  Database* db = (Database*) EEPROM.getDataPtr();
  strcpy(db->WifiPASS, pass);
  needUpdate = true;
}
const char* Database_getWifiPASS(void) {
  return ((Database*) EEPROM.getDataPtr())->WifiPASS;
}

void Database_setWifiMode(wifi_mode_t WifiMode) {
  Database* db = (Database*) EEPROM.getDataPtr();
  db->WifiMode = WifiMode;
  needUpdate = true;
}
wifi_mode_t Database_getWifiMode(void) {
  return ((Database*) EEPROM.getDataPtr())->WifiMode;
}

bool Database_needUpdate(void) {
    return needUpdate;
}
void Database_update(void) {
    EEPROM.commit();
    needUpdate = false;
}
