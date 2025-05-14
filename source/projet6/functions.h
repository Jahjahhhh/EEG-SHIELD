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
}

void reset_reg_values() {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(RESET_CMD);
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
  delay(100);
}

uint8_t get_chip_id() {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(SDATAC_CMD);
  hspi.transfer(RREG | ID_ADDR); // RREG command with address 0x00
  hspi.transfer(0);              // Number of register to read + 1
  uint8_t id = hspi.transfer(0); // Dummy byte to clock in response
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);

  return id;
}

void chip_init() {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(CONFIG1_ADDR);
  hspi.transfer(0b11010100); // No daisy mode, instead multiple readback mode, dont output internal osc, set data rate to 1 kSPS
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
}
