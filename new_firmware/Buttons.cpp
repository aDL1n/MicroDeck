#include "Buttons.h"

bool ButtonsInput::readA() { 
  int sensorValue = analogRead(A);

  bool currentState = (sensorValue > 4090) ? HIGH : LOW;

  if (lastA == HIGH && currentState == LOW) {
    delay(50);
    bool debouncedState = (analogRead(A) > 4090) ? HIGH : LOW;
    if (debouncedState == LOW) {
      lastA = debouncedState;
      return true;
    }
  }
  lastA = currentState;
  return false;
}

bool ButtonsInput::readB() { 
  int sensorValue = analogRead(B);

  bool currentState = (sensorValue > 4090) ? HIGH : LOW;

  if (lastB == HIGH && currentState == LOW) {
    delay(50);
    bool debouncedState = (analogRead(B) > 4090) ? HIGH : LOW;
    if (debouncedState == LOW) {
      lastB = debouncedState;
      return true;
    }
  }
  lastB = currentState;
  return false;
}

bool ButtonsInput::readX() { 
  int sensorValue = analogRead(X);

  bool currentState = (sensorValue > 4090) ? HIGH : LOW;

  if (lastX == HIGH && currentState == LOW) {
    delay(50);
    bool debouncedState = (analogRead(X) > 4090) ? HIGH : LOW;
    if (debouncedState == LOW) {
      lastX = debouncedState;
      return true;
    }
  }
  lastX = currentState;
  return false;
}

bool ButtonsInput::readY() { 
  int sensorValue = analogRead(Y);

  bool currentState = (sensorValue > 4090) ? HIGH : LOW;

  if (lastY == HIGH && currentState == LOW) {
    delay(50);
    bool debouncedState = (analogRead(Y) > 4090) ? HIGH : LOW;
    if (debouncedState == LOW) {
      lastY = debouncedState;
      return true;
    }
  }
  lastY = currentState;
  return false;
}