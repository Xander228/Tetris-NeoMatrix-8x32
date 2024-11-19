#include <Arduino.h>


class debounce {


  int buttonPin;  // the number of the pushbutton pin
private:
  // Variables will change:
  bool buttonState;                        // the current reading from the input pin
  bool lastButtonState = LOW;              // the previous reading from the input pin
  unsigned long lastDebounceTime = 0;      // the last time the output pin was toggled
  const unsigned long debounceDelay = 10;  // the debounce time; increase if the output flickers
  bool reading;

public:
  debounce(int buttonPin) {
    this->buttonPin = buttonPin;
    pinMode(buttonPin, INPUT_PULLUP);
  };

  bool held(void) {
    reading = digitalRead(buttonPin);
    if (reading != lastButtonState) lastDebounceTime = millis();
    lastButtonState = reading;
    if ((millis() - lastDebounceTime) <= debounceDelay) return !buttonState;
    buttonState = reading;
    return !buttonState;
  }
  bool pressed(void) {
    reading = digitalRead(buttonPin);
    if (reading != lastButtonState) lastDebounceTime = millis();
    lastButtonState = reading;
    if ((millis() - lastDebounceTime) <= debounceDelay) return 0;
    if(lastButtonState == buttonState) return 0;
    buttonState = reading;
    if (!lastButtonState == 0) return 0;
    return 1;
  }
};
