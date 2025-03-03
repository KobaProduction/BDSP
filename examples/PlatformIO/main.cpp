#include <Arduino.h>
#include <BDSP/transceivers.h>

BDSP::COBSZPETransceiver transceiver;

#define SAMPLES_SIZE 255

#if not defined(ANALOG_PIN) and defined(ESP32)
#    define ANALOG_PIN 0
#endif

uint16_t samples[SAMPLES_SIZE];

void setup() {
    Serial.begin(115200);
    Serial.println();

    auto config = transceiver.get_write_stream().get_strategy().get_config();
    config.delimiter_byte = '\n';
    transceiver.get_write_stream().get_strategy().set_config(config);
    transceiver.get_read_stream().get_strategy().set_config(config);

    transceiver.get_write_stream().set_stream_writer(
        [](uint8_t byte, void *ctx) {
            Serial.write(byte);
            Serial.flush();
        },
        nullptr);

    transceiver.set_packet_handler(
        [](BDSP::packet_context_t &packet_ctx, void *ctx) {
            reinterpret_cast<BDSP::ITransmitter *>(ctx)->send_packet(packet_ctx);
        },
        &transceiver);

    transceiver.set_error_handler(
        [](BDSP::parse_packet_status_t status, void *ctx) {
            Serial.print(F("error - BDSP::parse_packet_status_t code: "));
            Serial.println(uint32_t(status));
        },
        &transceiver);
}

void loop() {
    size_t available = Serial.available();
    if (available) {
        auto *buffer = new uint8_t[available];
        Serial.readBytes(buffer, available);
        transceiver.get_read_stream().read(buffer, available);
        delete[] buffer;
    }

    static uint32_t timer;
    if (millis() - timer < 1000)
        return;
    timer = millis();

    for (uint16_t &sample: samples) {
        sample = analogRead(ANALOG_PIN);
    }

    transceiver.send_data(1, reinterpret_cast<uint8_t *>(samples), SAMPLES_SIZE * sizeof(*samples));
}