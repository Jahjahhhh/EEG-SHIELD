#include <SPI.h>
#include "definitions.h"

SPIClass hspi(HSPI);

#define SPI_CLK 500000 // 500 kHz

void serial_init(long rate) {
  Serial.begin(rate);
}

void hspi_init() {
  hspi.begin(H_SCLK_PIN, H_MISO_PIN, H_MOSI_PIN, H_CS_PIN);
}

void pin_init() {
  pinMode(H_CS_PIN, OUTPUT);
  digitalWrite(H_CS_PIN, HIGH);

  pinMode(ADS_PWDN_PIN, OUTPUT);
  digitalWrite(ADS_PWDN_PIN, HIGH);

  pinMode(ADS_RESET_PIN, OUTPUT);
  digitalWrite(ADS_RESET_PIN, HIGH);

  pinMode(START_PIN, OUTPUT);
  digitalWrite(START_PIN, LOW);

  pinMode(DRDY_PIN, INPUT);

  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  
//  pinMode(LED2_PIN, OUTPUT);
//  digitalWrite(LED2_PIN, LOW);
  
//  pinMode(LED3_PIN, OUTPUT);
//  digitalWrite(LED3_PIN, LOW);
  
  pinMode(LED4_PIN, OUTPUT);
  digitalWrite(LED4_PIN, LOW);
  
  pinMode(LED5_PIN, OUTPUT);
  digitalWrite(LED5_PIN, LOW);
  
  pinMode(LED6_PIN, OUTPUT);
  digitalWrite(LED6_PIN, LOW);
  
  pinMode(LED7_PIN, OUTPUT);
  digitalWrite(LED7_PIN, LOW);
  
  pinMode(LED8_PIN, OUTPUT);
  digitalWrite(LED8_PIN, LOW);
}

void reset_reg_values() {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(RESET_CMD);
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
  delay(100);
}

uint8_t read_reg(uint8_t REG) {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(SDATAC_CMD);          // Gotta stop readings to r/w registers
  hspi.transfer(RREG | REG);          // RREG command with address 0x0
  hspi.transfer(0);                   // Number of register to read + 1
  uint8_t content = hspi.transfer(0); // Dummy byte to clock in response
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
  delayMicroseconds(3);

  return content;
}

void write_reg(uint8_t REG, uint8_t BYTE) {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(SDATAC_CMD);          // Gotta stop readings to r/w registers
  hspi.transfer(WREG | REG);          // RREG command with address 0x0
  hspi.transfer(0);                   // Number of register to write + 1
  hspi.transfer(BYTE);                // Data to send
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
  delayMicroseconds(3);
}

uint8_t get_chip_id() {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(SDATAC_CMD);
  hspi.transfer(RREG | ID_ADDR); 
  hspi.transfer(0);              
  uint8_t id = hspi.transfer(0); 
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);

  return id;
}

void send_RDATAC() {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(RDATAC_CMD);
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
}
