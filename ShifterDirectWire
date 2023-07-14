# Wire shifter directly to Arduino (WIP)
This method involves opening up the shifter and identifying the pins and corresponding microswitches so we can directly wire them to the arduino.

## Requirements
* Arduino Pro Mini 328 - 3.3V/8MHz (It has to be 3v3 volts to power off of the wheelbase easily)
* Mini-Din/PS2 plug (for plugging into the wheelbase.)
* Dupont connector Male to Female (If you don't want to solder wires)
* Multimeter (to identify which gear is which pin)
* USB Shifter of course. I can't guarantee it will work on all usb shifters. (What I used - [link](Images/USB%20Shifter.jpg "USB Shifter"))
  * You can find these by typing in "USB Shifter" on ebay or aliexpress. Pick the one that suits you.

## Identifying pins
1. Disassemble the shifter. For my shifter it was using a small allen key undoing the 4 screws on each corner of the top. Don't forget to remove the ball on top
2. With a small amount of force, you can remove the top plastic piece upwards. The small resistance might be due to the pins connecting the USB microcontroller with the cable. (You Shifter may be different)
3. We can pull out the microcontroller from the PCB over the microswitches revealing empty pins.
4. Grab a multimeter and switch it to continuity mode and start probing and writing down which pins do what. Connect one probe to the ground and the other on another pin. Cycle through pressing the microswitches until you hear a beep. Write down which gear that was.
  * If you want to make life a bit easier, use a male to male dupont connector and connect the multimeter probes to the end of the cable.


## Connecting Wires & Modifying code
The code is currently configured to use the pins 2-8 for the shifter....


