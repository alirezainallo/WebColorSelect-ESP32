#include <Arduino.h>
#include "main.h"

#define LED_PIN 2
void serial_init(unsigned long rate);
uint32_t tick_ms = 500;
void heartBeat_init(uint32_t ms);
void heartBeat_loop(void);
const unsigned long serialRate = 115200;
const char WiFi_SSID[] = "DIR-822-7732";//Aavan2//Mabna Telecom//DIR-822-7732
const char WiFi_PASS[] = "Yasahebazzaman1400";//Yasahebazzaman1400//m@bn@2062//Yasahebazzaman1400
void Wifi_onEvent(arduino_event_id_t event);
void WiFi_init(const char * ssid, const char * pass);

void setup() {
  // put your setup code here, to run once:
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable   detector
  serial_init(serialRate);
  WiFi_init(WiFi_SSID, WiFi_PASS);
  heartBeat_init(600);
}

void loop() {
  // put your main code here, to run repeatedly:
  heartBeat_loop();
}

void heartBeat_init(uint32_t ms){
  tick_ms = ms;
  pinMode(LED_PIN, OUTPUT);
}
void heartBeat_loop(void){
  static uint32_t nextTick;
  static uint32_t currTick;
  currTick = millis();
  if(currTick >= nextTick){
    static bool preLevel = false;
    preLevel = !preLevel;
    digitalWrite(LED_PIN, preLevel);
    nextTick = currTick + tick_ms;
  }
}

void serial_init(unsigned long rate){
  Serial.begin(rate);
  delay(3000);
  Serial.println("\n\nStarting...");
}

void WiFi_init(const char * ssid, const char * pass){
  WiFi.mode(WIFI_MODE_STA); 
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WiFi_SSID, WiFi_PASS);

  Serial.println("connecting to wifi...");
  WiFi.onEvent(Wifi_onEvent);

  while(!WiFi.isConnected()){
    delay(500);
  }
}

void Wifi_onEvent(arduino_event_id_t event) {
  switch (event) {
    // case SYSTEM_EVENT_STA_GOT_IP:
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("Connected to Wifi and got ip: ");
      Serial.println(WiFi.localIP());
      break;
    // case SYSTEM_EVENT_STA_DISCONNECTED:
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("Wifi Disconnected");
      break;
    default:

        break;
  }
}