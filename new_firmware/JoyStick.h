class JoystickInput
{
private:
  int xPin, yPin;
  int threshold = 500;

public:
  JoystickInput(int x, int y) : xPin(x), yPin(y) {}

  int readY()
  {
    int yVal = analogRead(yPin);

    if (yVal < 2048 - threshold)
    {
      return -1;
    }
    if (yVal > 2048 + threshold)
    {
      return 1;
    }
    return 0;
  }

  int readX()
  {
    int xVal = analogRead(yPin);

    if (xVal < 2048 - threshold)
    {
      return -1;
    }
    if (xVal > 2048 + threshold)
    {
      return 1;
    }
    return 0;
  }

};