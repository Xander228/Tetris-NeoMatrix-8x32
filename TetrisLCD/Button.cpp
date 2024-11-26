#include <Arduino.h>

class Button {
  int buttonPin;  // the number of the pushbutton pin

private:
  bool buttonState;                        // the current reading from the input pin
  bool lastButtonState = LOW;              // the previous reading from the input pin
  unsigned long lastDebounceTime = 0;      // the last time the output pin was toggled
  const unsigned long debounceDelay = 10;  // the debounce time

public:
  Button(int buttonPin) {
    this->buttonPin = buttonPin; //assigns the global variable buttonPin to the value of the parameter buttonPin
    pinMode(buttonPin, INPUT_PULLUP); //assigns the mode of the buttonPin to input with pullup
  };

  void begin(void){
    bool reading = digitalRead(buttonPin); //set reading to the current state of the pin
    lastButtonState = reading; //set the previous known button state to the current state
    buttonState = reading; //sets the known state of the button to the current reading
  }

  bool isHeld(void) {
    bool reading = digitalRead(buttonPin); //set reading to the current state of the pin

    //if the new button state is different from the current one, set the last debounce time to the current time
    if (reading != lastButtonState) lastDebounceTime = millis(); 
    lastButtonState = reading; //set the previous state to the current state to reference the next time this function is called

    //if the time since the last reading is less than the minimum debounce time, return whether the button is pressed
    if ((millis() - lastDebounceTime) <= debounceDelay) return !buttonState;
    buttonState = reading; //otherwise set button state to the reading
    return !buttonState; //and return whether the button is pressed
  }
  
  bool wasPressed(void) {
    bool reading = digitalRead(buttonPin); //set reading to the current state of the pin

    //if the new button state is different from the current one, set the last debounce time to the current time
    if (reading != lastButtonState) lastDebounceTime = millis();
    lastButtonState = reading; //set the previous state to the current state to reference the next time this function is called

    //if the time since the last reading is less than the minimum debounce time, return false
    if ((millis() - lastDebounceTime) <= debounceDelay) return 0;

    //if the current button state is equal to the previous button state, also return false
    if(lastButtonState == buttonState) return 0;
    buttonState = reading; //otherwise set button state to the reading
    if (!lastButtonState == 0) return 0; //and return false if the button is in a released state
    return 1; //return true if the button is pressed and the function has not returned true already
  }
};