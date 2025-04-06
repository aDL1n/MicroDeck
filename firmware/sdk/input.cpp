#include "../config.h"
#include <cstdint>
#include <Arduino.h>

struct JoyStick {
    bool button() {
      static uint32_t t_last = 0;
      if (!digitalRead(PIN_SW) && (millis() - t_last) > 1000) {
        t_last = millis();
        return true;
      }
      return false;
    }
  
    float readAxis(int pin) {
      const int STEPS = 3968;
      int value = analogRead(pin);
      float normalized = -2.0f * (STEPS / 2 - value) / STEPS;
      return (fabs(normalized) > JOYSTICK_DEADZONE) ? normalized : 0.0f;
    }
  };