#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "Button.cpp"


class UserInput{
public:
Button buttonLeft;
Button buttonDown;
Button buttonRight;
Button buttonUp;

private:
bool rot = 0;
bool drop = 0;
bool paused = 0;
int horz = 0;

public:
  UserInput(void) : buttonLeft(19), buttonDown(21), buttonRight(18), buttonUp(20){
  };

  void begin(void){
    buttonLeft.begin();
    buttonRight.begin();
    buttonUp.begin();
    buttonDown.begin();
  }

  bool input(void){
    if (buttonLeft.wasPressed()) return 1;
    if (buttonRight.wasPressed()) return 1;
    if (buttonUp.wasPressed()) return 1;
    if (buttonDown.isHeld()) return 1;
    return 0;
  }
  void update(void) {
    rot = 0;
    horz = 0;
    drop = 0;
    paused = 0;
    if (buttonLeft.wasPressed()) horz--;
    if (buttonRight.wasPressed()) horz++;
    if (buttonUp.wasPressed()) rot = 1;
    if (buttonDown.isHeld()) drop = 1;
    //if (pause.wasPressed()) paused = 1;
  }

  int horizontal(void){
    return horz;
  }
  bool rotation(void){
    return rot;
  }
  bool droping(void){
    return drop;
  }
  bool pausing(void){
    return paused;
  }
};