/* 
 *  Based on:
 *  Information provided by the People at ISRTV.com - https://www.isrtv.com/forums/topic/24532-gearbox-connector-on-base/
 *  
 *  Modified by azzajess to suit USB Shifter 6+R+Switch via direct wire (https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase)
 *  v1.2
*/

#include <Wire.h>

//setting up the arduino pins and putting them in an array
const int gearPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
//setting the shifter switch under pin 10
const int seqSwitchPin = 10;
//switch debounce delay
const int debounceDelay = 50; // Adjust this value as needed

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

//checks to make sure switch doesnt infintely trigger
bool prevSequentialMode = false;
byte prevSelectedGear = 0;
position prevSequentialGear = center;


void setup() {
  // for loop that sets gear pins as INPUT_PULLUP to enable internal pull-up resistors on arduino
  for (int i = 0; i < 8; i++) {
    pinMode(gearPins[i], INPUT_PULLUP);
  }
  pinMode(seqSwitchPin, INPUT_PULLUP);
  
  Wire.begin(0x03); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial.println("START");
  digitalWrite(13, HIGH);

  //On startup sends command neutral to initialize this. It isn't necessary, but
  //saves from moving to shift and back. Just be mindful of the sequential switch, I guess.
  setHMode(true);
  switchHGear(0);
  sendCommand();

  prevSequentialMode = digitalRead(seqSwitchPin) == LOW;
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
  //Serial.println(currpos);
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
  bool sequentialMode = digitalRead(seqSwitchPin) == LOW;
  bool gearPressed = false;
  byte selectedGear = 0;
  position sequentialGear = center;

  // Check if the sequential switch is in ON position
  // You can swap the 'sequentialGear =' if you want to reverse seq shifting
  // e.g down is up, up is down etc.
  if (sequentialMode) {
    // Check if gear 3 button is pressed
    if (digitalRead(gearPins[2]) == LOW) {
      sequentialGear = down;
      gearPressed = true;
    // Check if gear 4 button is pressed
    } else if (digitalRead(gearPins[3]) == LOW) {
      sequentialGear = up;
      gearPressed = true;
    } else {
      sequentialGear = center; // No gear button is pressed, set to center position
    }
    setHMode(false);
  } else {
    // Check gear buttons in H mode
    for (int i = 0; i < 8; i++) {
      if (digitalRead(gearPins[i]) == LOW) {
        selectedGear = i + 1; // Gear 1 to 8 (or neutral: 0)
        gearPressed = true;
        setHMode(true);
        break;
      }
    }
  }

  if (!gearPressed) {
    selectedGear = 0; // No gear button pressed, set to neutral
    setHMode(true);
  }
  // If in sequential mode and there's a change in gear position, send command
  if (sequentialMode && (sequentialMode != prevSequentialMode || sequentialGear != prevSequentialGear)) {
    switchSGear(sequentialGear);
    sendCommand();
  // If not in sequential mode and there's a change in gear position, send command
  } else if (!sequentialMode && (selectedGear != prevSelectedGear)) {
    switchHGear(selectedGear);
    sendCommand();
  }
  // Update previous states for the next loop iteration
  prevSequentialMode = sequentialMode;
  prevSelectedGear = selectedGear;
  prevSequentialGear = sequentialGear;

  delay(debounceDelay); // Introduce a delay for switch debounce
}
