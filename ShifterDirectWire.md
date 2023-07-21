# Wire shifter directly to Arduino
This method involves opening up the shifter and identifying the pins and corresponding microswitches so we can directly wire them to the arduino.

## Requirements
* Arduino Pro Mini 328 - 3.3V/8MHz (It has to be 3v3 volts to power off of the wheelbase easily)
* USB to TTL Serial **that supports 3v3 programing**  for flashing ino ([HardwireShifter2PS2.ino](/HardwireShifter2PS2/HardwireShifter2PS2.ino "HardwireShifter2PS2.ino"))
* Mini-Din/PS2 plug (for plugging into the wheelbase.)
* Dupont connector Male to Female (If you don't want to solder wires)
* Multimeter (to identify which gear is which pin)
* USB Shifter of course. I can't guarantee it will work on all usb shifters. (What I used - [link](Images/USB%20Shifter.jpg "USB Shifter"))
  * You can find these by typing in "USB Shifter" on ebay or aliexpress. Pick the one that suits you.

## Identifying pins
1. Disassemble the shifter. For my shifter it was using a small allen key undoing the 4 screws on each corner of the top. Don't forget to remove the ball on top
2. With a small amount of force, you can remove the top plastic piece upwards. The small resistance might be due to the pins connecting the USB microcontroller with the cable. (You Shifter may be different)
3. We can pull out the microcontroller from the PCB revealing empty pins.
4. Grab a multimeter and switch it to continuity mode and start probing and writing down which pins do what.
5. First identify the ground pin(s). You can use the below image as a start. 
6. Connect one probe to the ground and the other on another pin. Cycle through pressing the microswitches until you hear a beep. Write down which gear that was. If you want to make life a bit easier, use a male to male dupont connector and connect the multimeter probes to the end of the cable.

Below is my shifters pinout. Yours may vary.

![Pinout of Shifter PCB 6+R+switch Ver](https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase/assets/40465008/bc0d2ea4-aaf2-4b70-ab95-aa42036b9a00)

## Connecting Wires & Modifying code
The code shouldnt require any intervention or changes as long as you connect the pins switches to the correct arduino pins. Refer to the diagram below for connecting the pins from the shifter to the Arduino and what pins to connect the PS2 plug to. The common ground of the shifter pins should be connected to a ground on the Arduino.

### Wiring Diagram

![Hardwire Shifter Wiring DiagramL](https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase/assets/40465008/d3780173-6143-4a0a-9bc5-9cf9eb9b1583)


### Connection to PC before flashing
Just make sure the programmer is set to 3v3 and not 5v as you may damage the arduino & shield since it only supports 3v3 without modification. Connect up the programmers 3v3, Gnd, TX & RX to the pro mini with the shield on the bottom. And flash away. We will be using the connection for debugging.

That should be it, you can test out if it worked by pressing the switches and looking at the serial monitor to see if its the right switch.
