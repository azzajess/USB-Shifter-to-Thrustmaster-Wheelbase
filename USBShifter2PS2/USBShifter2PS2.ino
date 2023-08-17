/* 
 *  Based on:
 *  Simplified Thrustmaster T.16000M FCS Joystick Report Parser (https://github.com/felis/USB_Host_Shield_2.0/blob/master/examples/HID/t16km/t16km.ino)
 *  &
 *  People at ISRTV.com - https://www.isrtv.com/forums/topic/24532-gearbox-connector-on-base/
 *  
 *  Modified by azzajess to suit USB Shifter 6+R+Switch (https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase)
 *  v1.2
*/

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>
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

// Thrustmaster T.16000M HID report
struct GamePadEventData
{
  uint16_t  buttons;
  uint8_t   hat;
  uint16_t  x;
  uint16_t  y;
  uint8_t   twist;
  uint8_t   slider;
}__attribute__((packed));

class JoystickEvents
{
public:
  virtual void OnGamePadChanged(const GamePadEventData *evt);
};

#define RPT_GAMEPAD_LEN sizeof(GamePadEventData)

class JoystickReportParser : public HIDReportParser
{
  JoystickEvents    *joyEvents;

  uint8_t oldPad[RPT_GAMEPAD_LEN];

public:
  JoystickReportParser(JoystickEvents *evt);

  virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};


JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
  joyEvents(evt)
{}

void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
/* Original.  Code caused commands to infinitely loop.
  // Checking if there are changes in report since the method was last called
  // match = true (1) when size of old pan and memcmp = 0
  //  bool match = (sizeof(oldPad) == len) && (memcmp(oldPad, buf, len) == 0);
*/
  // match is true when memcmp = 0 (basically if there is no shifting, then its false
  //so it doesnt infintely loop
  bool match = (memcmp(oldPad, buf, len) == 0);

//   Calling Game Pad event handler
//check if nothing is happening otherwise activate commands
  if (!match && joyEvents) {
    joyEvents->OnGamePadChanged((const GamePadEventData*)buf);
    memcpy(oldPad, buf, len);
  }
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
  Serial.println(gear);
}

//Switchs gear to specificed gear number and display in serial monitor
void switchSGear(position currpos) {
  command[4] = currpos;
  Serial.print("SEQ Gear ");
  Serial.println(currpos);
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

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)
{
/*
    States These are just example commands.
    setHMode(true); // Set to H-mode
    switchHGear(3); // Gear 3 (or any number from 0-8
    setHMode(false); // Set to Seq-mode
    switchSGear(up); // Press up in seq
    sendCommand(); 


    These numbers are exact codes i get when using debug and shifting in different gears
    these may be different depending on shifter? 
    Use debug below these if statements to find them from serial monitor and replace
    the codes from there

*/
    
  //check if any of the keypad codes are the sequential commands
  //replace as necessary.
  bool isSequential = (evt->y == 8207 || evt->y == 8271 || evt->y == 8335); // Sequential mode range
  uint8_t gear = 0; // Gear 0 (neutral) by default

  if (!isSequential) {
    // H-Mode gear selection
    setHMode(true);

    // Gear mapping based on evt->y value for H-mode
    //change values depending on your shifter (if its different)
    if (evt->y == 31)      gear = 1;
    else if (evt->y == 47) gear = 2;
    else if (evt->y == 79) gear = 3;
    else if (evt->y == 143) gear = 4;
    else if (evt->y == 271) gear = 5;
    else if (evt->y == 527) gear = 6;
    else if (evt->y == 1039) gear = 7;
    else if (evt->y == 2063) gear = 8; // Reverse
     

    switchHGear(gear);
  }
  else {
    // Sequential mode gear selection
    setHMode(false);
    //gear mapping based off evt->y in S-mode
    if (evt->y == 8335)      switchSGear(up);    // Shift up
    else if (evt->y == 8271) switchSGear(down);  // Shift down
    else                     switchSGear(center); // Neutral
  }

  
  sendCommand();

/*
      Debug/Code Find Section
      Uncomment the below Code then flash the INO to arduino and connect to Shifter
      over USB. Then go through each of the gears on Shifter and mark down what
      each code is for each gear. If you have a switch like I do, then that changes
      the code of each shift. I used this to havea toggle between sequential and H shift
      For me, none of the other values had changed except for y:.
      I used this for the shift detection above.
*/

//  Serial.print("X: ");
//  Serial.print(evt->x);
//  Serial.print(" Y: ");
//  Serial.print(evt->y);
//  Serial.print(" Hat Switch: ");
//  Serial.print(evt->hat);
//  Serial.print(" Twist: ");
//  Serial.print(evt->twist);
//  Serial.print(" Slider: ");
//  Serial.print(evt->slider);
//  Serial.print(" Buttons: ");
//  Serial.print(evt->buttons);
//  Serial.println();


    //If its easier, just uncomment this for a clener clean debug once you know the only
    //value that changes is y.
//    Serial.println(evt->y);
}

USB                                             Usb;
USBHub                                          Hub(&Usb);
HIDUniversal                                    Hid(&Usb);
JoystickEvents                                  JoyEvents;
JoystickReportParser                            Joy(&JoyEvents);

void setup()
{
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
      Serial.println("OSC did not start.");

  delay( 200 );

  if (!Hid.SetReportParser(0, &Joy))
      ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  );

  //TH8A
  Wire.begin(0x03); // join i2c bus (address optional for master)
  digitalWrite(13, HIGH);

  //On startup sends command neutral to initialise this. It isnt nessacary but
  //saves from moving to shift and back. Just be mindful of sequential switch i guess
  setHMode(true);
  switchHGear(0);
  sendCommand();  
}

void loop()
{
    Usb.Task();

}
