#ifndef SliderTimelapseActiveMenu_h
#define SliderTimelapseActiveMenu_h

#include <Arduino.h>
#include "SliderMenu.h"
#include "SliderTimer.h"
#include "SliderMenuItem.h"
#include "SliderMainMenu.h"
#include "SPIEEPROM.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class SliderTimelapseActiveMenu : public SliderMenu {
  public:
    SliderTimelapseActiveMenu(SliderTimer &timer);
    void begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom);
    SliderMenu* buttonAction(char action, bool isHeld);
    void runBackgroundTasks();
    
  private:
    SliderTimer* _timer;

  protected:
    Adafruit_ILI9341* _lcd;
    SPIEEPROM* _eeprom;
    
    SliderMenu* buttonActionOk();
    SliderMenu* buttonActionBack();
    SliderMenu* buttonActionLeft();
    SliderMenu* buttonActionRight();
    SliderMenu* buttonActionDown();
    SliderMenu* buttonActionUp();
    
    void render();
};

#endif
