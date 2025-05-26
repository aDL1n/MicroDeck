#include <cstdint>
#include <Arduino.h>

class JoyStick
{
#define JOYSTICK_DEADZONE 0.4f

private:
    uint32_t t_last = 0;
    static constexpr int STEPS = 4096;

public:
    JoyStick() = default;
    ~JoyStick() = default;

    float readAxis(uint8_t axis_pin);
};
