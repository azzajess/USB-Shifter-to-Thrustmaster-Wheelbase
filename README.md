# USB Shifter to Thrustmaster Wheelbase
Use a Generic USB Shifter to connect to a Thrustmaster wheelbase, Emulating a TH8A shifter which will work on console!

Might work on other wheelbases but have only tested it on a T300 RS GT + PS4 (GT7)

IRTV Forums suggest it will also work on TMX wheelbase. It might work on any wheelbase that uses the TH8A Mini-Din (PS2) port


## Required
* Arduino Pro Mini 328 - 3.3V/8MHz (It has to be 3v3 volts to power off of the wheelbase easily)
* USB Hosts Shield mini (perfect for pro mini)
* USB Hosts Libary - https://github.com/felis/USB_Host_Shield_2.0#how-to-include-the-library
* Mini-Din/PS2 plug (for plugging into the wheel base.)
* USB Shifter of course. I can't guarantee it will work on all usb shifters. (What I used - [link](Images/USB%20Shifter.jpg "USB Shifter"))
  * You can find these by typing in "USB Shifter" on ebay or aliexpress. Pick the one that suits you.

## Instructions (WIP)
### Wiring Diagram
![Shifter Wiring Diagram](https://user-images.githubusercontent.com/40465008/220298924-f1d8d9e6-49c0-4f19-8c90-9cfb2f58f659.jpg)

### Board Mount Image
![IMG_20230221_194732](https://user-images.githubusercontent.com/40465008/220299141-47d3c65f-ebee-4849-89ef-2da4a68b6270.jpg)

### Flashing + Debug
If you have a similar shifter you can probably change it and flash the ino file and connect it to the shifter. However, it's possible that there may be variants of the shifter that have different commands. So we will have to uncomment out some debug code in the arduino file. These test will only work if your shifter is acting like a game controller (in windows for example) Let me know if its acting like keyboard instead and ill see what I can do. 
The image below shows the debug comments undone and the results of moving the shifter in the serial monitor. Notice how the y value is the only one changing?

![image](https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase/assets/40465008/bb0bcd43-24ab-4724-8383-3f831236dcfa)

Since we know the only value changing we can make it a bit cleaner by commenting the debug code again that we just uncommented and instead uncomment this line `Serial.println(evt->y);`
So then we should go through each gear one by one and mark that value that comes in the serial monitor. so when we shift into gear 1 a y value of 31 appears. Don't forget to make the neutral value when the shifter is in the centre. This is ‘Gear 0’ and I have a value of 15. Go through each gear marking the numbers and input then in the ino file under `void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)`
I have marked each gear in comments as well.
I have a switch on my shifter whichI have used to enable sequential shifting mode. When the switch is activated it changes all the values of y to a new set of numbers. You may not need this and you can uncomment the sequential shifting options out.
If your shifter has a switch like mine in the photos, don't forget to also flick the switch and test, the values you get might be similar to mine etc.
If you have a different shifter to mine, let me know what the values are and a picture of the shifter ill mark it down somewhere as a possible default preset.

Hopefully the code comments provide enough info to get an idea. Let me know if you have any issues, ill try my best to help.


## Disclaimer
Look, this is really hastily put together. I really wasnt happy with the lack of documentation for my dumb brain to comprehend so I used a Shield example that got me 90% of the way there and then piggy backed off that.

I'm 100% sure there is a better way of doing this that is cleaner but it does indeed work. Happy for anyone to provide suggestions, improvements and such!

Included is a reference folder for additional stuff downloaded from ISRTV.com. The information and .ino files are way too valuable to not have a backup somewhere...


Thanks to the people at ISRTV.com that did the heavy lifting. Their code was enough for me to adapt it here. Not ideal, but 



## Resources/Research
* USB Hosts Shield - https://chome.nerpa.tech/usb-host-shield-hardware-manual/
* USB Host Shield github info + examples - https://github.com/felis/USB_Host_Shield_2.0/
* Alot of the USB Shield Host code came from this example ino - https://github.com/felis/USB_Host_Shield_2.0/blob/master/examples/HID/t16km/t16km.ino#L55
* ISRTV forum where a lot of the info was from - https://www.isrtv.com/forums/topic/24532-gearbox-connector-on-base/
