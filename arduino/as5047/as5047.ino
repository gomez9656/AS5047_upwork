#include "SPI.h"
#define OP_READ  0x4000
#define PARITY_EVEN 0x0000
#define PARITY_ODD  0x8000
#define ADDR_NOP 0x0000
#define ADDR_ERRFL 0x0001
#define ADDR_ANGLECOM     0x3FFF
#define CMD_R_ANGLECOM  (PARITY_ODD  | OP_READ | ADDR_ANGLECOM)
#define CMD_R_NOP       (PARITY_ODD  | OP_READ | ADDR_NOP)  
#define CMD_R_ERRFL     (PARITY_EVEN | OP_READ | ADDR_ERRFL)
#define csPin 10 // pin 9 is used for selecting the first encoder
uint16_t x;
void setup() {
  Serial.begin(115200);

  // set psi options
  SPI.setDataMode(SPI_MODE1);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();

  // begin spi
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);

  // check
  //send data
  digitalWrite(csPin, LOW);
  x = SPI.transfer16(0x0001);
  digitalWrite(csPin, HIGH);
  Serial.println(x); // 0

  digitalWrite(csPin, LOW);
  x = SPI.transfer16(0x0007);
  digitalWrite(csPin, HIGH);
  Serial.println(x); // 49152

  // read
  digitalWrite(csPin, LOW);
  x = SPI.transfer16(CMD_R_ERRFL); 
  digitalWrite(csPin, HIGH);
  Serial.println(x); // 49152
  
  digitalWrite(csPin, LOW);
  x = SPI.transfer16(CMD_R_NOP);
  digitalWrite(csPin, HIGH);
  Serial.println(x); // 16388 
}

void loop() {
  digitalWrite(csPin, LOW);
  SPI.transfer16(CMD_R_ANGLECOM); 
  digitalWrite(csPin, HIGH);
  Serial.println(x); 
  digitalWrite(csPin, LOW);
  x = SPI.transfer16(CMD_R_NOP);
  digitalWrite(csPin, HIGH);
  x &= ~(1UL << 15);
  x &= ~(1UL << 14);
  Serial.print("Angle = ");
  Serial.println(x/16384.0*360.0);
  delay(100);
}




  
