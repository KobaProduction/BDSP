#include <Arduino.h>
#include <bdsp_transceiver.h>

BDSPTransceiver transceiver;

void setup() {
    Serial.begin(115200);
    Serial.println();
    cobs_config_t config = {.delimiter = '\n', .depth = 255};
    transceiver.set_config(
            config,
            [] (uint8_t *data, size_t size, void *context) {
                Serial.write(data, size);
                Serial.flush();
            },
            [] (Packet &packet, void *context) {
//                Serial.print("Got packet. ID: ");
//                Serial.println(packet.id);
                packet.id += 1;
                for (uint16_t i = 0; i < packet.size; ++i) packet.data_ptr[i] = i;
                transceiver.send_packet(packet);
            },
            nullptr,
            nullptr
    );
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