#include "./utils.h"

void print_status(const char str[], void *status) {
    Serial.print(str);
    Serial.println(*reinterpret_cast<uint8_t*>(status));
}
