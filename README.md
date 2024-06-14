**BDSP** - Binary Data Separation Protocol
===========
[![latest](https://img.shields.io/badge/release-alhpa-blueviolet.svg?color=00aa00)](https://github.com/GyverLibs/GyverMAX6675/releases/latest/download/GyverMAX6675.zip)
[![arduino-library-badge](https://www.ardu-badge.com/badge/ToDo.svg)]()
[![latest](https://img.shields.io/badge/PlatformIO-ToDo-blueviolet.svg?color=00aa00)]()
[![Foo](https://img.shields.io/badge/README-RUSSIAN-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/KobaProduction/BDSP?_x_tr_sl=en&_x_tr_tl=ru)


TThe library implements a protocol for dividing a binary data stream into packets based on a modification of the COBS encoder that performs "on-the-fly" encoding (a depth parameter size buffer is used for encoding).

We have multiple goals with this library:

* Easy way to use.
* Asynchronous architecture
* Minimal dependencies.
* High performance.
* Possibility of flexible configuration.
* COBS encoding. Low data redundancy.


## Getting Started

Here's a simple program for Arduino:

```cpp
#include <Arduino.h>
#include <bdsp_transceiver.h>

#define SIZE_DATA 255

BDSPTransceiver transceiver;

void setup() {
    Serial.begin(115200);
    Serial.println();
    cobs_config_t config = {.delimiter = '\n', .depth = 30};
    transceiver.set_config(
            config,
            [] (uint8_t *data, size_t size) {Serial.write(data, size); Serial.flush();},
            [] (Packet &packet, void *context) {
                Serial.print("Got packet. ID: ");
                Serial.println(packet.id);
            },
            nullptr
    );
}

void loop() {
    uint8_t data[SIZE_DATA] = {0};
    transceiver.send_data(1, data, SIZE_DATA);
    size_t available = Serial.available();
    if (available) {
        auto *buffer = new uint8_t[available];
        Serial.readBytes(buffer, available);
        transceiver.parse(buffer, available);
        delete [] buffer;
    }
    delay(5000);
}
```

## Supported Platforms

Right now the library is supported on a variety of Arduino compatible platforms.  If it's ARM or AVR and uses the Arduino software (or a modified version of it to build) then it is likely supported. 

* Arduino & compatibles - straight up Arduino devices, Uno, Duo, Leonardo, Mega, Nano, etc...
* ESP8266 and ESP32 based boards
* STM32 series.
