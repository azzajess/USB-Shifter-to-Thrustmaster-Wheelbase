# USB Shifter to Thrustmaster Wheelbase
Use a Generic USB Shifter to connect to a Thrustmaster wheelbase, Emulating a TH8A shifter which will work on console!
This allows for compatibility with wheelbase while also retaining PC plug and play functionality with the USB Shiel method or if you want a more permanent solution try the hardwire option

Might work on other wheelbases but have only tested it on a T300 RS GT + PS4 (GT7)

IRTV Forums suggest it will also work on TMX wheelbase. It might work on any wheelbase that uses the TH8A Mini-Din (PS2) port

## Pre-requisuite Knowledge
* You will need to know how to solder wires
* Flash Arduino with TTL Adaptor
* Know how to import a libary into Arduino IDE
* Comment/uncomment code and read from serial monitor in Arduino IDE

## Hardwire or USB Shield?
Clicking the link will take you to the instructions page showing you whats required, how to 
### [Hardwire](ShifterDirectWire.md)
This method involves opening up the shifter and identifying the pins and corresponding microswitches so we can directly wire them to the arduino.
This is a more permanent solution and you will lose the USB functionality due to having to remove the USB microcontroller in the shifter to easily access the pins. It is possible to retain both in the shifter with a bit of fiddling and work, but I won't be going through that in this guide.
### [USB Shield](USBShield.md)
This allows for compatibility with the wheelbase while also retaining PC USB plug and play functionality. (as it retains USB cable that can be unplugged from shield)
This is more of a 'plug and play' solution as nothing is permanently attached or removed. Unfortunately I have gotten a report thus far that it hasn't worked for them (more info in discussions and in the documentation) so your mileage may vary. USB shifters are not all made the same way, which can introduce variables. I have had success with this method however on my 6+R+switch Shifter. So, let me know how you go with this method should you try it.

## Print or Cut your own Shifter Top Plates!
I have also made up faceplates that closely match the original top plates!
These come in 6+R, 7+R and switch versions that can be both 3d printed or laser cut (dxf files)

You can find more details and downloads to these below
* Thingiverse - https://www.thingiverse.com/thing:6157415
* Printables - https://www.printables.com/model/545213-usb-shifter-top-plate


And the original Onshape sketch here

https://cad.onshape.com/documents/a3d9d44c33b1132797555d44/w/ee8f103d1e1c9a353ae6a04f/e/e706485c28816b59edb06570?renderMode=0&uiState=64cf4ada2f1e726c453e6d22

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
