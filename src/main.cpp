#include <Arduino.h>
#include <BDSP.h>

BDSP bdsp(BDSP_SENDER);
bdsp_status_t status;

#define COUNT 255
uint8_t data[COUNT] = {0};

void setup() {
    Serial.begin(115200);
    Serial.println();

    status = bdsp.set_writer([] (uint8_t *d, size_t s) {Serial.write(d, s);});
    cobs_config_t config = {.delimiter = '\n', .depth = 255};
    status = bdsp.set_config(config);
}

void loop() {
    status = bdsp.send_data(1, data, COUNT);
    delay(5000);
}