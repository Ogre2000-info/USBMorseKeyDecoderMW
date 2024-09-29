]# USB Morse Decoder for straight key only.
## Just for fun and learning

by
Marek Wągrodzki - Ogre2000

# Description
It is a Staight CW Morse Decoder on Seeeduino XIAO board.
I've designed small box to connect USB, Jack and buzzer.

#Project Parts
1. Seeeduino XIAO SAMD21 - https://www.seeedstudio.com/Seeeduino-XIAO-Arduino-Microcontroller-SAMD21-Cortex-M0+-p-4426.html
2. Stereo Mini (3.5mm) Jack socket (female), for 9mm hole panel mount.
3. Passive Buzzer, 9mm.
4. 3D printed Box.

#Installation issues
- On older Windows OS (eg. Win 8.1), there is additional Serial driver needed.
- Before upload code on chip, it's need to be "reset twice" using "no hole reset" pins on boad, next to USB-C.

- On Windows 11 - no additional drivers installation needed, no change mode needed.
