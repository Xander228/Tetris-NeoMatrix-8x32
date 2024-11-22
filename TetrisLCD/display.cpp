#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#ifndef TETROMINOS_H
#define TETROMINOS_H
#include "Tetrominos.cpp"
#endif
#define MATRIX_PIN 6 //matrix pin

class Display {
private:
  const uint16_t dim = 4; //dim factor
  const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8; //LCD pins
  int x; //width
  Adafruit_NeoMatrix matrix; //create neoMatrix object
  LiquidCrystal lcd; //creat LCD object

  //piece colors
  uint16_t colors[12] = {
    matrix.Color(0, 0, 0),        //black
    matrix.Color(0, 255, 255),    //cyan
    matrix.Color(0, 0, 255),      //blue
    matrix.Color(255, 127, 0),    //orange
    matrix.Color(255, 255, 0),    //yellow
    matrix.Color(0, 255, 0),      //green
    matrix.Color(255, 0, 255),    //purple
    matrix.Color(255, 0, 0),      //red
    matrix.Color(255, 255, 255),  //white
  };

  //ghost piece colors
  uint16_t colorsGhost[12] = {
    matrix.Color(0 / dim, 0 / dim, 0 / dim),      //dim black
    matrix.Color(0 / dim, 255 / dim, 255 / dim),  //dim cyan 
    matrix.Color(0 / dim, 0 / dim, 255 / dim),    //dim blue
    matrix.Color(255 / dim, 127 / dim, 0 / dim),  //dim orange
    matrix.Color(255 / dim, 255 / dim, 0 / dim),  //dim yellow
    matrix.Color(0 / dim, 255 / dim, 0 / dim),    //dim green
    matrix.Color(255 / dim, 0 / dim, 255 / dim),  //dim purple
    matrix.Color(255 / dim, 0 / dim, 0 / dim),    //dim red
    matrix.Color(255 / dim, 255 / dim, 255 / dim) //dim white
  };

  //logo colors
  uint16_t colorsLogo[15] = {
    matrix.Color(0, 0, 0), matrix.Color(26, 66, 147), 
    matrix.Color(3, 14, 50), matrix.Color(240, 30, 40), 
    matrix.Color(255, 0, 0), matrix.Color(245, 140, 35), 
    matrix.Color(240, 75, 35), matrix.Color(250, 225, 35), 
    matrix.Color(250, 160, 30), matrix.Color(122, 255, 65), 
    matrix.Color(18, 129, 15), matrix.Color(0, 226, 245), 
    matrix.Color(7, 111, 231), matrix.Color(255, 56, 255), 
    matrix.Color(113, 3, 83)
  };

public:
  Display(void)
  //setup matrix 
  : matrix(8, 32, MATRIX_PIN,
    NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
    NEO_GRB + NEO_KHZ800),
  //setup LCD
  lcd(rs, en, d4, d5, d6, d7) {
    matrix.begin(); //initialize matrix
    lcd.begin(16, 2); //initialize lcd
    matrix.setTextWrap(false); //set text wrap mode to false for the matrix
    matrix.setBrightness(50); //set beightness to 50/255 for the matrix
    lcd.clear(); //clears the lcd
    int x = matrix.width(); //sets x to the width of the matrix
  };

  //displays the piece
  void piece(uint8_t pieceNum, uint8_t pieceRotation, int pieceX, int pieceY) {
    //iterates through the y axis
    for (uint8_t Y = 0; Y < 4; Y++) {
      //iterates through the x axis
      for (uint8_t X = 0; X < 4; X++) {
        //if the cell is empty skip writing to matrix
        if (colors[pgm_read_byte(&tetrominos[pieceNum][Y][pieceRotation][X])] == 0) continue; 

        //write the piece color to the board at the given cell
        matrix.drawPixel(X + pieceX, Y + pieceY, colors[pgm_read_byte(&tetrominos[pieceNum][Y][pieceRotation][X])]);
      }
    }
  }

  //display the board
  void board(uint8_t game[32][8]) {
    //iterates through the y axis
    for (uint8_t Y = 0; Y < 32; Y++) {
      //iterates through the x axis
      for (uint8_t X = 0; X < 8; X++) {
        //if the cell is empty skip writing to matrix
        if (colors[game[Y][X]] == 0) continue;

        //write the cell color to the board 
        matrix.drawPixel(X, Y, colors[game[Y][X]]);
      }
    }
  }

  //display the next piece in the queue
  void next(uint8_t pieceNum) {
    matrix.fillRect(0, 0, 8, 6, colors[0]); //clear the top section of the matrix
    matrix.fillRect(0, 6, 8, 1, colors[8]); //draw the dividing line
    //iterates through the y axis
    for (uint8_t Y = 0; Y < 4; Y++) {
      //iterates through the x axis
      for (uint8_t X = 0; X < 4; X++) {
        //if the cell is empty skip writing to matrix
        if (colors[pgm_read_byte(&tetrominos[pieceNum][Y][0][X])] == 0) continue;

        //if the piece is the square, apply offset to center it
        if (pieceNum == 3) {
          //draw the piece
          matrix.fillRect(2 * X, 2 * Y - 1, 2, 2, colors[pgm_read_byte(&tetrominos[pieceNum][Y][0][X])]);
          continue; //skip the rest of the code in the for loop
        }

        //if the piece is the line, apply an offset to center it
        if (pieceNum == 0) {
          //draw the cell
          matrix.fillRect(2 * X, 2 * Y, 2, 2, colors[pgm_read_byte(&tetrominos[pieceNum][Y][0][X])]);
          continue; //skip the rest of the code in the for loop
        }

        //draw the cell (no offset)
        matrix.fillRect(2 * X + 1, 2 * Y - 1, 2, 2, colors[pgm_read_byte(&tetrominos[pieceNum][Y][0][X])]);
      }
    }
  }

  //display the ghost piece
  void ghost(uint8_t pieceNum, uint8_t pieceRot, int pieceX, int pieceY, uint8_t board[32][8]) {
    bool above = false; //sets the boolean to check if the piece is above another piece to false

    //iterates the piece down the y axis of the board
    for (uint8_t changeY = pieceY; changeY < 32; changeY++) {
      //iterates through the y axis of the piece
      for (uint8_t Y = 0; Y < 4; Y++) {
        //iterates through the x axis of the piece
        for (uint8_t X = 0; X < 4; X++) {
          //if the cell is empty, skip it
          if (pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X]) == 0) continue;

          //if the piece is above a solid cell in the board
          if (board[changeY + Y + 1][pieceX + X] != 0 || changeY + Y > 30) {
            above = true; //set above to true
            break; //break out of the inner for loop
          }
        }
        if (above) break; //if the piece is above a solid cell, break out of the middle for loop
      }

      //if the piece is above a solid cell
      if (above) {
        //iterates through the y axis of the piece
        for (uint8_t Y = 0; Y < 4; Y++) {
          //iterates through the x axis of the piece
          for (uint8_t X = 0; X < 4; X++) {
            //if the cell is empty, skip it
            if (colors[pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X])] == 0) continue;

            //draw the ghost cell
            matrix.drawPixel(X + pieceX, Y + changeY, colorsGhost[pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X])]);
          }
        }
        break; //break from the outermost for loop
      }
    }
  }

  //wrap the matrix.clear method
  void clear() {
    matrix.clear(); //clear the matrix
  }

  //wrap the matrix.show method
  void show() {
    matrix.show(); //show the current matrix buffer
  }

  //display the score on the LCD
  void score(unsigned long score, unsigned short lines, unsigned short level) {
    lcd.setCursor(0, 0);  //set cursor to upper left
    lcd.print("Score: "); //write "Score: " to the lcd
    lcd.print(score);     //write the current score to the lcd
    lcd.setCursor(0, 1);  //set cursor to bottom left
    lcd.print("Lvl: ");   //write "Lvl: " to the lcd
    lcd.print(level);     //write the current level to the lcd
    lcd.setCursor(8, 1);  //set cursor to bottom middle
    lcd.print("Line: ");  //write ("Line: " to the lcd
    lcd.print(lines);     //write the current cleared lines to the lcd
  }

  //display the wait screen
  void idle(void) {
    matrix.clear(); //clear the display
    matrix.setCursor(x, 0); //
    for (uint8_t Y = 0; Y < 13; Y++) {
      for (uint8_t X = 0; X < 19; X++) {
        if (pgm_read_byte(&logo[Y][X]) == 0) continue;
        matrix.drawPixel(X + x, Y + 4, colorsLogo[pgm_read_byte(&logo[Y][X])]);
      }
    }
    if (--x < -20) x = matrix.width();
    matrix.show();
    delay(100);
  }

  bool paused = 0;
  void pause(void){//bool button, uint8_t game[32][8], uint8_t pieceNum) {
    //if (button == 1 && paused == 0) {
      matrix.setTextColor(colors[8]);
      matrix.clear();
      matrix.setCursor(1, 0);
      matrix.print("P");
      matrix.setCursor(1, 6);
      matrix.print("a");
      matrix.setCursor(1, 12);
      matrix.print("u");
      matrix.setCursor(1, 18);
      matrix.print("s");
      matrix.setCursor(1, 24);
      matrix.print("e");
      matrix.show();
      paused = 1;
      return;
    //}

    // if (button == 1 && paused == 1) {
    //   for (int x = 8; x > -36; x--) {
    //     matrix.clear();
    //     matrix.setCursor(x, 12);
    //     matrix.print("3 2 1");
    //     matrix.show();
    //     delay(83);
    //   }
    //   next(pieceNum);
    //   for (uint8_t Y = 31; Y > 8; Y--) {
    //     for (uint8_t X = 0; X < 8; X++) {
    //       if (colors[game[Y][X]] == 0) continue;
    //       matrix.drawPixel(X, Y, colors[game[Y][X]]);
    //     }
    //     matrix.show();
    //     delay(30);
    //   }
    //   paused = 0;
    //   return;
    // }
  }

  void countdown(void){
    for (int x = 8; x > -36; x--) {
      matrix.clear();
      matrix.setCursor(x, 12);
      matrix.print("3 2 1");
      matrix.show();
      delay(83);
    }
  }
};