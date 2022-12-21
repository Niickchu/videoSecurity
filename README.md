# videoSecurity

## What is this? ##
This is a real-time security system implemented on a microcontroller (BeagleBone Green). The system is able to perform video processing, facial detection and facial recognition in real time and, by means of two servo motors and an LED matrix, let the user know if it recognized the person in the frame or not.

## System Components ##
**1\. USB Camera:** The USB camera is used to capture images, where facial detection and facial recognition using openCV 4.6.0 is run on these images in real time.

**2\. Servo:**  The servo works along with the camera, where the servo will move to point the LED to where your face is. The servo will also shake its ‘head’ horizontally if you are not recognized, and nod its ‘head’ vertically if you are recognized.

**3\. LED Matrix:** The RGB 8x8 LED matrix displays multiple images for the user including: A smiley face, a frowney face, a loading icon, a checkmark, and the position/size of the users head as seen by the camera. It runs on a bare-metal programmable real-time unit (PRU) due to the very critical hard real-time nature of this device.

**4\. Distance Sensor:**  The ultrasonic motion detector identifies an object approaching the system and turns the camera when the object is within 60cm of the sensor. This peripheral also runs on a PRU.

### Video Demos ###

[Final Demo](https://www.youtube.com/shorts/5ZF8nbTvUDc)    
[Old initial proof of concept](https://www.youtube.com/shorts/GQ6ElP9bKJY)
