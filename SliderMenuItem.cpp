#include "SliderMenuItem.h"

SliderMenuItem::SliderMenuItem() {
  _selected = false;
}

void SliderMenuItem::begin(Adafruit_ILI9341 &lcd, String label, int labelX, int labelY, int dataX, int dataY, bool selectable) {
  _lcd = &lcd;
  _label = label;
  _labelX = labelX;
  _labelY = labelY;
  _dataX = dataX;
  _dataY = dataY;
  _selectable = selectable;

  drawLabel();
}

void SliderMenuItem::drawLabel() {
  
  _lcd->setCursor(_labelX, _labelY);
  
  if(!_selectable)
    _lcd->setTextColor(UNSELECTABLE_COLOR, ILI9341_BLACK); //black background makes sure the old data gets erased
  else if(_selected)
    _lcd->setTextColor(SELECTED_LABEL_COLOR, ILI9341_BLACK); //black background makes sure the old data gets erased
  else
    _lcd->setTextColor(LABEL_COLOR, ILI9341_BLACK); //black background makes sure the old data gets erased
    
  _lcd->setTextSize(LABEL_SIZE);
  _lcd->print(_label);
}

void SliderMenuItem::drawData(String data) {
  
  _lcd->setCursor(_dataX, _dataY);
  
  if(!_selectable)
    _lcd->setTextColor(UNSELECTABLE_COLOR, ILI9341_BLACK); //black background makes sure the old data gets erased
  else if(_selected)
    _lcd->setTextColor(SELECTED_DATA_COLOR, ILI9341_BLACK); //black background makes sure the old data gets erased
  else
    _lcd->setTextColor(DATA_COLOR, ILI9341_BLACK); //black background makes sure the old data gets erased
    
  _lcd->setTextSize(DATA_SIZE);
  _lcd->setTextWrap(false);
  _lcd->print(data);
  _lcd->print("                    "); //padding makes sure that the background color erases old data if the new data has fewer characters
}

void SliderMenuItem::select() {
  if(_selectable)
    _selected = true;
}

void SliderMenuItem::unselect() {
  _selected = false;
}

