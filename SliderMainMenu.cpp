#include "SliderMainMenu.h"

SliderMainMenu::SliderMainMenu() : SliderMenu() {
  _menuItemsSize = 3; // this is because C++ is too retarded to let you do sizeof() on an array pointer
  _menuItems = new SliderMenuItem[_menuItemsSize];
}

void SliderMainMenu::begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom) {

  _lcd = &lcd;
  _eeprom = &eeprom;

  _lcd->fillScreen(ILI9341_BLACK); 
  
  _menuItems[_currentSelection].select(); // set selected menu Item before it's draw so it's draw in the selected color
  
  _menuItems[0].begin(lcd, "Video Slider", 0, 0, 130, 0, true);
  _menuItems[1].begin(lcd, "Timelapse Slider", 0, 30, 130, 30, true);
  _menuItems[2].begin(lcd, "Settings", 0, 60, 130, 60, true);
}

SliderMenu* SliderMainMenu::buttonAction(char action, bool isHeld) {
  
  if(action == 'D') {
    return buttonActionDown();
  }
  else if(action == 'U') {
    return buttonActionUp();
  }
  else if(action == 'K') {
    return buttonActionOk();
  }
}

SliderMenu* SliderMainMenu::buttonActionOk() {

  if(_currentSelection == 1) { //OK pressed on timelapse menu item
    SliderMenu* newMenu = new SliderTimelapseMenu();
    newMenu->begin(*_lcd, *_eeprom);
    return newMenu;
  }
  else if(_currentSelection == 2) {  //OK pressed on settings menu item
    SliderMenu* newMenu = new SliderSettingsMenu();
    newMenu->begin(*_lcd, *_eeprom);
    return newMenu;
  }

  return this;
}




