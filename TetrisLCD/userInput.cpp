#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "Button.cpp"


class UserInput{
public:
Button buttonLeft; //creates a new button object
Button buttonDown; //creates a new button object
Button buttonRight; //creates a new button object
Button buttonUp; //creates a new button object

private:
bool rot = 0; //creates a boolean to represent the rotation button's state
bool drop = 0; //creates a boolean to represent the drop button's state
int horz = 0; //creates a boolean to represent the horizontal button's state

public:
  UserInput(void) : buttonLeft(19), buttonDown(21), buttonRight(18), buttonUp(20){
  }; //contruct each button object with the coresponding input pins

  void begin(void){
    buttonLeft.begin(); //setup left button
    buttonRight.begin(); //setup right button
    buttonUp.begin(); //setup up button
    buttonDown.begin(); //setup down button
  }

  bool input(void){
    //return whether any button was pressed
    return buttonLeft.wasPressed() || buttonRight.wasPressed() || buttonUp.wasPressed() || buttonDown.isHeld();
  }
  void update(void) {
    rot = buttonUp.wasPressed(); //set rotation button state
    drop = buttonDown.isHeld(); //set drop button state
    horz = buttonRight.wasPressed() ? 1 : buttonLeft.wasPressed() ? -1 : 0; //set horizontal button state
  }

  int horizontal(void){
    return horz; //return the horizontal button state
  }
  bool rotation(void){
    return rot; //return the rotation button state
  }
  bool droping(void){
    return drop; //return the dropping button state
  }
};