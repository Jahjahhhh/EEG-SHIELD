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

uint8_t read_reg(uint8_t REG) {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(SDATAC_CMD);          // Gotta stop readings to r/w registers
  hspi.transfer(RREG | REG);          // RREG command with address 0x0
  hspi.transfer(0);                   // Number of register to read + 1
  uint8_t content = hspi.transfer(0); // Dummy byte to clock in response

  digitalWrite(H_CS_PIN, HIGH);

  return content;
}

void write_reg(uint8_t REG, uint8_t BYTE) {
  digitalWrite(H_CS_PIN, LOW);

  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(SDATAC_CMD);          // Gotta stop readings to r/w registers
  hspi.transfer(RREG | REG);          // RREG command with address 0x0
  hspi.transfer(0);                   // Number of register to read + 1
  hspi.transfer(BYTE);                // Data to send
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
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

void CONFIG1() {                                                      // CONFIG1 register current configuration
  digitalWrite(H_CS_PIN, LOW);                                        // Multiple readback mode (no daisy chaining)
                                                                      // Oscillator clock output disabled
  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));   // Output data rate = 1 kSPS
  hspi.transfer(WREG | CONFIG1_ADDR); // WREG cmd with addr 0x1
  hspi.transfer(0); // Number of registers to read + 1
  hspi.transfer(0b11010100); // Data to send
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
}

void CONFIG2() {                                                      // CONFIG2 register current configuration
  digitalWrite(H_CS_PIN, LOW);                                        // Test signals generated internally
                                                                      // Test signal amplitude: 1*-(VREFP -VREFN) / 2400
  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));   // Test signal frequency: Pulsed at FCLK/2^21
  hspi.transfer(CONFIG2_ADDR);
  hspi.transfer(0b11010001);
  hspi.endTransaction();

  digitalWrite(H_CS_PIN, HIGH);
}

void CONFIG3() {                                                      // CONFIG3 register current configuration (datasheets says send 0b11100000, p.62)
  digitalWrite(H_CS_PIN, LOW);                                        // Enable internal reference buffer
  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));   // BIAS_IN Signal is routed to the channel that has MUX_Setting 010
  hspi.transfer(CONFIG3_ADDR);                                        // BIASREF signal (AVDD + AVSS) / 2 generated internally
  hspi.transfer(0b11111110);                                          // BIAS buffer is enabled
  hspi.endTransaction();                                              // BIAS sense is enabled
                                                                      // BIAS is connected
  digitalWrite(H_CS_PIN, HIGH);
}
