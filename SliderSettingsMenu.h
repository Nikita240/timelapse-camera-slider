#ifndef SliderSettingsMenu_h
#define SliderSettingsMenu_h

#include <Arduino.h>
#include "SliderMenu.h"
#include "SliderMenuItem.h"
#include "SliderMainMenu.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class SliderSettingsMenu : public SliderMenu {
  public:
    SliderSettingsMenu();
    void begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom);
    SliderMenu* buttonAction(char action, bool isHeld);
    
  private:

  protected:
    Adafruit_ILI9341* _lcd;
    SPIEEPROM* _eeprom;

    SliderMenu* buttonActionBack();
};

#endif
