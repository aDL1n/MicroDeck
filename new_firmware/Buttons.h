#include "Arduino.h"

#define X 35
#define Y 33
#define A 32
#define B 34

class ButtonsInput {
  private:
    bool lastX = LOW;
    bool lastY = LOW;
    bool lastA = LOW;
    bool lastB = LOW;

  public:
    ButtonsInput() = default;

    bool readX();
    bool readY();
    bool readA();
    bool readB();
};