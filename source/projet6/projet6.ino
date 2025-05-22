#include <SPI.h>
#include "functions.h"
#include "definitions.h"

void setup() {
  pin_init();
  serial_init(115200);
  hspi_init();
  reset_reg_values();
}

void loop() {
  // Print ADS1299-4 ID (revision + model)
  uint8_t id = read_reg(ID_ADDR);
  Serial.printf("ADS1299-4 ID: 0x%02X\n", id);

  write_reg(CONFIG1_ADDR, 0b11010100);
  delay(500);

  uint8_t config1 = read_reg(CONFIG1_ADDR);

  delay(500);

  Serial.println(config1);
  Serial.printf("CONFIG1: 0x%02X\n", config1);

  delay(500);
}
