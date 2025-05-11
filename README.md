# AstroUSB
Firmware for the hacked Orico's USB 7-port Switch Hub.

With this project, I designed a modification to Orico's circuit so as to be able to turn individual ports from the computer.

![astrousbc](https://github.com/user-attachments/assets/37e5ce10-f2d4-4e20-b748-5764c6684ad1)

## External Libraries
No external libraries are required.

Arduino IDE is suggested for development, because it includes all required tooling to upload code to the microcontroller.

## Why Arduino Nano and not Raspberry Pi Pico?
Due to bad design choices PCB had to be improvised after it had already arrived.
If you order P-channel MOSFETs with a high enough gate threshold difference, it shouldn't be a problem to run it on Pi Pico, but I didn't. Therefore I needed a microcontroller with 5V GPIO pins.
Honestly, Arduino has given me more headaches than it'd been worth to just reorder appropriate MOSFETs. Just use Pi Pico in newer designs.

## ASCOM Driver
AstroUSB comes with an ASCOM driver, which communicates over the Universal Serial Bus with the device. Driver is written in C# and is available for download [here](https://github.com/mytja/AstroUSB-ASCOM/releases).

## INDI Driver
INDI driver is not planned for now.

## License
Mozilla Public License 2.0
