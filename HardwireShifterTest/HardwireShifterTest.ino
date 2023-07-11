

#include <Wire.h>





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

//commands seen from ociliscope in ISRTV forum - 
//apparently these codes have changed for different wheel versions however
//these worked for me...
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

//Sequential shift
enum position {
  center = 0x04,
  down = 0x05,
  up = 0x06
};



void setup() {
  //configure pins as input with pullup resistor allowing for switches to be connected
  //between pins and ground.
  pinMode(2, INPUT_PULLUP); //Gear 1
  pinMode(3, INPUT_PULLUP); //Gear 2
  pinMode(4, INPUT_PULLUP); //Gear 3
  pinMode(5, INPUT_PULLUP); //Gear 4
  pinMode(6, INPUT_PULLUP); //Gear 5
  pinMode(7, INPUT_PULLUP); //Gear 6
  pinMode(8, INPUT_PULLUP); //Reverse
  Wire.begin(0x03); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.println("START");
  digitalWrite(13, HIGH);

  //On startup sends command neutral to initialise this. It isnt nessacary but
  //saves from moving to shift and back. Just be mindful of sequential switch i guess
  setHMode(true);
  switchHGear(0);
  sendCommand(); 
}

//H mode shifting codes
void setHMode(bool isHMode) {
  if (isHMode) {
    command[0] |= 0x80;
  } else {
    command[0] &= ~0x80;
  }
}

//Switchs gear to specificed gear number and display in serial monitor
void switchHGear(byte gear) { // Gear num 0-N, 8-R
  command[3] = (0x80 >> (8-gear));
  Serial.print("Gear ");
  Serial.print(" ");
  Serial.println(gear);
}

//Switchs gear to specificed gear number and display in serial monitor
void switchSGear(position currpos) {
  command[4] = currpos;
}

//sends command over PS2 port to T300 wheelbase
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
//read the pushbutton value into a variable
//int sensorVal = digitalRead(2);
   
  
  //Gear 1
  if (digitalRead(2) == LOW) {
    setHMode(true);
    switchHGear(1);
    sendCommand();
    Serial.print("Gear 1");
  }

  //Gear 2
  else if (digitalRead(3) == LOW) {
    setHMode(true);
    switchHGear(2);
    sendCommand();
    Serial.print("Gear 2");
  }

  //Gear 3
  else if (digitalRead(4) == LOW) {
    setHMode(true);
    switchHGear(3);
    sendCommand();
    Serial.print("Gear 3");
  }

  //Gear 4
  else if (digitalRead(5) == LOW) {
    setHMode(true);
    switchHGear(4);
    sendCommand();
    Serial.print("Gear 4");
  }

  //Gear 5
  else if (digitalRead(6) == LOW) {
    setHMode(true);
    switchHGear(5);
    sendCommand();
    Serial.print("Gear 5");
  }

  //Gear 6
  else if (digitalRead(7) == LOW) {
    setHMode(true);
    switchHGear(6);
    sendCommand();
    Serial.print("Gear 6");

    //    Gear 7 (My shifter doesnt have 7th gear so this has to be filled out if you have one)
    //    } else if (evt->y == ){
    //      setHMode(true);
    //      switchHGear(7);
    //      sendCommand();
  }

  //Gear 8/Reverse
  else if (digitalRead(8) == LOW) {
    setHMode(true);
    switchHGear(8);
    sendCommand();
    Serial.print("Reverse");
  }

  // //Switch enable = Sequential mode
  // else if (evt->y == 8207) {
  // setHMode(false);
  // switchSGear(center);
  // sendCommand();
  // }

  // //Sequential Switch on = Shift up
  // else if (evt->y == 8335) {
  // setHMode(false);
  // switchSGear(up);
  // sendCommand();
  // }

  // //Sequential Switch on = Shift down
  // else if (evt->y == 8271) {
  // setHMode(false);
  // switchSGear(down);
  // sendCommand();
  // }

  //Neutral/Gear0
  //if no other buttons are being activated then it must(?) be in neutral
  else {
    setHMode(true);
    switchHGear(0);
    sendCommand();
    Serial.print("Neutral");
  }

// States
  //setHMode(true);   // Set to H-mode
  //switchHGear(3);   // Gear 3
  //setHMode(false);  // Set to Seq-mode
  //switchSGear(up);  // Press up
  //sendCommand();
  //Serial.println("Complete");
}
