#ifndef SPIEEPROM_h
#define SPIEEPROM_h

#include <Arduino.h>
#include <SPI.h>

#define EEPROM_INSTRUCTION_WREN  0b00000110 //Set the write enable latch (enable write operations)
#define EEPROM_INSTRUCTION_WRDI  0b00000100 //Reset the write enable latch (disable write operations)
#define EEPROM_INSTRUCTION_RDSR  0b00000101 //Read STATUS register
#define EEPROM_INSTRUCTION_WRSR  0b00000101 //Write STATUS register 
#define EEPROM_INSTRUCTION_READ  0b00000011 //Read data from memory array beginning at selected address
#define EEPROM_INSTRUCTION_WRITE 0b00000010 //Write data to memory array beginning at selected address

#define EEPROM_WIP_REGISTER_BIT  0b00000001 //Which bit of the status register is for WIP (Write-In-Process)

class SPIEEPROM {
  public:
    SPIEEPROM(byte CS_pin, SPISettings &settings);
    void begin();
    uint8_t read8bits(uint16_t addr);
    uint16_t read16bits(uint16_t addr);
    uint32_t read32bits(uint16_t addr);
    void write8bits(uint16_t addr, uint8_t data);
    void write16bits(uint16_t addr, uint16_t data);
    void write32bits(uint16_t addr, uint32_t data);

  private:
    byte _CS_pin;
    SPISettings* _settings;

    void selectDevice();
    void unselectDevice();
    void enableWrite();
    bool isEEPROMBusy();
    void waitForEEPROM();
};

#endif
