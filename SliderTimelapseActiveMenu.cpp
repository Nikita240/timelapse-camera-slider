#include "SliderTimelapseActiveMenu.h"

SliderTimelapseActiveMenu::SliderTimelapseActiveMenu(SliderTimer &timer) : SliderMenu() {

  _timer = &timer;
  
  _currentSelection = 2;
  
  _menuItemsSize = 3; // this is because C++ is too retarded to let you do sizeof() on an array pointer
  _menuItems = new SliderMenuItem[_menuItemsSize];
}

void SliderTimelapseActiveMenu::begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom) {
  
  _lcd = &lcd;
  _eeprom = &eeprom;

  _lcd->fillScreen(ILI9341_BLACK); 

  _menuItems[_currentSelection].select(); // set selected menu Item before it's draw so it's draw in the selected color  
  _menuItems[0].begin(lcd, "Timelapse:",     0,  0,  130,   0, false);
  _menuItems[1].begin(lcd, "Frames:",        0, 30,  130,  30, false);
  _menuItems[2].begin(lcd, "Status:",        0, 60,  130,  60, true);
  render();
}

void SliderTimelapseActiveMenu::render() {

  for(byte i=0; i<_menuItemsSize; i++) {
    _menuItems[i].drawLabel();
  }
   
  _menuItems[0].drawData(_timer->getTimelapseTimeString());
  _menuItems[1].drawData(_timer->getFramesShotString());
  _menuItems[2].drawData(_timer->getDirectionString());
}

void SliderTimelapseActiveMenu::runBackgroundTasks() {

  _timer->runBackgroundTasks();
  render();
}

SliderMenu* SliderTimelapseActiveMenu::buttonAction(char action, bool isHeld) {
  
  if(action == 'D') {
    return buttonActionDown();
  }
  else if(action == 'U') {
    return buttonActionUp();
  }
  else if(action == 'L') {
    return buttonActionLeft();
  }
  else if(action == 'R') {
    return buttonActionRight();
  }
  else if(action == 'K') {
    return buttonActionOk();
  }
  else if(action == 'B') {
    return buttonActionBack();
  }
}

SliderMenu* SliderTimelapseActiveMenu::buttonActionDown() {
  
  if(_currentSelection+1 < _menuItemsSize) {
    selectMenuItem(_currentSelection+1);
  }

  render();

  return this;
}

SliderMenu* SliderTimelapseActiveMenu::buttonActionUp() {
  
  if(_currentSelection-1 >= 2) {
    selectMenuItem(_currentSelection-1);
  }

  render();

  return this;
}

SliderMenu* SliderTimelapseActiveMenu::buttonActionOk() {

  _timer->stopTimelapse();
  
  return this;
}

SliderMenu* SliderTimelapseActiveMenu::buttonActionBack() {

  _timer->stopTimelapse();
  
  SliderMenu* newMenu = new SliderTimelapseMenu();
  newMenu->begin(*_lcd, *_eeprom);
  return newMenu;
}

SliderMenu* SliderTimelapseActiveMenu::buttonActionLeft() {

  _timer->startLeftTimelapse();

  return this;
}

SliderMenu* SliderTimelapseActiveMenu::buttonActionRight() {

  _timer->startRightTimelapse();

  return this;
}

