/* 
 *  Based on:
 *  Simplified Thrustmaster T.16000M FCS Joystick Report Parser ()
 *  &
 *  https://www.isrtv.com/forums/topic/24532-gearbox-connector-on-base/
 *  
 *  Modified by azzajess to suit USB Shifter 6+R+Switch ()
 *  
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

//commands seen from ociliscope in forum - 
//appentely these codes have changed for different wheel versions
//these work for me...
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

//uint16_t gear1 = 000;

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
  //virtual void OnButtonUp(uint8_t but_id);
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
/* Original.
  // Checking if there are changes in report since the method was last called
  // match = true (1) when size of old pan and memcmp = 0
  //  bool match = (sizeof(oldPad) == len) && (memcmp(oldPad, buf, len) == 0);
*/
  // match is true when memcmp = 0 (basically when nothing is happening it wont loop
  bool match = (memcmp(oldPad, buf, len) == 0);
  //Serial.println(memcmp(oldPad, buf, len));
  //  Serial.println(match);
  //  Serial.println(sizeof(oldPad));
//   Calling Game Pad event handler
//check if nothing is happening otherwise activate
  if (!match && joyEvents) {
    joyEvents->OnGamePadChanged((const GamePadEventData*)buf);
    memcpy(oldPad, buf, len);
  }
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
  Serial.println(gear);
}

void switchSGear(position currpos) {
  command[4] = currpos;
  Serial.print("SEQ Gear ");
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

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)
{
/*
    States These are just example commands.
    setHMode(true); // Set to H-mode
    switchHGear(3); // Gear 3 (or any number from 0-8
    setHMode(false); // Set to Seq-mode
    switchSGear(up); // Press up in seq
    sendCommand(); 
*/
    //These numbers are exact codes i get when using debug and shifting in different gears
    //these may be different depending on shifter? Use debug to find them out and fill it in
    if (evt->y == 15){
      //Neutral/Gear0
      setHMode(true);
      switchHGear(0);
      sendCommand();
    } else if (evt->y == 31){
      //Gear 1
      setHMode(true);
      switchHGear(1);
      sendCommand();
      } else if (evt->y == 47){
      //Gear 2
      setHMode(true);
      switchHGear(2);
      sendCommand();  
      } else if (evt->y == 79){
      //Gear 3
      setHMode(true);
      switchHGear(3);
      sendCommand();
      } else if (evt->y == 143){
      //Gear 4
      setHMode(true);
      switchHGear(4);
      sendCommand();
      } else if (evt->y == 271){
      //Gear 5
      setHMode(true);
      switchHGear(5);
      sendCommand();
      } else if (evt->y == 527){
      //Gear 5
      setHMode(true);
      switchHGear(6);
      sendCommand();
//    } else if (evt->y == ){
//      //Gear 7 (My shifter doesnt have 7th gear so this has to be filled out if you have one)
//      setHMode(true);
//      switchHGear(7);
//      sendCommand();
      } else if (evt->y == 2063){
      //Gear 8/Reverse
      setHMode(true);
      switchHGear(8);
      sendCommand();
      }
      //Switch enable = Sequential mode
      else if (evt->y == 8207) {
      setHMode(false);
      switchSGear(center);
      sendCommand();
      }
      else if (evt->y == 8335) {
      //Sequential Switch on = Shift up
      setHMode(false);
      switchSGear(up);
      sendCommand();
      }
      else if (evt->y == 8271) {
        //Sequential Switch on = Shift down
      setHMode(false);
      switchSGear(down);
      sendCommand();
      }

//  debug section. I had these all enabled to figure it out then only used the numbers
//  that had changed from one another and marked them down. For me y: is what changed

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


    //or just use this singular (your letter may vary?)
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

   //TH8A (from 
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
