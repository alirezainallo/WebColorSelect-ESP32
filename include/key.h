#ifndef _KEY_H
#define _KEY_H

#include <Arduino.h>



/**
 * @brief Choose your needed trigge mode
 *    you can use more than one of them
 *    with using "|" symbol.
 * 
 */
typedef enum
{
  RISING_EDGE  = 0x01,
  FALLING_EDGE = 0x02,
  HIGH_LEVEL   = 0x04,
  LOW_LEVEL    = 0x08,
}triggeMode;


typedef struct keyHandler_tag keyHandler;

/**
 * @brief use this form for your key callBack.
 * 
 */
typedef std::function<void(keyHandler *hkey, void *userData)> keyCallBack;

struct keyHandler_tag
{
  keyCallBack callBack;
  void * userData;
  uint8_t mode;  //from triggeMode
  uint8_t status;//from triggeMode
  uint8_t pin;
  uint8_t cur_state;
  uint8_t pre_state;
};

/**
 * @brief initialize your key before loop();
 * 
 * @param callBack add callBack for when trigge @param mode occur.
 * @param userData this give you un your @param callBack
 * @param mode use it from triggeMode enum.
 * @param pin your current pin that key is connected to
 * @return keyHandler* save it for keyProccess function
 */
keyHandler *key_init (keyCallBack callBack,
                      void * userData,
                      uint8_t mode,
                      uint8_t pin);

/**
 * @brief call this function in your loop();
 * 
 * @param hkey this is key_init @return keyHandler* that you should have save it
 */
void keyProccess (keyHandler *hkey);


#endif //_KEY_H