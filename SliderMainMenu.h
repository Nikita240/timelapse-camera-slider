#ifndef SliderMainMenu_h
#define SliderMainMenu_h

#include <Arduino.h>
#include "SliderMenu.h"
#include "SliderTimelapseMenu.h"
#include "SliderSettingsMenu.h"
#include "SliderMenuItem.h"
#include "SPIEEPROM.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class SliderMainMenu : public SliderMenu {
  public:
    SliderMainMenu();
    void begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom);
    SliderMenu* buttonAction(char action, bool isHeld);
    
  private:

  protected:
    Adafruit_ILI9341* _lcd;
    SPIEEPROM* _eeprom;

    SliderMenu* buttonActionOk();
};

#endif
