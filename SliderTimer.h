#ifndef SliderTimer_h
#define SliderTimer_h

#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>

#include "SPIEEPROM.h"

#include "StepperDriverDef.h"

#define FOOTAGE_FPS 24
#define DEFAULT_SHUTTERCOUNT 1000
#define DEFAULT_STOPTIME 4000 //milliseconds
#define DEFAULT_SLIDER_TRAVEL 0.9

#define STABILIZE_TIME 50 //milliseconds

#define STOP_TIME_MIN 100
#define STOP_TIME_MAX 60000

#define EEPROM_ADDR_SHUTTERCOUNT 32
#define EEPROM_ADDR_STOPTIME 36 //addresses incremented by 4 bytes because Cortex MCU's have 32 bit integers
#define EEPROM_ADDR_SLIDER_TRAVEL 40

#define PIN_SHUTTER_RELEASE 17
//#define PIN_SHUTTER_RELEASE 1

union floatint {
  float f;
  int i;
};

class SliderTimer {
  public:
  
    SliderTimer();
    void begin(SPIEEPROM &eeprom);
    
    
    String getTimelapseTimeString();
    String getFootageTimeString();
    String getFramesShotString();
    String getDirectionString();
    
    int getStopTime();
    void setStopTime(int stopTime);
    String getStopTimeString();
    
    int getShutterCount();
    void setShutterCount(int shutterCount);
    String getShutterCountString();
    
    float getSliderTravel();
    void setSliderTravel(float sliderTravel);
    String getSliderTravelString();
    
    void saveToEEPROM();

    void runBackgroundTasks();
    void startLeftTimelapse();
    void startRightTimelapse();
    void stopTimelapse();
    
  private:
    SPIEEPROM* _eeprom;
    unsigned int _stopTime; // miliseconds 
    unsigned int _shutterCount;
    unsigned int _stepperSpeed; // steps/second
    unsigned int _stepsBetweenShutters; 
    float _sliderTravel; // percentage of slider length that's allowed to be traversed
    byte _travelDirection;
    String _currentTask;
    unsigned int _lastTaskTime;
    unsigned int _framesShot;

    String secondsToString(uint32_t seconds);
    String millisecondsToString(uint32_t milliseconds);
    void releaseShutter();
    void unreleaseShutter();
    
};

#endif
