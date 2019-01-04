#include "SliderTimer.h" // include the declaration for this class

SliderTimer::SliderTimer() {
  _travelDirection = NO_DIR;
  _currentTask = "none";
  _lastTaskTime = 0;
  _framesShot = 0;
}

void SliderTimer::begin(SPIEEPROM &eeprom) {
  _eeprom = &eeprom;

  pinMode(PIN_SHUTTER_RELEASE, OUTPUT);
  digitalWrite(PIN_SHUTTER_RELEASE, LOW);

  _stepperSpeed = SLIDER_MAX_SPEED; // steps/second

  //read from eeprom
  _stopTime = _eeprom->read32bits(EEPROM_ADDR_STOPTIME);
  _shutterCount = _eeprom->read32bits(EEPROM_ADDR_SHUTTERCOUNT);
  floatint sliderTravelFloatint;
  sliderTravelFloatint.i = _eeprom->read32bits(EEPROM_ADDR_SLIDER_TRAVEL);
  _sliderTravel = sliderTravelFloatint.f;

  //if nothing in eeprom, set to default
  if (_stopTime == 0xFFFFFFFF)
    _stopTime = DEFAULT_STOPTIME;
  if (_shutterCount == 0xFFFFFFFF)
    _shutterCount = DEFAULT_SHUTTERCOUNT;
  if (sliderTravelFloatint.i == 0xFFFFFFFF)
    _sliderTravel = DEFAULT_SLIDER_TRAVEL;

}

void SliderTimer::runBackgroundTasks() {

  if (_currentTask == "waiting") {

    //If wait is not over, do nothing. If wait is over, then do stuff.
    if (millis() - _lastTaskTime >= _stopTime) {

//      unreleaseShutter();

      //check if all frames have been shot
      if(_framesShot >= _shutterCount) {
        stopTimelapse();
      }
      else {
        stepperDriver.move(_travelDirection, SLIDER_STEP_LENGTH * _sliderTravel / _shutterCount);

        _currentTask = "moving";
        _lastTaskTime = millis();
      }
      
    }
  }
  
  if (_currentTask == "moving") {

    //If stepper is still moving, do nothing. If stepper is stopped, then do stuff.
    if (stepperDriver.busyCheck() == 0) {

      releaseShutter();
      _currentTask = "waiting";
      _lastTaskTime = millis();
    }
  }

//  else if (_currentTask == "stabilizing") {
//
//    //If wait is not over, do nothing. If wait is over, then do stuff.
//    if (millis() - _lastTaskTime >= STABILIZE_TIME) {
//
//      releaseShutter();
//      
//      _currentTask = "waiting";
//      _lastTaskTime = millis();
//    }
//  }
  
}

void SliderTimer::startLeftTimelapse() {

  _travelDirection = LEFT;
  _currentTask = "moving";
  _lastTaskTime = millis();
  _framesShot = 0;
}

void SliderTimer::startRightTimelapse() {

  _travelDirection = RIGHT;
  _currentTask = "moving";
  _lastTaskTime = millis();
  _framesShot = 0;
}

void SliderTimer::stopTimelapse() {

  stepperDriver.softStop();
//  unreleaseShutter();
  _travelDirection = NO_DIR;
  _currentTask = "none";
}

void SliderTimer::setStopTime(int stopTime) {

  if (stopTime >= STOP_TIME_MIN && stopTime <= STOP_TIME_MAX) { // check that the new stopTime is within limits
    _stopTime = stopTime;
  }
}

void SliderTimer::setShutterCount(int shutterCount) {

  if (shutterCount >= 1 && shutterCount <= 50000) { // check that the new shutter count is within limits
    _shutterCount = shutterCount;
  }
}

void SliderTimer::setSliderTravel(float sliderTravel) {

  if (sliderTravel >= 0 && sliderTravel <= 1) {
    _sliderTravel = sliderTravel;
  }
}

int SliderTimer::getStopTime() {
  return _stopTime;
}

int SliderTimer::getShutterCount() {
  return _shutterCount;
}

float SliderTimer::getSliderTravel() {
  return _sliderTravel;
}

String SliderTimer::getStopTimeString() {
  return millisecondsToString(_stopTime);
}

String SliderTimer::getTimelapseTimeString() {
  uint32_t totalMoveTime = SLIDER_STEP_LENGTH * _sliderTravel / SLIDER_MAX_SPEED;
  uint32_t totalStopTime = (_stopTime+300+100) * _shutterCount / 1000;

  return secondsToString(totalMoveTime + totalStopTime);
}

String SliderTimer::getFootageTimeString() {
  return millisecondsToString(_shutterCount / (FOOTAGE_FPS / 1000.0));
}

String SliderTimer::getShutterCountString() {
  String str = "";
  str += _shutterCount;
  str += "frames";
  return str;
}

String SliderTimer::getSliderTravelString() {
  String str = "";
  str += round(_sliderTravel * 1000) / 10;
  str += "%";
  return str;
}

String SliderTimer::getFramesShotString() {
  String str = "";
  str += _framesShot;
  str += "/";
  str += _shutterCount;
  str += "frames";
  return str;
}

String SliderTimer::getDirectionString() {
  String str = "";
  switch (_travelDirection) {
    case LEFT   : str += "Left";
      break;
    case RIGHT  : str += "Right";
      break;
    case NO_DIR : str += "Stopped";
      break;
  }
  return str;
}

String SliderTimer::secondsToString(uint32_t seconds) {
  String str = "";

  if (hour(seconds)) {
    str += hour(seconds);
    str += "h ";
  }

  if (minute(seconds)) {
    str += minute(seconds);
    str += "m ";
  }

  str += second(seconds);
  str += "s";

  return str;
}

String SliderTimer::millisecondsToString(uint32_t milliseconds) {
  byte firstDecimalPoint = (milliseconds % 1000) / 100; // get the first digit of the remainder
  int seconds = milliseconds / 1000;

  String str = "";

  if (hour(seconds)) {
    str += hour(seconds);
    str += "h ";
  }

  if (minute(seconds)) {
    str += minute(seconds);
    str += "m ";
  }

  str += second(seconds);
  str += ".";
  str += firstDecimalPoint;
  str += "s";

  return str;
}

void SliderTimer::saveToEEPROM() {

  _eeprom->write32bits(EEPROM_ADDR_STOPTIME, _stopTime);
  _eeprom->write32bits(EEPROM_ADDR_SHUTTERCOUNT, _shutterCount);

  floatint sliderTravelFloatint;
  sliderTravelFloatint.f = _sliderTravel;
  _eeprom->write32bits(EEPROM_ADDR_SLIDER_TRAVEL, sliderTravelFloatint.i);
}

void SliderTimer::releaseShutter() {

  delay(300); //let slider stabilize
  
  digitalWrite(PIN_SHUTTER_RELEASE, HIGH);
  delay(100);
  digitalWrite(PIN_SHUTTER_RELEASE, LOW);
  
  _framesShot++;

  Serial.println("RELEASE SHUTTER");
}

void SliderTimer::unreleaseShutter() {

  digitalWrite(PIN_SHUTTER_RELEASE, LOW);
}



