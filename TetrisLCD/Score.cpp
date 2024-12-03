#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

class Score {
public:
unsigned long score = 0; //start the score at zero
unsigned short lines = 0; //start the line count at zero
unsigned short level = 1; //start the current level at one

Score(void){};

void countLines(uint8_t lineNum){
switch (lineNum){ //choose the score multiplier based on lines cleared
case 1: //one line cleared
score += (100ul * level); //increase score by 100 * the current level
break; //end case
case 2: //two lines cleared
score += (300ul * level); //increase score by 300 * the current level
break; //end case
case 3: //three lines cleared
score += (500ul * level); //increase score by 500 * the current level
break; //end case
case 4: //four lines cleared
score += (800ul * level); //increase score by 800 * the current level
break; //end case
}
lines += lineNum; //increase lines by the lines cleared
level = (lines / 10) + 1; //set the level to the number of lines cleared / 10 + 1
}

};