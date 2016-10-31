# VellemanK8056library

This library is fork of [AlbanT](https://github.com/AlbanT/VellemanK8056library) Arduino library for controlling a Velleman K8056 relayboard using a serial connection.


## The wiring to Arduino

Tested on controllers: MEGA2560, Uno, Leonardo, Micro, ESP8266.

MEGA accept up to 4 serial ports, serial port is defined with begin() method e.g. `Relay.begin(3);`. On other controllers it will use Serial0 by default.

**IMPORTANT !!!!!!!**  
**IMPORTANT !!!!!!!**  
**IMPORTANT !!!!!!!**

Velleman K8056 board require inverted signal to work properly. You can fix this by using software serial (not implemented jet) or HW breakout circuit [SparkFun Transceiver Breakout - MAX3232](https://www.sparkfun.com/products/11189).

![image of connection](http://reptile-addict.nl/wp-content/gallery/velleman-k8056-arduino-library/Untitled-Sketch_bb.png)

###Thanks to
[A.S. Tilanus](http://reptile-addict.nl/?portfolio=velleman-k8056-arduino-library) for making initial version of this library.
