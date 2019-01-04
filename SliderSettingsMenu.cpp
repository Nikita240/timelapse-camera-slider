#include "SliderSettingsMenu.h"

SliderSettingsMenu::SliderSettingsMenu() : SliderMenu() {
  _menuItemsSize = 1; // this is because C++ is too retarded to let you do sizeof() on an array pointer
  _menuItems = new SliderMenuItem[_menuItemsSize];
}

void SliderSettingsMenu::begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom) {

  _lcd = &lcd;
  _eeprom = &eeprom;

  _lcd->fillScreen(ILI9341_BLACK); 
  
  _menuItems[_currentSelection].select(); // set selected menu Item before it's draw so it's draw in the selected color
  
//  _menuItems[0].begin(lcd, "SEND NUDES", 0, 0, 130, 0, false);

  //temp
  _lcd->setCursor(0, 0);
  
  _lcd->setTextColor(ILI9341_WHITE, ILI9341_BLACK); //black background makes sure the old data gets erased
    
  _lcd->setTextSize(3);
  _lcd->print("SEND NUDES");
}

SliderMenu* SliderSettingsMenu::buttonAction(char action, bool isHeld) {
  
  if(action == 'B') {
    return buttonActionBack();
  }
}

SliderMenu* SliderSettingsMenu::buttonActionBack() {

  SliderMenu* newMenu = new SliderMainMenu();
  newMenu->begin(*_lcd, *_eeprom);
  return newMenu;
}




