#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

class points {
public:
unsigned short lines = 0;
unsigned short level = 1;




points(void){

};
unsigned long score = 0;


void countLines(uint8_t lineNum){
switch (lineNum){
case 1:
score += (100ul * level);
break;
case 2:
score += (300ul * level);
break;
case 3: 
score += (500ul * level);
break;
case 4:
score += (800ul * level);
break;
}
lines += lineNum;
level = (lines / 10) + 1;
}

};