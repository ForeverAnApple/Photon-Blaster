# Photon Blaster!
-----

## List of Components
* 1 x Photon
* 12v RGB LED strip (SMD5050)
* 3 x 10k Ohm Resistors
* 3 x Logic Level N-channel MOSFETs
* 1 x Breadboard
* Hookup wires
* 12v Power Supply - 8xAA Battery Pack
* 4.5v Power Supply - 3xAA Battery Pack
* Sparkfun MMA8452Q Accelerometer
* PBC Pipe (The Blaster Body)

## Schematics

### LEDs

Here's our basic schematic of the SMD5050.
![Schematic of LED](References/Schematic.jpg)

Schematic of MMA8452Q Accelerometer
![Schematic of Accelerometer](References/AccelerometerSchematic0.png)

## Functionalities

### States
The blaster itself runs differently depending on the state that it is in. States are changed due accelerometer detected position changes. In this implementation, the 3 main states are:

* Inactive
  - Breathes red slowly. Does nothing.

* Idle
  - Ability to shoot. Breathes normally.

* Receiving
  - Receives IoT Functionalities such as changing the fade color during idle mode, rainbow, and strobe.

## Progress

### Initial Setup

#### Here we have our Photon Device, LEDs, and the Gauntlet prototype itself.
![progress0](References/Progress0.jpg)

#### First iteration:
![First LED Setup](References/Blaster/LEDSchematic0.jpg)

#### Second iteration:
![Second LED Setup](References/Blaster/LEDSchematic1.jpg)

#### It works!
![LEDGif0](References/Blaster/LEDProgress0.gif) ![LEDGif1](References/Blaster/LEDProgress1.gif)

#### Finalized Circuit
![Circuit0](References/Blaster/FullSchematic2.jpg)

#### Finalized Blaster
![Blaster0](References/Blaster/FullBlaster0.jpg)
![Blaster1](References/Blaster/Blaster0.gif)
