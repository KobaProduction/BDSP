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