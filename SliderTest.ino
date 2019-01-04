#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SparkFunAutoDriver.h>
#include "StepperDriverDef.h"

//#include "SliderMenu.h"
//#include "SliderTimelapseMenu.h"
#include "SliderMainMenu.h"
#include "SPIEEPROM.h"

#if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined (__AVR_ATmega328P__)
  #define STMPE_CS 6
  #define TFT_CS   9
  #define TFT_DC   10
  #define SD_CS    5
#endif

#define PIN_LEFT  16
#define PIN_RIGHT 13
#define PIN_UP    11
#define PIN_DOWN  15
#define PIN_OK    12
#define PIN_BACK  5
#define PIN_EEPROM_CS 19
#define PIN_STEPPER_DRIVER_CS    A0
#define PIN_STEPPER_DRIVER_RESET A4
#define PIN_STEPPER_DRIVER_BUSY  0

#define INT_MAX 2147483647
#define BUTTON_HOLD_THRESHOLD 500 // in milliseconds
#define BUTTON_DOUBLE_CLICK_THRESHOLD 50 // in milliseconds

SPISettings spisettings = SPISettings(5000000, MSBFIRST, SPI_MODE0);
SPIEEPROM eeprom = SPIEEPROM(PIN_EEPROM_CS, spisettings); //5Mhz is maximum speed at 3.3v

Adafruit_ILI9341 lcd = Adafruit_ILI9341(TFT_CS, TFT_DC);
SliderMenu* menu = new SliderMainMenu();

AutoDriver stepperDriver = AutoDriver(0, PIN_STEPPER_DRIVER_CS, PIN_STEPPER_DRIVER_RESET, PIN_STEPPER_DRIVER_BUSY);

char action = '\0'; // used to determine which button was pressed
int buttonPressTime = 0; // used to time how long a button has been held
bool needsSave = false; // used to determine if settings need to be saved to EEPROM


void setup() {
  Serial.begin(9600);

  delay(3000); //let serial start for debug
  
  Serial.println("Serial test");

  eeprom.begin();

  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_OK, INPUT_PULLUP);
  pinMode(PIN_BACK, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_LEFT),   ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_RIGHT),  ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_UP),     ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_DOWN),   ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_OK),     ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BACK),   ISR, FALLING);

  pinMode(PIN_STEPPER_DRIVER_CS, OUTPUT);
  pinMode(PIN_STEPPER_DRIVER_RESET, OUTPUT);

  digitalWrite(PIN_STEPPER_DRIVER_CS, HIGH);
  digitalWrite(PIN_STEPPER_DRIVER_RESET, LOW);
  digitalWrite(PIN_STEPPER_DRIVER_RESET, HIGH);// This low/high is a reset of the L6470 chip on the
                                               //  Autodriver board, and is a good thing to do at
                                               //  the start of any Autodriver sketch, to be sure
                                               //  you're starting the Autodriver from a known state.

  stepperDriverConfig();
  
  lcd.begin(); // start lcd
  lcd.setRotation(3); // rotate LCD to make it horizontal

  menu->begin(lcd, eeprom);

}

void loop() {

  if(action != '\0') { //Fire action once
    
    if(action == 'L' || action == 'R') {
      needsSave = true;  
    }
    
    menu = menu->buttonAction(action, false);     
    action = '\0';
  }
  
  if(millis()-buttonPressTime > BUTTON_HOLD_THRESHOLD) { // if either button is being held down
    
    if(!digitalRead(PIN_LEFT)) { 
      menu = menu->buttonAction('L', true); 
      needsSave = true;
    }
    else if(!digitalRead(PIN_RIGHT)) { 
      menu = menu->buttonAction('R', true); 
      needsSave = true;
    }
  }
  else if(needsSave) { // putting the save in the else here ensures that EEPROM write is not triggered when a button is HELD down
    menu->saveToEEPROM();
    needsSave = false;
  }

  menu->runBackgroundTasks();
}

void ISR() {
  
  //find which pin is low
  if(!digitalRead(PIN_LEFT)) {
    action = 'L';
  }
  else if(!digitalRead(PIN_RIGHT)) {
    action = 'R';
  }
  else if(!digitalRead(PIN_UP)) {
    action = 'U';
  }
  else if(!digitalRead(PIN_DOWN)) {
    action = 'D';
  }
  else if(!digitalRead(PIN_OK)) {
    action = 'K';   
  }
  else if(!digitalRead(PIN_BACK)) {
    action = 'B';
  }
  
  if(millis()-buttonPressTime < BUTTON_DOUBLE_CLICK_THRESHOLD) { //check that it's not a double click
    action = '\0';
  }
  else {
    buttonPressTime = millis();
  }
}

