#include <SPI.h>
#include "functions.h"
#include "definitions.h"

volatile bool dataReady = false;

void onDRDY() {
  dataReady = true;
}

void setup() {
  pin_init();
  serial_init(115200);
  hspi_init();
  reset_reg_values();

  delay(5000); // this gotta be long enough

  digitalWrite(H_CS_PIN, LOW);
  delayMicroseconds(2);
  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  hspi.transfer(SDATAC_CMD);
  delayMicroseconds(2);
  hspi.transfer(WREG | CONFIG3_ADDR);
  delayMicroseconds(2);
  hspi.transfer(0);
  delayMicroseconds(2);
  hspi.transfer(0b11100000); 
  delayMicroseconds(2);
  hspi.endTransaction();
  digitalWrite(H_CS_PIN, HIGH);
  delayMicroseconds(2);

  digitalWrite(H_CS_PIN, LOW);
  delayMicroseconds(2);
  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  delayMicroseconds(2);
  hspi.transfer(WREG | CONFIG1_ADDR);
  delayMicroseconds(2);
  hspi.transfer(1);
  delayMicroseconds(2);
  hspi.transfer(0b11010100); 
  delayMicroseconds(2);
  hspi.transfer(0b11010000); 
  delayMicroseconds(2);
  hspi.endTransaction();
  digitalWrite(H_CS_PIN, HIGH);
  delayMicroseconds(2);

  digitalWrite(H_CS_PIN, LOW);
  delayMicroseconds(2);
  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  delayMicroseconds(2);
  hspi.transfer(WREG | CH1SET_ADDR);
  delayMicroseconds(2);
  hspi.transfer(3);
  delayMicroseconds(2);
  hspi.transfer(0b00000101);
  delayMicroseconds(2);
  hspi.transfer(0b00000101);
  delayMicroseconds(2);
  hspi.transfer(0b00000101);
  delayMicroseconds(2);
  hspi.transfer(0b00000101);
  delayMicroseconds(2);
  hspi.endTransaction();
  digitalWrite(H_CS_PIN, HIGH);
  delayMicroseconds(2);

//  uint8_t res1 = read_reg(CH1SET_ADDR);
//  uint8_t res2 = read_reg(CH2SET_ADDR);
//  uint8_t res3 = read_reg(CH3SET_ADDR);
//  uint8_t res4 = read_reg(CH4SET_ADDR);
//  Serial.print(res1); Serial.print(" ");
//  Serial.print(res2); Serial.print(" ");
//  Serial.print(res3); Serial.print(" ");
//  Serial.println(res4);
  
  attachInterrupt(digitalPinToInterrupt(DRDY_PIN), onDRDY, FALLING);
  delay(100);

  digitalWrite(START_PIN, HIGH);

  digitalWrite(H_CS_PIN, LOW);
  delayMicroseconds(2);
  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
  delayMicroseconds(2);
  hspi.transfer(RDATAC_CMD);
  delayMicroseconds(2);
  hspi.endTransaction();
  digitalWrite(H_CS_PIN, HIGH);
}

void loop() {
  if(dataReady) {
    dataReady = false;

    byte data[15] = {0}; // 3 status bytes + 4 channels * 3 bytes = 15 bytes total
    digitalWrite(H_CS_PIN, LOW);
    for(int i = 0; i < 15; i++) {
      data[i] = hspi.transfer(0);
    }
    digitalWrite(H_CS_PIN, HIGH);

    int32_t status_bits24 = ((int32_t)data[0] << 16) | ((int32_t)data[1] << 8) | data[2];
    int32_t chan1_bits24  = ((int32_t)data[3] << 16) | ((int32_t)data[4] << 8) | data[5];
    int32_t chan2_bits24  = ((int32_t)data[6] << 16) | ((int32_t)data[7] << 8) | data[8];
    int32_t chan3_bits24  = ((int32_t)data[9] << 16) | ((int32_t)data[10] << 8) | data[11];
    int32_t chan4_bits24  = ((int32_t)data[12] << 16) | ((int32_t)data[13] << 8) | data[14];

    int32_t chan1_bits32 = convert24To32bit(chan1_bits24);
    int32_t chan2_bits32 = convert24To32bit(chan2_bits24);
    int32_t chan3_bits32 = convert24To32bit(chan3_bits24);
    int32_t chan4_bits32 = convert24To32bit(chan4_bits24);

    int32_t channels[4] = {chan1_bits32, chan2_bits32, chan3_bits32, chan4_bits32};
    Serial.write((uint8_t*)channels, sizeof(channels));
  }
}
