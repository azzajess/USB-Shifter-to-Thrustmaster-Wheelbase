/*
Arduino 3.3V required or 5V with external power.

Base connector / note / Arduino pin
DIN6_1 /  nothing      /
DIN6_2 /  I2C-SCL      / A5
DIN6_3 /  /Shifter ON  / GND
DIN6_4 /  I2C-SDA      / A4
DIN6_5 /  Vdd          / RAW 3.3V
DIN6_6 /  Vss          / GND
*/

byte command[14] = {
  0x00, // Shifter mode 0 - S / 0x80 - H
  0x0C, // Unknown
  0x01, // Unknown
  0x00, // Gear in H-mode
  0x00, // Gear in S-Mode 0x04 - center, 0x05 - down, 0x06 - up
  0x80, // Unknown
  0x80, // Unknown
  0x00, // Y cordinate
  0x00, // X cordinate
  0x00, // Unknown
  0x00, // Unknown
  0x00, // Unknown
  0x00, // Unknown
  0x00  // Unknown
};

enum position {
  center = 0x04,
  down = 0x05,
  up = 0x06
};

#include <Wire.h>

void setup() {
  Wire.begin(0x03); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.println("START");
  digitalWrite(13, HIGH);
}

void setHMode(bool isHMode) {
  if (isHMode) {
    command[0] |= 0x80;
  } else {
    command[0] &= ~0x80;
  }
}

void switchHGear(byte gear) { // Gear num 0-N, 8-R
  command[3] = (0x80 >> (8-gear));
  Serial.print("Gear ");
  Serial.print(" ");
  Serial.println(gear);
}

void switchSGear(position currpos) {
  command[4] = currpos;
}

void sendCommand() {
  Wire.beginTransmission(0x01);
  Wire.write(command, 14);
  Wire.endTransmission();
}

void tryByte(byte nbyte, byte nbit) {
  command[nbyte] &= ~(0x01 << nbit-1);
  command[nbyte] |= (0x01 << nbit);
  // command[nbyte] |= 0x40;
  Serial.print("Check byte ");
  Serial.print(nbyte);
  Serial.print(" bit ");
  Serial.println(nbit);
}

void loop() {
  // States
  setHMode(true); // Set to H-mode
  switchHGear(3); // Gear 3
  setHMode(false); // Set to Seq-mode
  switchSGear(up); // Press up
  sendCommand();
  Serial.println("Complete");
}