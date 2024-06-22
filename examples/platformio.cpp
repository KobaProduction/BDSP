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