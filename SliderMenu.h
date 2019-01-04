#ifndef SliderMenu_h
#define SliderMenu_h

#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "SliderMenuItem.h"
#include "SPIEEPROM.h"

class SliderMenu {
  public:
    SliderMenu();
    virtual void begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom);
    virtual SliderMenu* buttonAction(char action, bool isHeld);
    virtual void saveToEEPROM();
    virtual void runBackgroundTasks();

  protected:
    Adafruit_ILI9341* _lcd;
    SPIEEPROM* _eeprom;
    byte _currentSelection;
    SliderMenuItem *_menuItems;
    byte _menuItemsSize; // this is because C++ is too retarded to let you do sizeof() on an array pointer

    void selectMenuItem(byte menuItemIndex);
    void render();
    SliderMenu* buttonActionDown();
    SliderMenu* buttonActionUp();
};

#endif
