# retro-joystick-adapter
Arduino (Leonardo or Pro Micro) code to connect a retro [gameport](https://en.wikipedia.org/wiki/Game_port) joystick with a [DA-15 plug](https://en.wikipedia.org/wiki/D-subminiature#Game_controller_ports) to USB

This sketch uses the joystick library [ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary)
to simulate a
USB joystick. It is tested with an Arduino Pro Micro and a Wingman Extreme
[from around 1995](https://en.wikipedia.org/wiki/List_of_Logitech_products#Joysticks) analog joystick.

Because the USB manufacturer and model are subject to a USB registration the simulated joystick will appear as the Arduino board instead of a joystick model.

The connections assumed in this sketch are:
Arduino board | Game Port Pin (optional pins) | other connections
--------------|-------------------------------|------------------
VCC | 1 (8, 9, 15)
GND | 4 (5, 12)
D2 | 2
D3 | 7
D4 | 10
D5 | 14
D7 | | via 220R to blue 3mm LED
A1 | 13 | via 4.7k to GND
A2 | 3 | via 68k to GND
A3 | 6 | via 68k to GND

The analog inputs A1, A2 and A3 are pulled down to GND, the digital pins use
the board pulldown feature. For maximum compatibility the pins 5 and 12 should be connected to GND as well. For the Wingman Extreme this is unnecessary.
According to the game port layout there are three more pins for +5V: 8, 9 and 15. Again this was unnecessary for the joystick I tested.
On pin D7 a LED lights up when a joystick is found.

