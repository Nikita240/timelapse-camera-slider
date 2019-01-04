#include "SPIEEPROM.h"

SPIEEPROM::SPIEEPROM(byte CS_pin, SPISettings &settings) {
  _CS_pin = CS_pin;
  _settings = &settings;
}

void SPIEEPROM::begin() {
  SPI.begin();
  pinMode(_CS_pin, OUTPUT);
  unselectDevice();
}

//set write enable latch
void SPIEEPROM::enableWrite() {
  
  SPI.beginTransaction(*_settings);
  selectDevice();
  
  SPI.transfer(EEPROM_INSTRUCTION_WREN); //send enable write latch instruction

  SPIEEPROM::unselectDevice(); //unselect SPI device
  SPI.endTransaction(); 
}

uint8_t SPIEEPROM::read8bits(uint16_t addr) {
  
  SPI.beginTransaction(*_settings);
  selectDevice();
  
  SPI.transfer(EEPROM_INSTRUCTION_READ); //send begin read instruction
  SPI.transfer16(addr); //send memory address to be read
  uint8_t recievedData = SPI.transfer(0x00); //clock out 8 bits and read data

  SPIEEPROM::unselectDevice(); //unselect SPI device
  SPI.endTransaction();

  return recievedData;
}

uint16_t SPIEEPROM::read16bits(uint16_t addr) {
  
  SPI.beginTransaction(*_settings);
  selectDevice();
  
  SPI.transfer(EEPROM_INSTRUCTION_READ); //send begin read instruction
  SPI.transfer16(addr); //send memory address to be read
  uint16_t recievedData = SPI.transfer16(0x0000); //clock out 16 bits and read data

  SPIEEPROM::unselectDevice(); //unselect SPI device
  SPI.endTransaction();

  return recievedData;
}

uint32_t SPIEEPROM::read32bits(uint16_t addr) {
  
  SPI.beginTransaction(*_settings);
  selectDevice();
  
  SPI.transfer(EEPROM_INSTRUCTION_READ); //send begin read instruction
  SPI.transfer16(addr); //send memory address to be read
  uint32_t recievedData = SPI.transfer16(0x0000) << 16; //clock out 16 bits, read MSB data and shift it to beginning of 32 bit varaible to set the first half
  recievedData = recievedData | SPI.transfer16(0x0000); //clock out 16 bits, read LSB data and OR it to beginning of 32 bit varaible to set the second half

  SPIEEPROM::unselectDevice(); //unselect SPI device
  SPI.endTransaction();

  return recievedData;
}

void SPIEEPROM::write8bits(uint16_t addr, uint8_t data) {

  enableWrite();
  
  SPI.beginTransaction(*_settings);
  selectDevice();
  
  SPI.transfer(EEPROM_INSTRUCTION_WRITE); //send begin write instruction
  SPI.transfer16(addr); //send memory address to be wrote
  SPI.transfer(data); //clock out 8 bits of data

  SPIEEPROM::unselectDevice(); //unselect SPI device
  SPI.endTransaction();
  
  waitForEEPROM();
}

void SPIEEPROM::write16bits(uint16_t addr, uint16_t data) {

  enableWrite();
  
  SPI.beginTransaction(*_settings);
  selectDevice();
  
  SPI.transfer(EEPROM_INSTRUCTION_WRITE); //send begin write instruction
  SPI.transfer16(addr); //send memory address to be wrote
  SPI.transfer16(data); //clock out 16 bits of data
  

  SPIEEPROM::unselectDevice(); //unselect SPI device
  SPI.endTransaction();

  waitForEEPROM();
}

void SPIEEPROM::write32bits(uint16_t addr, uint32_t data) {

  Serial.println(data);

  enableWrite();
  
  SPI.beginTransaction(*_settings);
  selectDevice();
  
  SPI.transfer(EEPROM_INSTRUCTION_WRITE); //send begin write instruction
  SPI.transfer16(addr); //send memory address to be wrote
  SPI.transfer16(data >> 16); //clock out first 16 bits of data. Take the 32 bytes, and shift the right half out of existance to get the MSB 16 bits
  SPI.transfer16(data & 0x0000FFFF); //clock out last 16 bits of data. Take the 32 bytes, and AND the first 16 bits with zero so that you're only left with the LSB 16 bits

  SPIEEPROM::unselectDevice(); //unselect SPI device
  SPI.endTransaction();

  waitForEEPROM();
}

bool SPIEEPROM::isEEPROMBusy() {
  
  SPI.beginTransaction(*_settings);
  selectDevice();
  
  SPI.transfer(EEPROM_INSTRUCTION_RDSR); //send read status register instruction
  bool isBusy = EEPROM_WIP_REGISTER_BIT & SPI.transfer(0x00); //clock out 8 bits of data, then AND against the Write-In-Process register bit to get a bit that represents busy status.

  SPIEEPROM::unselectDevice(); //unselect SPI device
  SPI.endTransaction();

  return isBusy;
}

void SPIEEPROM::waitForEEPROM() {

  while(isEEPROMBusy()) {
    delay(1);
  }
}

void SPIEEPROM::unselectDevice() {
  digitalWrite(_CS_pin, HIGH); //unselect SPI device
}

void SPIEEPROM::selectDevice() {
  digitalWrite(_CS_pin, LOW); //select SPI device
}
