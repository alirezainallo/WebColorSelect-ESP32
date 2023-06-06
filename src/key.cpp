#include "key.h"

keyHandler *key_init (keyCallBack callBack,
  void * userData,
  uint8_t mode,
  uint8_t pin) {

  keyHandler *hkey;
  hkey = (keyHandler*)malloc(sizeof(keyHandler));

  hkey->callBack = callBack;
  hkey->userData = userData;
  hkey->mode     = mode;
  hkey->status   = HIGH_LEVEL;
  hkey->pin      = pin;
  hkey->pre_state = HIGH;
  hkey->cur_state = HIGH;

  pinMode(pin, INPUT_PULLUP);
  
  return hkey;
}
void keyProccess (keyHandler *hkey) {
  hkey->cur_state = digitalRead(hkey->pin);
  if(hkey->cur_state == hkey->pre_state)
  {
    if(hkey->cur_state == HIGH)
    {
      hkey->status = HIGH_LEVEL;
    }
    else
    {
      hkey->status = LOW_LEVEL;
    }
  }
  else
  {
    hkey->pre_state = hkey->cur_state;
    if(hkey->cur_state == HIGH)
    {
      hkey->status = RISING_EDGE;
    }
    else
    {
      hkey->status = FALLING_EDGE;
    }
  }

  if(hkey->mode & hkey->status)
  {
    hkey->callBack(hkey, hkey->userData);
  }
}
