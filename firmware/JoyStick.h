class JoystickInput {
private:
  int xPin, yPin;
  int threshold = 1000;
  int xCenter = 2048;
  int yCenter = 2048;

public:
  JoystickInput(int x, int y)
    : xPin(x), yPin(y) {}

  int readY() {
    int rawValue = analogRead(yPin);
    int deviation = rawValue - yCenter;

    if (deviation < -threshold) {
      return -1;
    }
    if (deviation > threshold) {
      return 1;
    }
    return 0;
  }

  int readX() {
    int rawValue = analogRead(xPin);
    int deviation = rawValue - xCenter;

    if (deviation < -threshold) {
      return -1;
    }
    if (deviation > threshold) {
      return 1;
    }
    return 0;
  }
};