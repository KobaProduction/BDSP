**BDSP** - Binary Data Separation Protocol
===========
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/kobaproduction/library/BDSP.svg)](https://registry.platformio.org/libraries/kobaproduction/BDSP)
[![Russian Translate](https://img.shields.io/badge/README-RUSSIAN_TRANLATE-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/KobaProduction/BDSP?_x_tr_sl=en&_x_tr_tl=ru)


The library implements a protocol for dividing a binary data stream into packets based on a modification of the COBS encoder that performs "on-the-fly" encoding (a depth parameter size buffer is used for encoding).

This README is not relevant. Arduino IDE not supported. #todo add instruction for build library for Arduino IDE.

We have multiple goals with this library:
* Easy way to use;
* Asynchronous architecture;
* No dependencies;
* High performance;
* Possibility of flexible configuration;
* Low and predictable data redundancy based on the COBS encoder.

## BDSP (COBS) Protocol Data Redundancy
![BDSP (COBS) Protocol Data Redundancy](docs/attachments/images/bdsp-data-redundancy.webp)
Parameters 16, 32, 64, 128, 255 are the search depth parameter, which is equal to the allocated buffer for the COBS encoder, which is used at the base of the BDSP transmitter and transceiver objects.

Redundancy is formed due to the constant component of the packet: identifier (1 byte), data size (2 bytes), checksum (1 byte), as well as COBS encoding redundancy of at least 1 service byte and a separator byte. 
**In total, we get 6 bytes, excluding the data itself.** 

_Redundancy is indicated in the worst case, since in COBS encoding, service bytes can rarely be added, taking into account that a separator byte will often occur in the source data._


## Getting Started

Here's a simple program for Platformio (Arduino Framework):

```cpp
#todo
```

## Supported Platforms

Due to the fact that the library does not use external dependencies, it can be used on any platform. However, the development emphasis is on embedded systems, including Arduino-compatible platforms. 
If the architecture is ARM or AVR and you are using Arduino or PlatformIO software to build, then it is most likely supported. 

For complete freedom from platforms and the ability to build in any environment, **help is needed** in redesigning the library structure and editing the cmake configuration. 

The functionality of the BDSP protocol was tested on the following boards (Framework Arduino):
* Arduino & compatibles - straight up Arduino devices, Uno, Duo, Leonardo, Mega, Nano, etc...;
* ESP8266 and ESP32 based boards;
* STM32 series.


# Package Build

Thx `pananton` for https://github.com/pananton/cpp-lib-template

ToDo: Write instruction on install BDSP

# Coverage

[Instructions on how to set up an environment for assessing code coverage.](docs/coverage.md)

ToDo: add coverage status