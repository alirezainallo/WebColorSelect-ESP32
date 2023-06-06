#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include "main.h"

bool changeColorFlag = false;
String newColorValue = "";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
/*
// Stores LED state
String ledState;

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}
*/
void webServer_init(void){
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int paramsNr = request->params();
    Serial.println(paramsNr);

    if(paramsNr){
      AsyncWebParameter* p = request->getParam(0);
      Serial.print("Param name: ");
      Serial.println(p->name());
      
      Serial.print("Param value: ");
      Serial.println(p->value());
      newColorValue = p->value();
      changeColorFlag = true;
      Serial.println("------");
    }
    request->send(SPIFFS, "/index.html", String(), false);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello, world");
  });
  /*
  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  */
  
  // Start server
  server.begin();
  Serial.println("server.begin();");
}


void webServer_loop(void){
  if(changeColorFlag){
    changeColorFlag = false;
    //color is ready in newColorValue in string type //like that #009900
  }
}

void spiffs_init(void);

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
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable   detector
  serial_init(serialRate);
  spiffs_init();
  WiFi_init(WiFi_SSID, WiFi_PASS);
  webServer_init();

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
  // delay(3000);
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

void spiffs_init(void){
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    while(true)delay(500);
  }
  Serial.println("SPIFFS mount Ok");

  /*
  File tmp = SPIFFS.open("/hello.txt", "r+", true);
  if(!tmp){
    Serial.println("if(tmp), Fail");
  }
  else{
    Serial.println("if(tmp), Ok");
  }
  */
}