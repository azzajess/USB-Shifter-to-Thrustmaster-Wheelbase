#include <Wire.h>

const int gearPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int seqSwitchPin = 10;
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

//bool active = false;
bool prevSequentialMode = false;
byte prevSelectedGear = 0;
position prevSequentialGear = center;


void setup() {
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

  if (sequentialMode) {
    if (digitalRead(gearPins[2]) == LOW) {
      sequentialGear = down;
      gearPressed = true;
    } else if (digitalRead(gearPins[3]) == LOW) {
      sequentialGear = up;
      gearPressed = true;
    } else {
      sequentialGear = center;
    }
    setHMode(false);
  } else {
    for (int i = 0; i < 8; i++) {
      if (digitalRead(gearPins[i]) == LOW) {
        selectedGear = i + 1;
        gearPressed = true;
        setHMode(true);
        break;
      }
    }
  }

  if (!gearPressed) {
    selectedGear = 0;
    setHMode(true);
  }

  if (sequentialMode && (sequentialMode != prevSequentialMode || sequentialGear != prevSequentialGear)) {
    switchSGear(sequentialGear);
    sendCommand();
  } else if (!sequentialMode && (selectedGear != prevSelectedGear)) {
    switchHGear(selectedGear);
    sendCommand();
  }

  prevSequentialMode = sequentialMode;
  prevSelectedGear = selectedGear;
  prevSequentialGear = sequentialGear;

  delay(debounceDelay);
}
