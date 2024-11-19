#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#ifndef TETROMINOS_H
#define TETROMINOS_H
#include "tetrominos.cpp"
#endif
#define PIN 6

class display {
private:
  const uint16_t dim = 4;
  const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
  int x = matrix.width();
  Adafruit_NeoMatrix matrix;
  LiquidCrystal lcd;

  uint16_t colors[12] = {
    matrix.Color(0, 0, 0),
    matrix.Color(0, 255, 255),
    matrix.Color(0, 0, 255),
    matrix.Color(255, 127, 0),
    matrix.Color(255, 255, 0),
    matrix.Color(0, 255, 0),
    matrix.Color(255, 0, 255),
    matrix.Color(255, 0, 0),
    matrix.Color(255, 255, 255),
  };

  uint16_t colorsGhost[12] = {
    matrix.Color(0 / dim, 0 / dim, 0 / dim), matrix.Color(0 / dim, 255 / dim, 255 / dim), matrix.Color(0 / dim, 0 / dim, 255 / dim),
    matrix.Color(255 / dim, 127 / dim, 0 / dim), matrix.Color(255 / dim, 255 / dim, 0 / dim), matrix.Color(0 / dim, 255 / dim, 0 / dim),
    matrix.Color(255 / dim, 0 / dim, 255 / dim), matrix.Color(255 / dim, 0 / dim, 0 / dim), matrix.Color(255 / dim, 255 / dim, 255 / dim)
  };

  uint16_t colorsLogo[15] = {
    matrix.Color(0, 0, 0), matrix.Color(26, 66, 147), matrix.Color(3, 14, 50),
    matrix.Color(240, 30, 40), matrix.Color(255, 0, 0),
    matrix.Color(245, 140, 35), matrix.Color(240, 75, 35),
    matrix.Color(250, 225, 35), matrix.Color(250, 160, 30),
    matrix.Color(122, 255, 65), matrix.Color(18, 129, 15),
    matrix.Color(0, 226, 245), matrix.Color(7, 111, 231),
    matrix.Color(255, 56, 255), matrix.Color(113, 3, 83)
  };

  //   uint16_t colorsGhost[12] = {
  //   matrix.Color(0, 0, 0), matrix.Color(0, 127, 127), matrix.Color(0, 0, 127),
  //   matrix.Color(127, 63, 0), matrix.Color(127, 127, 0), matrix.Color(0, 127, 0),
  //   matrix.Color(127, 0, 127), matrix.Color(127, 0, 0), matrix.Color(127, 127, 127)
  // };

public:
  display(void)
    : matrix(8, 32, PIN,
             NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
             NEO_GRB + NEO_KHZ800),
      lcd(rs, en, d4, d5, d6, d7) {
    matrix.begin();
    lcd.begin(16, 2);
    matrix.setTextWrap(false);
    matrix.setBrightness(120);
    lcd.clear();
    int x = matrix.width();
  };


  void piece(uint8_t pieceNum, uint8_t pieceRotation, int pieceX, int pieceY) {
    for (uint8_t Y = 0; Y < 4; Y++) {
      for (uint8_t X = 0; X < 4; X++) {
        if (colors[pgm_read_byte(&tetrominos[pieceNum][Y][pieceRotation][X])] == 0) continue;
        matrix.drawPixel(X + pieceX, Y + pieceY, colors[pgm_read_byte(&tetrominos[pieceNum][Y][pieceRotation][X])]);
      }
    }
  }

  void board(uint8_t game[32][8]) {
    for (uint8_t Y = 0; Y < 32; Y++) {
      for (uint8_t X = 0; X < 8; X++) {
        if (colors[game[Y][X]] == 0) continue;
        matrix.drawPixel(X, Y, colors[game[Y][X]]);
      }
    }
  }

  void next(uint8_t pieceNum) {
    matrix.fillRect(0, 0, 8, 6, colors[0]);
    matrix.fillRect(0, 6, 8, 1, colors[8]);
    for (uint8_t Y = 0; Y < 4; Y++) {
      for (uint8_t X = 0; X < 4; X++) {
        if (colors[pgm_read_byte(&tetrominos[pieceNum][Y][0][X])] == 0) continue;
        if (pieceNum == 3) {
          matrix.fillRect(2 * X, 2 * Y - 1, 2, 2, colors[pgm_read_byte(&tetrominos[pieceNum][Y][0][X])]);
          continue;
        }
        if (pieceNum == 0) {
          matrix.fillRect(2 * X, 2 * Y, 2, 2, colors[pgm_read_byte(&tetrominos[pieceNum][Y][0][X])]);
          continue;
        }
        matrix.fillRect(2 * X + 1, 2 * Y - 1, 2, 2, colors[pgm_read_byte(&tetrominos[pieceNum][Y][0][X])]);
      }
    }
  }

  void ghost(uint8_t pieceNum, uint8_t pieceRot, int pieceX, int pieceY, uint8_t board[32][8]) {
    bool above = 0;
    for (uint8_t changeY = pieceY; changeY < 32; changeY++) {
      for (uint8_t Y = 0; Y < 4; Y++) {
        for (uint8_t X = 0; X < 4; X++) {
          if (pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X]) == 0) continue;
          if (board[changeY + Y + 1][pieceX + X] != 0 || changeY + Y > 30) {
            above = 1;
            break;
          }
        }
        if (above == 1) break;
      }
      if (above == 1) {
        for (uint8_t Y = 0; Y < 4; Y++) {
          for (uint8_t X = 0; X < 4; X++) {
            if (colors[pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X])] == 0) continue;
            matrix.drawPixel(X + pieceX, Y + changeY, colorsGhost[pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X])]);
          }
        }
        break;
      }
      if (above == 1) break;
    }
  }

  void begin() {
    matrix.clear();
  }

  void end() {
    matrix.show();
  }


  void score(unsigned long score, unsigned short lines, unsigned short level) {
    lcd.setCursor(0, 0);
    lcd.print("Score: ");
    lcd.print(score);
    lcd.setCursor(0, 1);
    lcd.print("Lvl: ");
    lcd.print(level);
    lcd.setCursor(8, 1);
    lcd.print("Line: ");
    lcd.print(lines);
  }

  void idle(void) {
    
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
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
  void pause(bool button, uint8_t game[32][8], uint8_t pieceNum) {
    if (button == 1 && paused == 0) {
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
    }

    if (button == 1 && paused == 1) {
      for (int x = 8; x > -36; x--) {
        matrix.clear();
        matrix.setCursor(x, 12);
        matrix.print("3 2 1");
        matrix.show();
        delay(83);
      }
      next(pieceNum);
      for (uint8_t Y = 31; Y > 8; Y--) {
        for (uint8_t X = 0; X < 8; X++) {
          if (colors[game[Y][X]] == 0) continue;
          matrix.drawPixel(X, Y, colors[game[Y][X]]);
        }
        matrix.show();
        delay(30);
      }
      paused = 0;
      return;
    }
  }
};