# USB Shifter to Thrustmaster Wheelbase
This allows for compatibility with wheelbase while also retaining PC plug and play functionality. (as it retains USB cable that can be unplugged from shield)

NOTE: This method can be unreliable and I cannot guarantee it will work with your shifter. @NikderNoob95 has kindly tested out the USB shield method on his 6+R USB Shifter but was unable to get it to work. The microcontroller in his shifter was different to the one I am using. If you have a shifter with a microcontroller that uses a type c port on the inside of the unit, this method may not work. Regardless, let me know how you go in the discussions if you attempt this method!

## Required
* Arduino Pro Mini 328 - 3.3V/8MHz (It has to be 3v3 volts to power off of the wheelbase easily)
* USB Hosts Shield mini (perfect for pro mini)
* USB Hosts Libary - https://github.com/felis/USB_Host_Shield_2.0#how-to-include-the-library
* USB to TTL Serial **that supports 3v3 programing** for flashing ino ([USBShifter2PS2.ino](/USBShifter2PS2/USBShifter2PS2.ino "USBShifter2PS2.ino"))
* Mini-Din/PS2 plug (for plugging into the wheel base.)
* USB Shifter of course. I can't guarantee it will work on all usb shifters. (What I used - [link](Images/USB%20Shifter.jpg "USB Shifter"))
  * You can find these by typing in "USB Shifter" on ebay or aliexpress. Pick the one that suits you.

## Instructions (WIP)
### Wiring Diagram
![Shifter Wiring Diagram](https://user-images.githubusercontent.com/40465008/220298924-f1d8d9e6-49c0-4f19-8c90-9cfb2f58f659.jpg)

### Board Mount Image
![IMG_20230221_194732](https://user-images.githubusercontent.com/40465008/220299141-47d3c65f-ebee-4849-89ef-2da4a68b6270.jpg)

### Connection to PC before flashing
Just make sure the programmer is set to 3v3 and not 5v as you may damage the arduino & shield since it only supports 3v3 without modification. Connect up the programmers 3v3, Gnd, TX & RX to the pro mini with the shield on the bottom. And flash away. We will be using the connection for debugging.

### Flashing + Debug
If you have a similar shifter you can probably change it and flash the ino file and connect it to the shifter. However, it's possible that there may be variants of the shifter that have different commands. So we will have to uncomment out some debug code in the arduino file. These test will only work if your shifter is acting like a game controller (in windows for example) Let me know if its acting like keyboard instead and ill see what I can do. 

The image below shows the debug comments undone and the results of moving the shifter in the serial monitor. Notice how the y value is the only one changing?

![image](https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase/assets/40465008/bb0bcd43-24ab-4724-8383-3f831236dcfa)

Since we know the only value changing we can make it a bit cleaner by commenting the debug code again that we just uncommented and instead uncomment this line `Serial.println(evt->y);`

So then we should go through each gear one by one and mark that value that comes in the serial monitor. so when we shift into gear 1 a y value of 31 appears. Don't forget to make the neutral value when the shifter is in the centre. This is ‘Gear 0’ and I have a value of 15. Go through each gear marking the numbers and input then in the ino file under `void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)`

I have marked each gear in comments as well.

I have a switch on my shifter which I have used to enable sequential shifting mode. When the switch is activated it changes all the values of y to a new set of numbers. You may not need this and you can uncomment the sequential shifting options out.
**If your shifter has a switch like mine in the photos, don't forget to also flick the switch and test, the values you get might be similar to mine etc.**
If you have a different shifter to mine, let me know what the values are and a picture of the shifter ill mark it down somewhere as a possible default preset.

Hopefully the code comments provide enough info to get an idea. Let me know if you have any issues, ill try my best to help.
