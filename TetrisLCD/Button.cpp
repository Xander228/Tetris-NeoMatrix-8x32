#include <Arduino.h>


class Button {

public:
int buttonPin;  // the number of the pushbutton pin

private:
bool buttonState;                        // the current reading from the input pin
bool lastState; // the previous reading from the input pin

public:
  Button(int buttonPin) {
    this->buttonPin = buttonPin; //set global variable buttonPin to the parameter buttonPin
    pinMode(buttonPin, INPUT_PULLUP); //initialize buttonPin as an input with pullup
  };

  void begin(void){

  }


  bool isHeld(void) {
    buttonState = !digitalRead(buttonPin);
    return buttonState; 

    // reading = digitalRead(buttonPin); //get the state of the button
    // if (reading != lastButtonState) lastDebounceTime = millis(); 
    // lastButtonState = reading;
    // if ((millis() - lastDebounceTime) <= debounceDelay) return !buttonState;
    // buttonState = reading;
    // return !buttonState;
  }
  bool wasPressed(void) {
    buttonState = !digitalRead(buttonPin);
    if(!lastState && buttonState){
      lastState = true;
      return true;
    }
    lastState = buttonState;
    return false;

    // reading = digitalRead(buttonPin);
    // if (reading != lastButtonState) lastDebounceTime = millis();
    // lastButtonState = reading;
    // if ((millis() - lastDebounceTime) <= debounceDelay) return 0;
    // if(lastButtonState == buttonState) return 0;
    // buttonState = reading;
    // if (!lastButtonState == 0) return 0;
    // return 1;
  }
};
