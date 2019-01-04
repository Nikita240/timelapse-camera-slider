#ifndef SliderMenuItem_h
#define SliderMenuItem_h

#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define LABEL_COLOR ILI9341_WHITE
#define LABEL_SIZE 2
#define DATA_COLOR ILI9341_WHITE
#define DATA_SIZE 2
#define SELECTED_LABEL_COLOR ILI9341_BLUE
#define SELECTED_DATA_COLOR ILI9341_PINK
#define UNSELECTABLE_COLOR ILI9341_DARKGREY

class SliderMenuItem{
  public:
    SliderMenuItem();
    void begin(Adafruit_ILI9341 &lcd, String label, int labelX, int labelY, int dataX, int dataY, bool selectable);
    void drawData(String data);
    void drawLabel();
    void select();
    void unselect();

  private:
    Adafruit_ILI9341* _lcd;
    String _label;
    int _labelY, _labelX, _dataX, _dataY;
    bool _selected;
    bool _selectable;
};

#endif
