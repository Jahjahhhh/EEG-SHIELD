#include <SPI.h>
#include "functions.h"

void setup() {
  pin_init();
  serial_init(115200);
  hspi_init();
  reset_reg_values();
}

void loop() {
  // Print ADS1299-4 ID (revision + model)
  uint8_t id = get_chip_id();
  Serial.println(id);
  Serial.printf("ADS1299-4 ID: 0x%02X\n", id);
  delay(1000);
}
