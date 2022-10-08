# USB Shifter to Thrustmaster Wheelbase
Use a Generic USB Shifter to connect to a Thrustmaster wheelbase, Emulating a TH8A shifter which will work on console!

Might work on other wheelbases but have only tested it on a T300 RS GT + PS4 (GT7)

IRTV Forums suggest it will also work on TMX wheelbase. It might work on any wheelbase that uses the TH8A PS2 port


## Required
* Arduino Pro Mini 328 - 3.3V/8MHz (It has to be 3v3 volts to power off of the wheelbase easily)
* USB Hosts Shield mini (perfect for pro mini)
* USB Hosts Libary - https://github.com/felis/USB_Host_Shield_2.0#how-to-include-the-library
* PS2 plug (for plugging into the wheel base.)
* USB Shifter of course. I can't guarantee it will work on all usb shifters. (What I used - [link](Images/USB%20Shifter.jpg "USB Shifter"))
  * You can find these by typing in "USB Shifter" on ebay or aliexpress. Pick the one that suits you.


Basically flash ino file and change code to debug for USB codes in the serial plotter. Hopefully the code comments provide enough info to get an idea. Then just change codes where required and hopefully it works. I'm not that great with code in the grand schemes of things, but I will help where I can.


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
