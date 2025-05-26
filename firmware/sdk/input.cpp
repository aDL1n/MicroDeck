#include "input.h"


float readAxis(uint8_t axis_pin) {
    int value = analogRead(axis_pin);
    float normalized = -2.0f * (4096 / 2 - value) / 4096;
    return (fabs(normalized) > 0.4) ? normalized : 0.0f;
}