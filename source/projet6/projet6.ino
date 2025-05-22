#include <SPI.h>
#include "functions.h"
#include "definitions.h"

volatile bool dataReady = false;

void onDRDY() {
  dataReady = true;
}

void setup() {
  pin_init();
  serial_init(9600);
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

    byte data[12] = {1,2,3,4,5,6,7,8,9,10,11};
    digitalWrite(H_CS_PIN, LOW);
    for(int i = 0; i < 12; i++) {
      data[i] = hspi.transfer(0);
    }
    digitalWrite(H_CS_PIN, HIGH);
    for(int i = 0; i < 12; i++) {
      Serial.println(data[i]);
    }
  }
}

//volatile bool dataReady = false;
//
//void onDRDY() {
//  dataReady = true;
//}
//
//void setup() {
//  pin_init();
//  serial_init(115200);
//  hspi_init();
//  reset_reg_values();
//
//  
////  write_reg(CONFIG3_ADDR, 0b11100000); // 0xE0
////  write_reg(CONFIG1_ADDR, 0b11010100); // 0xD4
////  write_reg(CONFIG2_ADDR, 0b11010000); // 0xD0
////  write_reg(CH1SET_ADDR,  0b00000101); // 0b101 for test signals
////  write_reg(CH2SET_ADDR,  0b00000101); // 0b101 for test signals
////  write_reg(CH3SET_ADDR,  0b00000101); // 0b101 for test signals
////  write_reg(CH4SET_ADDR,  0b00000101); // 0b101 for test signals
////  
////  attachInterrupt(digitalPinToInterrupt(DRDY_PIN), onDRDY, FALLING);
////  digitalWrite(START_PIN, HIGH);
////  send_RDATAC();
//}
//
//void loop() {
////  byte data[12] = {1,2,3,4,5,5,6,7,8,9,10,11};
////  
////  Serial.print("Waiting on data ready.\n");
////  while(digitalRead(DRDY_PIN) == HIGH);
////  
////  digitalWrite(H_CS_PIN, HIGH);
////  
////  hspi.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
////  
////  for(int i = 0; i < 12; i++) {
////    data[i] = hspi.transfer(0);
////  }
////  
////  hspi.endTransaction();
////  digitalWrite(H_CS_PIN, HIGH);
////  
////  for(int chan = 0; chan < 4; chan++) {
////    for(int b = 0; b < 3; b++) {
////      Serial.print(data[(chan * 3) + b]);
////    }
////    Serial.print("\n");
////  }
////Print ADS1299-4 ID (revision + model)
//  uint8_t id = read_reg(ID_ADDR);
//  Serial.printf("ADS1299-4 ID: 0x%02X\n", id);
//
//  write_reg(CONFIG1_ADDR, 0b11010100);
//  delay(500);
//
//  uint8_t config1 = read_reg(CONFIG1_ADDR);
//
//  delay(500);
//
//  Serial.println(config1);
//  Serial.printf("CONFIG1: 0x%02X\n", config1);
//
//  delay(500);
//}
