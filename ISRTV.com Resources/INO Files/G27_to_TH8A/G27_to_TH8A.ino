// TH8A
/*
Arduino 3.3V required or 5V with external power.

Base connector / note / Arduino pin - my color thread
DIN6_1 /  nothing      /
DIN6_2 /  I2C-SCL      / A5   azul forte - blue dark
DIN6_3 /  /Shifter ON  / GND   azul fraco - blue light
DIN6_4 /  I2C-SDA      / A4    branco - white
DIN6_5 /  Vdd          / RAW 3.3V   vermelho - red
DIN6_6 /  Vss          / GND        laranja - orange
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


// G27_Shifter_USB

//#include <HID.h>
#include <G27PedalsShifter.h>

#define USE_SHIFTER


// SHIFTER PINS
//| DB9 | Original | Harness | Shifter | Description             | Pro Micro   |
//|   1 | Purple   | Purple  |       1 | Button Clock            | pin 0       |
//|   2 | Grey     | Blue    |       7 | Button Data             | pin 1       |
//|   3 | Yellow   | Yellow  |       5 | Button !CS & !PL (Mode) | pin 4       |
//|   4 | Orange   | Orange  |       3 | Shifter X axis          | pin 8  (A8) |
//|   5 | White    | White   |       2 | SPI input               |             |
//|   6 | Black    | Black   |       8 | GND                     | GND         |
//|   7 | Red      | Red     |       6 | +5V                     | VCC         |
//|   8 | Green    | Green   |       4 | Shifter Y axis          | pin 9 (A9)  |
//|   9 | Red      | Red     |       1 | +5V                     | VCC         |
#define SHIFTER_CLOCK_PIN  0
#define SHIFTER_DATA_PIN   1
#define SHIFTER_MODE_PIN   4
#define SHIFTER_X_PIN      A0
#define SHIFTER_Y_PIN      A1

// BUTTON DEFINITIONS
#define BUTTON_REVERSE         1

#define BUTTON_RED_CENTERRIGHT 4
#define BUTTON_RED_CENTERLEFT  5
#define BUTTON_RED_RIGHT       6
#define BUTTON_RED_LEFT        7
#define BUTTON_BLACK_TOP       8
#define BUTTON_BLACK_RIGHT     9
#define BUTTON_BLACK_LEFT      10
#define BUTTON_BLACK_BOTTOM    11
#define BUTTON_DPAD_RIGHT      12
#define BUTTON_DPAD_LEFT       13
#define BUTTON_DPAD_BOTTOM     14
#define BUTTON_DPAD_TOP        15

#define OUTPUT_BLACK_TOP       7
#define OUTPUT_BLACK_LEFT      8
#define OUTPUT_BLACK_RIGHT     9
#define OUTPUT_BLACK_BOTTOM    10
#define OUTPUT_DPAD_TOP        11
#define OUTPUT_DPAD_LEFT       12
#define OUTPUT_DPAD_RIGHT      13
#define OUTPUT_DPAD_BOTTOM     14
#define OUTPUT_RED_LEFT        15
#define OUTPUT_RED_CENTERLEFT  16
#define OUTPUT_RED_CENTERRIGHT 17
#define OUTPUT_RED_RIGHT       18

// SHIFTER AXIS THRESHOLDS
#define SHIFTER_XAXIS_12        400 //Gears 1,2
#define SHIFTER_XAXIS_56        600 //Gears 5,6, R
#define SHIFTER_YAXIS_135       700 //Gears 1,3,5
#define SHIFTER_YAXIS_246       300 //Gears 2,4,6, R



// MISC.
#define MAX_AXIS            1023
#define SIGNAL_SETTLE_DELAY 10

// SHIFTER CODE
int buttonTable[] = {
  // first four are unused
  0, 0, 0, 0,
  OUTPUT_RED_CENTERRIGHT,
  OUTPUT_RED_CENTERLEFT,
  OUTPUT_RED_RIGHT,
  OUTPUT_RED_LEFT,
  OUTPUT_BLACK_TOP,
  OUTPUT_BLACK_RIGHT,
  OUTPUT_BLACK_LEFT,
  OUTPUT_BLACK_BOTTOM,
  OUTPUT_DPAD_RIGHT,
  OUTPUT_DPAD_LEFT,
  OUTPUT_DPAD_BOTTOM,
  OUTPUT_DPAD_TOP
};

void waitForSignalToSettle() {
  delayMicroseconds(SIGNAL_SETTLE_DELAY);
}

void getButtonStates(int *ret) {
  digitalWrite(SHIFTER_MODE_PIN, LOW);    // Switch to parallel mode: digital inputs are read into shift register
  waitForSignalToSettle();
  digitalWrite(SHIFTER_MODE_PIN, HIGH);   // Switch to serial mode: one data bit is output on each clock falling edge

#if defined(DEBUG_SHIFTER)
  Serial.print("\nBUTTON STATES:");
#endif

  for(int i = 0; i < 16; ++i) {           // Iteration over both 8 bit registers
    digitalWrite(SHIFTER_CLOCK_PIN, LOW);         // Generate clock falling edge
    waitForSignalToSettle();

    ret[i] = digitalRead(SHIFTER_DATA_PIN);

#if defined(DEBUG_SHIFTER)
    if (!(i % 4)) Serial.print("\n");
    Serial.print(" button");
    if (i < 10) Serial.print(0);
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(ret[i]);
#endif

    digitalWrite(SHIFTER_CLOCK_PIN, HIGH);        // Generate clock rising edge
    waitForSignalToSettle();
  }
}

void getShifterPosition(int *ret) {
  ret[0] = analogRead(SHIFTER_X_PIN);
  ret[1] = analogRead(SHIFTER_Y_PIN);
}

int getCurrentGear(int shifterPosition[], int btns[]) {
  int gear = 0;  // default to neutral
  int x = shifterPosition[0], y = shifterPosition[1];

    if (x < SHIFTER_XAXIS_12)                // Shifter on the left?
    {
      if (y > SHIFTER_YAXIS_135) gear = 1;   // 1st gear
      if (y < SHIFTER_YAXIS_246) gear = 2;   // 2nd gear
    }
    else if (x > SHIFTER_XAXIS_56)           // Shifter on the right?
    {
      if (y > SHIFTER_YAXIS_135) gear = 5;   // 5th gear
      if (y < SHIFTER_YAXIS_246) gear = 6;   // 6th gear
    }
    else                                     // Shifter is in the middle
    {
      if (y > SHIFTER_YAXIS_135) gear = 3;   // 3rd gear
      if (y < SHIFTER_YAXIS_246) gear = 4;   // 4th gear
    }

  if (gear != 6) btns[BUTTON_REVERSE] = 0;  // Reverse gear is allowed only on 6th gear position
  if (btns[BUTTON_REVERSE] == 1) gear = 7;  // Reverse is 7th gear (for the sake of argument)


if (gear == 0){
  setHMode(true);
  switchHGear(0);
  sendCommand();
}
else if (gear == 1){
  setHMode(true);
  switchHGear(1);
  sendCommand();
}
else if (gear == 2){
  setHMode(true);
  switchHGear(2);
  sendCommand();
}
else if (gear == 3){
  setHMode(true);
  switchHGear(3);
  sendCommand();
}
else if (gear == 4){
  setHMode(true);
  switchHGear(4);
  sendCommand();
}
else if (gear == 5){
  setHMode(true);
  switchHGear(5);
  sendCommand();
}
else if (gear == 6){
  setHMode(true);
  switchHGear(6);
  sendCommand();
}
else if (gear == 7){
  setHMode(true);
  switchHGear(8);
  sendCommand();
}
  return gear;
  
  
}

void setButtonStates(int buttons[], int gear) {
  // release virtual buttons for all gears
  for (byte i = 0; i < 7; ++i) {
    G27.setButton(i, LOW);
  }

  if (gear > 0) {
    G27.setButton(gear - 1, HIGH);
  }

  for (byte i = BUTTON_RED_CENTERRIGHT; i <= BUTTON_DPAD_TOP; ++i) {
    G27.setButton(buttonTable[i], buttons[i]);
  }
}

void describeButtonStates(int buttons[], int shifterPosition[], int gear) {
  Serial.print("\nSHIFTER X: ");
  Serial.print(shifterPosition[0]);
  Serial.print(" Y: ");
  Serial.print(shifterPosition[1]);

  Serial.print(" GEAR: ");
  Serial.print(gear);
  Serial.print(" REVERSE: ");
  Serial.print(buttons[BUTTON_REVERSE]);

  Serial.print(" RED BUTTONS:");
  if (buttons[BUTTON_RED_LEFT]) {
    Serial.print(" 1");
  }
  if (buttons[BUTTON_RED_CENTERLEFT]) {
    Serial.print(" 2");
  }
  if (buttons[BUTTON_RED_CENTERLEFT]) {
    Serial.print(" 3");
  }
  if (buttons[BUTTON_RED_RIGHT]) {
    Serial.print(" 4");
  }

  Serial.print(" BLACK BUTTONS:");
  if (buttons[BUTTON_BLACK_LEFT]) {
    Serial.print(" LEFT");
  }
  if (buttons[BUTTON_BLACK_TOP]) {
    Serial.print(" TOP");
  }
  if (buttons[BUTTON_BLACK_BOTTOM]) {
    Serial.print(" BOTTOM");
  }
  if (buttons[BUTTON_BLACK_RIGHT]) {
    Serial.print(" RIGHT");
  }

  Serial.print(" D-PAD:");
  if (buttons[BUTTON_DPAD_LEFT]) {
    Serial.print(" LEFT");
  }
  if (buttons[BUTTON_DPAD_TOP]) {
    Serial.print(" UP");
  }
  if (buttons[BUTTON_DPAD_BOTTOM]) {
    Serial.print(" DOWN");
  }
  if (buttons[BUTTON_DPAD_RIGHT]) {
    Serial.print(" RIGHT");
  }
}



























void setup() {

//TH8A
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  
  Wire.begin(0x03); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.println("START");
  digitalWrite(13, HIGH);

//SHIFTER G27
  #if !defined(DEBUG_SHIFTER)
    G27.begin(false);
  #endif


  pinMode(SHIFTER_MODE_PIN, OUTPUT);
  pinMode(SHIFTER_CLOCK_PIN, OUTPUT);

  digitalWrite(SHIFTER_MODE_PIN, HIGH);
  digitalWrite(SHIFTER_CLOCK_PIN, HIGH);

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
  Serial.print("Gear ");
  Serial.print(" ");
  Serial.println(currpos);
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
// SHIFTER G27
  int buttonStates[16];
  getButtonStates(buttonStates);
  int shifterPosition[2];
  getShifterPosition(shifterPosition);
  int gear = getCurrentGear(shifterPosition, buttonStates);

#if defined(DEBUG_SHIFTER)
  describeButtonStates(buttonStates, shifterPosition, gear);
#elif defined(USE_SHIFTER)
  setButtonStates(buttonStates, gear);
#endif

#if !defined(DEBUG_SHIFTER) || !defined(DEBUG_PEDALS)
  G27.sendState();
#endif

}
