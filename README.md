# USBShifter2T300
Use a Generic USB Shifter to connect to the T300RS GT wheelbase to Emulating a TH8A shifter


Required
* Arduino Pro Mini 328 - 3.3V/8MHz (It has to be 3v3 volts to power off of the wheelbase easily)
* USB Hosts Shield mini (perfect for pro mini)
* USB Hosts Libary - https://github.com/felis/USB_Host_Shield_2.0#how-to-include-the-library
* PS2 plug (for plugging into the wheel base.)
* USB Shifter of course. I cant guarentee it will work on all usb shifters. (What I used - [link](http://example.com "USB Shifter"))
  * You can find these by typing in "USB Shifter" on ebay or aliexpress. Pick the one that suits you.




Basically flash ino file and change code to debug for USB codes in the serial plotter. Hopefully the code comments provide enough info to get an idea. Then just change codes where required and hopefully it works. Im not that great with code in the grand schemes of things, but I will help where I can.


Disclaimer
Look, this is really hastily put together. I really wasnt happy with the lack of documentation for my dumb brain to comprehend so I used a Shield example that got me 90% of the way there and then piggy backed off that.
Im 100% there is a better way of doing this that is cleaner but it doesnt indeed work. Happy for anyone to provide suggestions improvements and the such!

Included is a refernce folder for additional stuff downloaded from ISRTV.com. The information and .ino files are way to valuable to not have a backup somewhere...


Thanks to the people at ISRTV.com that did the heavy lifting. Their code was enough for me to adapt it here. Not ideal, but 



Resources/Reasearch
* USB Hosts Shield - https://chome.nerpa.tech/usb-host-shield-hardware-manual/
* USB Host Shield github info + examples - https://github.com/felis/USB_Host_Shield_2.0/
* Alot of the USB Shield Host code came from this example ino - https://github.com/felis/USB_Host_Shield_2.0/blob/master/examples/HID/t16km/t16km.ino#L55
* ISRTV forum where a lot of the info was from - https://www.isrtv.com/forums/topic/24532-gearbox-connector-on-base/
