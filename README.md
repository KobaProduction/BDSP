**BDSP** - Binary Data Separation Protocol
===========
[![arduino-library-badge](https://www.ardu-badge.com/badge/BDSP.svg?)](https://www.ardu-badge.com/BDSP)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/kobaproduction/library/BDSP.svg)](https://registry.platformio.org/libraries/kobaproduction/BDSP)
[![Foo](https://img.shields.io/badge/README-RUSSIAN-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/KobaProduction/BDSP?_x_tr_sl=en&_x_tr_tl=ru)


The library implements a protocol for dividing a binary data stream into packets based on a modification of the COBS encoder that performs "on-the-fly" encoding (a depth parameter size buffer is used for encoding).

We have multiple goals with this library:
* Easy way to use;
* Asynchronous architecture;
* No dependencies;
* High performance;
* Possibility of flexible configuration;
* COBS encoding. Low data redundancy.

## BDSP Protocol Data Redundancy
![BDSP Protocol Data Redundancy](docs/attachments/images/bdsp-data-redundancy.webp)
Parameters 16, 32, 64, 128, 255 are the search depth parameter, which is equal to the allocated buffer for the COBS encoder, which is used at the base of the BDSP transmitter and transceiver objects.

Redundancy is formed due to the constant component of the packet: identifier (1 byte), data size (2 bytes), checksum (1 byte), as well as COBS encoding redundancy of at least 1 service byte and a separator byte. 
**In total, we get 6 bytes, excluding the data itself.** 

_Redundancy is indicated in the worst case, since in COBS encoding, service bytes can rarely be added, taking into account that a separator byte will often occur in the source data._


## Getting Started

Here's a simple program for Arduino:

```cpp
#include <Arduino.h>
#include <BDSP.h>

BDSPTransceiver transceiver;

void setup() {
    Serial.begin(115200);
    Serial.println();
    COBS::config_t config = {.delimiter = '\n', .depth = 255};
    transceiver.set_config(
            config,
            [] (uint8_t *data, size_t size, void *context) {
                Serial.write(data, size);
                Serial.flush();
            },
            [] (Packet &packet, void *context) {
                // Sending the packet back.
                transceiver.send_packet(packet);
            }
    );

    transceiver.set_error_handler([] (BDSP::receiver_error_t error, void *context) {
        auto &bdsp_transceiver= *static_cast<BDSPTransceiver*>(context);
        Packet error_packet(0, 1);
        error_packet.create_buffer();
        error_packet.data_ptr[0]=error;
        bdsp_transceiver.send_packet(error_packet);
    }, &transceiver);
}

void loop() {
    size_t available = Serial.available();
    if (available) {
        auto *buffer = new uint8_t[available];
        Serial.readBytes(buffer, available);
        transceiver.parse(buffer, available);
        delete[] buffer;
    }
}
```

## Supported Platforms

Due to the fact that the library does not use external dependencies, it can be used on any platform. However, the development emphasis is on embedded systems, including Arduino-compatible platforms. 
If the architecture is ARM or AVR and you are using Arduino or PlatformIO software to build, then it is most likely supported. 

For complete freedom from platforms and the ability to build in any environment, **help is needed** in redesigning the library structure and editing the cmake configuration. 

The functionality of the BDSP protocol was tested on the following boards (Arduino):
* Arduino & compatibles - straight up Arduino devices, Uno, Duo, Leonardo, Mega, Nano, etc...
* ESP8266 and ESP32 based boards
* STM32F103 series.
