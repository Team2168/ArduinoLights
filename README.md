ArduinoLights
=============
An arduino project to control a strip of RGB addressable LEDs from it's I/O pins.

###Hardware:
We use LPD8806 addressable LED strip, and the arduino libraries from: https://github.com/adafruit/LPD8806

Wire the strip and input pins per the comments in the source code. Note that floating (not tied HIGH or LOW) input pins will cause sporadic operation. 

###Configure the eclipse development environment:
* Move the LPD8806 library folder and its contents to your <arduinosketchfolder>/libraries/ folder, 
  if the libraries folder does not exist - create it first!
  e.g. C:\Program Files (x86)\Arduino\libraries
* Restart the IDE
