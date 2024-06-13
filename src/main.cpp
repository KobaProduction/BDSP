#include <Arduino.h>

#include <bdsp_transmitter.h>

BDSPTransmitter transmitter;

#define COUNT 255
uint8_t data[COUNT] = {0};

void setup() {
    Serial.begin(115200);
    Serial.println();
    cobs_config_t config = {.delimiter = '\n', .depth = 255};

    transmitter.set_config(config, [] (uint8_t *data, size_t size) {Serial.write(data, size);});
}

void loop() {
    transmitter.send_data(1, data, COUNT);
    delay(5000);
}