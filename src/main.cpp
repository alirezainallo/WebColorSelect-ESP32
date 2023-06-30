#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include "main.h"
#include <SPI.h>
// #include <ESP32DMASPIMaster.h>

uint32_t ch2num(char ch);
uint32_t str2num(char * str);
void strHex2colorHex(char * str, uint32_t *r, uint32_t *g, uint32_t *b);
void displayColor(uint32_t r, uint32_t g, uint32_t b);
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
  // /?color=ff00dd
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
      Serial.println("------");
      changeColorFlag = true;
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
    uint32_t r,g,b;
    changeColorFlag = false;

    //color is ready in newColorValue in string type //like that #009900
    Serial.printf("value: %s\n", newColorValue.c_str());
    strHex2colorHex((char*)(newColorValue.c_str()), &r, &g, &b);
    Serial.printf("r: %d, g: %d, b: %d\n", r, g, b);
    displayColor(r, g, b);
  }
}


#define LED_PIN 2
void serial_init(unsigned long rate);
void spiffs_init(void);
uint32_t tick_ms = 500;
void heartBeat_init(uint32_t ms);
void heartBeat_loop(void);
const unsigned long serialRate = 115200;
const char WiFi_SSID[] = "KHOSH1";//Aavan2//Mabna Telecom//DIR-822-7732
const char WiFi_PASS[] = "Tafi@110";//Yasahebazzaman1400//m@bn@2062//Yasahebazzaman1400
void Wifi_onEvent(arduino_event_id_t event);
void WiFi_init(const char * ssid, const char * pass);


#define RGB_ARRAY_LED 12
typedef struct{
  uint32_t start3ones : 3;
  uint32_t brightness : 5;//global
  uint32_t BLUE       : 8;
  uint32_t GREEN      : 8;
  uint32_t RED        : 8;
}rgbFrame_t;
typedef struct{
  uint32_t   startFrame;
  rgbFrame_t LED[RGB_ARRAY_LED];
  uint32_t   stopFrame;
}rgbArray_t;
typedef enum{
  ARJ_0  = 0,
  ARJ_1  = 1,
  ARJ_2  = 2,
  ARJ_3  = 3,
  ARJ_4  = 4,
  ARJ_5  = 5,
  ARJ_6  = 6,
  ARJ_7  = 7,
  ARJ_8  = 8,
  ARJ_9  = 9,
  ARJ_10 = 10,
  ARJ_11 = 11,
  ARJ_12 = 12,
  ARJ_13 = 13,
  ARJ_14 = 14,
  ARJ_15 = 15,
  ARJ_16 = 16,
  ARJ_17 = 17,
  ARJ_18 = 18,
  ARJ_19 = 19,
  ARJ_20 = 20,
  ARJ_21 = 21,
  ARJ_22 = 22,
  ARJ_23 = 23,
  ARJ_24 = 24,
  ARJ_25 = 25,
  ARJ_26 = 26,
  ARJ_27 = 27,
  ARJ_28 = 28,
  ARJ_29 = 29,
  ARJ_30 = 30,
  ARJ_31 = 31
}brightness_t;
typedef enum{
  reserved = 0,
}grayLevel_t;
typedef enum{
  LED_R,
  LED_G,
  LED_B,
}ledColor_t;
static rgbArray_t rgbArray;
static uint32_t rgbArray_ms;
void rgbArray_init(void);
void rgbArray_loop(void);
void rgbFrame_init(rgbFrame_t *Frame, uint32_t r, uint32_t g, uint32_t b);
void rgbArray_feed(rgbArray_t *frame, uint32_t r, uint32_t g, uint32_t b);
void setColor(ledColor_t color, uint32_t value);
void NextColor(ledColor_t *color);
void displayColor(uint32_t r, uint32_t g, uint32_t b);
void spiCheckPin(void){
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);  
}

void setup() {
  // put your setup code here, to run once:
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable   detector
  serial_init(serialRate);
  spiffs_init();
  WiFi_init(WiFi_SSID, WiFi_PASS);
  webServer_init();

  heartBeat_init(600);
  rgbArray_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  webServer_loop();
  heartBeat_loop();
  // rgbArray_loop();
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

void rgbArray_init(void){
  // rgbArray_ms  = ms;
  SPI.begin();
  delay(50);
  rgbArray_feed(&rgbArray, 255, 255, 0);
  SPI.transfer(&rgbArray, sizeof(rgbArray));
}
void rgbArray_loop(void){
  static uint32_t curTick;
  static uint32_t nexTick;
  // static brightness_t brigh = ARJ_0;
  // static uint32_t *pBrigh   = (uint32_t*)&brigh;

  curTick = millis();
  if(nexTick < curTick){
    static ledColor_t color = LED_R;
    static bool op = false;
    static uint32_t value = 0;
    nexTick = curTick + rgbArray_ms;
    // rgbArray_feed(&rgbArray, value, value, value);
    setColor(color, value);
    Serial.printf("value: 0x%02x, 0x%02x, 0x%02x\n", rgbArray.LED[0].RED, rgbArray.LED[0].GREEN, rgbArray.LED[0].BLUE);
    switch(op){
      case true:
        value--;
        if(value == 0){
          op = false;
          value++;
          NextColor(&color);
        }
        break;
      case false:
        value++;
        if(value > 255){
          op = true;
          value--;
        }
        break;
    }
    
    
    SPI.transfer(&rgbArray, sizeof(rgbArray));

    /*
    // if(!value){
    //   value = 0xFF;
    // }
    // else{
    //   value = 0x00;
    // }

    // if(++*pBrigh > ARJ_31){
    //   *pBrigh = ARJ_0;
    // }
    */
  }
}
void rgbFrame_init(rgbFrame_t *Frame, uint32_t r, uint32_t g, uint32_t b){
  Frame->start3ones = 0x7;
  Frame->brightness = 0x1F;//global
  Frame->BLUE       = (uint8_t)b;
  Frame->GREEN      = (uint8_t)g;
  Frame->RED        = (uint8_t)r;
}
void rgbArray_feed(rgbArray_t *frame, uint32_t r, uint32_t g, uint32_t b){
  frame->startFrame = 0x00000000;
  for(int i = 0; i < RGB_ARRAY_LED; i++){
    rgbFrame_init(&frame->LED[i], r, g, b);
  }
  frame->stopFrame  = 0xFFFFFFFF;
}
void NextColor(ledColor_t *color){
  switch(*color){
  case LED_R:
    *color = LED_G;
    break;
  case LED_G:
    *color = LED_B;
    break;
  case LED_B:
    *color = LED_R;
    break;
  
  default:
    break;
  }
}
void setColor(ledColor_t color, uint32_t value){
  switch(color){
  case LED_R:
    rgbArray_feed(&rgbArray, value, 0x00, 0x00);
    break;
  case LED_G:
    rgbArray_feed(&rgbArray, 0x00, value, 0x00);
    break;
  case LED_B:
    rgbArray_feed(&rgbArray, 0x00, 0x00, value);
    break;
  
  default:
    break;
  }
}
void displayColor(uint32_t r, uint32_t g, uint32_t b){
  rgbArray_feed(&rgbArray, r, g, b);
  SPI.transfer(&rgbArray, sizeof(rgbArray));
}

uint32_t ch2num(char ch){
  uint32_t retVal = 0;//if not num in Ascii
  if((ch >= '0') && (ch <= '9')){
    const uint32_t difAsciiDigitToNum = '0';
    retVal = (uint32_t)(ch - difAsciiDigitToNum);
  }
  else if((ch >= 'a') && (ch <= 'f')){
    const int32_t difAsciiSmallToNum = 'a' - 10;
    retVal = (uint32_t)(ch - difAsciiSmallToNum);
  }
  else if((ch >= 'A') && (ch <= 'F')){
    const int32_t difAsciiCapitalToNum = 'A' - 10;
    retVal = (uint32_t)(ch - difAsciiCapitalToNum);
  }
  else{

  }
  // Serial.printf("%c: %d\n", ch, retVal);
  return retVal;
}
uint32_t str2num(char * str){
  uint32_t retVal = 0;
  retVal  = ch2num(str[0]) << 4;
  retVal += ch2num(str[1]);
  return retVal;
}
void strHex2colorHex(char * str, uint32_t *r, uint32_t *g, uint32_t *b){
  *r = str2num(&str[0]);
  *g = str2num(&str[2]);
  *b = str2num(&str[4]);
}