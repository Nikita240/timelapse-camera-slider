#ifndef SliderTimelapseMenu_h
#define SliderTimelapseMenu_h

#include <Arduino.h>
#include "SliderMenu.h"
#include "SliderTimer.h"
#include "SliderMenuItem.h"
#include "SliderMainMenu.h"
#include "SliderTimelapseActiveMenu.h"
#include "SPIEEPROM.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class SliderTimelapseMenu : public SliderMenu {
  public:
    SliderTimelapseMenu();
    void begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom);
    SliderMenu* buttonAction(char action, bool isHeld);
    void saveToEEPROM();
    
  private:
    SliderTimer _timer;

  protected:
    Adafruit_ILI9341* _lcd;
    SPIEEPROM* _eeprom;
    SliderMenu* buttonActionOk();
    SliderMenu* buttonActionBack();
    SliderMenu* buttonActionLeft(bool isHeld);
    SliderMenu* buttonActionRight(bool isHeld);
    SliderMenu* buttonActionDown();
    SliderMenu* buttonActionUp();
    
    void render();
};

#endif
