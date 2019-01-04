#include "SliderTimelapseMenu.h"

SliderTimelapseMenu::SliderTimelapseMenu() : SliderMenu(), _timer() {
  
  _currentSelection = 2;
  
  _menuItemsSize = 5; // this is because C++ is too retarded to let you do sizeof() on an array pointer
  _menuItems = new SliderMenuItem[_menuItemsSize];
}

void SliderTimelapseMenu::begin(Adafruit_ILI9341 &lcd, SPIEEPROM &eeprom) {
  
  _lcd = &lcd;
  _eeprom = &eeprom;
  _timer.begin(eeprom);

  _lcd->fillScreen(ILI9341_BLACK); 

  _menuItems[_currentSelection].select(); // set selected menu Item before it's draw so it's draw in the selected color  
  _menuItems[0].begin(lcd, "Timelapse:",     0,  0,  130,   0, false);
  _menuItems[1].begin(lcd, "Footage:",       0, 30,  130,  30, false);
  _menuItems[2].begin(lcd, "Stoptime:",      0, 60,  130,  60, true);
  _menuItems[3].begin(lcd, "Frames:",        0, 90,  130,  90, true);
  _menuItems[4].begin(lcd, "Travel:",        0, 120, 130, 120, true);

  render();
}

void SliderTimelapseMenu::render() {

  for(byte i=0; i<_menuItemsSize; i++) {
    _menuItems[i].drawLabel();
  }
   
  _menuItems[0].drawData(_timer.getTimelapseTimeString());
  _menuItems[1].drawData(_timer.getFootageTimeString());
  _menuItems[2].drawData(_timer.getStopTimeString());
  _menuItems[3].drawData(_timer.getShutterCountString());
  _menuItems[4].drawData(_timer.getSliderTravelString());
}

void SliderTimelapseMenu::saveToEEPROM() {
  _timer.saveToEEPROM();
}

SliderMenu* SliderTimelapseMenu::buttonAction(char action, bool isHeld) {
  
  if(action == 'D') {
    return buttonActionDown();
  }
  else if(action == 'U') {
    return buttonActionUp();
  }
  else if(action == 'L') {
    return buttonActionLeft(isHeld);
  }
  else if(action == 'R') {
    return buttonActionRight(isHeld);
  }
  else if(action == 'K') {
    return buttonActionOk();
  }
  else if(action == 'B') {
    return buttonActionBack();
  }
}

SliderMenu* SliderTimelapseMenu::buttonActionDown() {
  
  if(_currentSelection+1 < _menuItemsSize) {
    selectMenuItem(_currentSelection+1);
  }

  render();

  return this;
}

SliderMenu* SliderTimelapseMenu::buttonActionUp() {
  
  if(_currentSelection-1 >= 2) {
    selectMenuItem(_currentSelection-1);
  }

  render();

  return this;
}

SliderMenu* SliderTimelapseMenu::buttonActionOk() {
  
  SliderMenu* newMenu = new SliderTimelapseActiveMenu(_timer);
  newMenu->begin(*_lcd, *_eeprom);
  return newMenu;
}

SliderMenu* SliderTimelapseMenu::buttonActionBack() {
  
  SliderMenu* newMenu = new SliderMainMenu();
  newMenu->begin(*_lcd, *_eeprom);
  return newMenu;
}

SliderMenu* SliderTimelapseMenu::buttonActionLeft(bool isHeld) {

  if(_currentSelection == 2) { //decrease stopTime by 100ms 

    if(isHeld) {
      _timer.setStopTime(_timer.getStopTime()-500);
    }
    else {
      _timer.setStopTime(_timer.getStopTime()-100);
    }
    render();
  }
  else if(_currentSelection == 3) { //decrease shutterCount by 1

    if(isHeld) {
      _timer.setShutterCount(_timer.getShutterCount()-25);
    }
    else {
      _timer.setShutterCount(_timer.getShutterCount()-1);
    }
    render();
  }
  else if(_currentSelection == 4) { //decrease sliderTravel by 1%

    if(isHeld) {
      _timer.setSliderTravel(_timer.getSliderTravel()-0.10);
    }
    else {
      _timer.setSliderTravel(_timer.getSliderTravel()-0.01);
    }
    render();
  }

  return this;
}

SliderMenu* SliderTimelapseMenu::buttonActionRight(bool isHeld) {

  if(_currentSelection == 2) { //decrease stopTime by 100ms 
    
    if(isHeld) {
      _timer.setStopTime(_timer.getStopTime()+500);
    }
    else {
      _timer.setStopTime(_timer.getStopTime()+100);
    }
    render();
  }
  else if(_currentSelection == 3) { //increase shutterCount by 1

    if(isHeld) {
      _timer.setShutterCount(_timer.getShutterCount()+25);
    }
    else {
      _timer.setShutterCount(_timer.getShutterCount()+1);
    }
    render();
  }
   else if(_currentSelection == 4) { //increase sliderTravel by 1%

    if(isHeld) {
      _timer.setSliderTravel(_timer.getSliderTravel()+0.10);
    }
    else {
      _timer.setSliderTravel(_timer.getSliderTravel()+0.01);
    }
    render();
  }

  return this;
}

