**BDSP** - Binary Data Separation Protocol
===========
[![latest](https://img.shields.io/badge/release-alhpa-blueviolet.svg?color=00aa00)](https://github.com/GyverLibs/GyverMAX6675/releases/latest/download/GyverMAX6675.zip)
[![arduino-library-badge](https://www.ardu-badge.com/badge/ToDo.svg)]()
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/kobaproduction/library/BDSP.svg)](https://registry.platformio.org/libraries/kobaproduction/BDSP)
[![Foo](https://img.shields.io/badge/README-RUSSIAN-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/KobaProduction/BDSP?_x_tr_sl=en&_x_tr_tl=ru)


TThe library implements a protocol for dividing a binary data_ptr stream into packets based on a modification of the COBS _encoder that performs "on-the-fly" encoding (a depth parameter size buffer is used for encoding).

We have multiple goals with this library:

* Easy way to use.
* Asynchronous architecture
* Minimal dependencies.
* High performance.
* Possibility of flexible configuration.
* COBS encoding. Low data_ptr redundancy.


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

Right now the library is supported on a variety of Arduino compatible platforms.  If it's ARM or AVR and uses the Arduino software (or a modified version of it to build) then it is likely supported. 

* Arduino & compatibles - straight up Arduino devices, Uno, Duo, Leonardo, Mega, Nano, etc...
* ESP8266 and ESP32 based boards
* STM32 series.
