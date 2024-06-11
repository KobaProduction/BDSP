#ifndef BDSP_UTILS_H
#define BDSP_UTILS_H

#include <Arduino.h>

void print_status(const char str[], void *status) {
    Serial.print(str);
    Serial.println(*reinterpret_cast<uint8_t*>(status));
}

#endif //BDSP_UTILS_H
