#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "debounce.cpp"


class userInput{
private:
debounce buttonLeft;
debounce buttonDown;
debounce buttonRight;
debounce buttonUp;
debounce pause;
bool rot = 0;
bool drop = 0;
bool paused = 0;
int horz = 0;

public:
  userInput(void) : buttonLeft(2), buttonDown(3), buttonRight(4), buttonUp(5), pause(7){
  };

  void begin(void){
    buttonLeft.pressed();
    buttonRight.pressed();
    buttonUp.pressed();
    buttonDown.held();
    pause.held();    
  }

  bool input(void){
    if (buttonLeft.pressed()) return 1;
    if (buttonRight.pressed()) return 1;
    if (buttonUp.pressed()) return 1;
    if (buttonDown.held()) return 1;
    return 0;
  }
  void update(void) {
    rot = 0;
    horz = 0;
    drop = 0;
    paused = 0;
    if (buttonLeft.pressed()) horz--;
    if (buttonRight.pressed()) horz++;
    if (buttonUp.pressed()) rot = 1;
    if (buttonDown.held()) drop = 1;
    if (pause.pressed()) paused = 1;
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