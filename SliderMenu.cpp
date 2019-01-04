#include "SliderMenu.h"

SliderMenu::SliderMenu() {
  _currentSelection = 0;  
}

void SliderMenu::begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom) {
  
  _lcd = &lcd;
  _eeprom = &eeprom;

  _menuItems[_currentSelection].select();
}

void SliderMenu::selectMenuItem(byte menuItemIndex) {
  //start by unselecting all menu items
  for(byte i=0; i<_menuItemsSize; i++) {
    _menuItems[i].unselect();
  }

  //select selected menu item
  _menuItems[menuItemIndex].select();

  _currentSelection = menuItemIndex;
}

void SliderMenu::render() {
  
  for(byte i=0; i<_menuItemsSize; i++) {
    _menuItems[i].drawLabel();
  }
}

void SliderMenu::saveToEEPROM() {
 
}

void SliderMenu::runBackgroundTasks() {
  
}

SliderMenu* SliderMenu::buttonAction(char action, bool isHeld) {
}

SliderMenu* SliderMenu::buttonActionDown() {
  
  if(_currentSelection+1 < _menuItemsSize) {
    selectMenuItem(_currentSelection+1);
  }

  render();

  return this;
}

SliderMenu* SliderMenu::buttonActionUp() {
  
  if(_currentSelection-1 >= 0) {
    selectMenuItem(_currentSelection-1);
  }

  render();

  return this;
}

