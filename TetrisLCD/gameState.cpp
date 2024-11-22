#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef TETROMINOS_H
#define TETROMINOS_H
#include "Tetrominos.cpp"
#endif


class GameState {
private:
  uint8_t pieceNum;
  int cursorX, cursorY;
  int pieceX, pieceY;
  int changeX, changeY;
  uint8_t pieceRot;

public:
  uint8_t board[32][8];

  GameState(void){};

  void resetPiece(uint8_t pieceNum, int pieceX, int pieceY, uint8_t pieceRot) {
    this->pieceNum = pieceNum;
    this->pieceX = pieceX;
    this->pieceY = pieceY;
    this->pieceRot = pieceRot;
    cursorX = pieceX - 2;
    cursorY = pieceY - 2;
  }


  void movePiece(int changeX, bool changeRot) {
    int desX = cursorX + changeX;
    uint8_t desRot = pieceRot;
    if (changeRot) desRot++;
    if (desRot > 3) desRot = 0;

    if(isOverlapped(desX, cursorY, desRot, board)) return;
    if(isOutOfBounds(desX, cursorY, desRot)) return;

    
    cursorX = desX;
    pieceX = cursorX + 2;
    pieceRot = desRot;

    // cursorX += changeX;
    // if (changeRot) pieceRot++;
    // if (pieceRot > 3) pieceRot = 0;
    // bool done = 0;
    // while (done == 0) {
    //   for (uint8_t Y = 0; Y < 4; Y++) {
    //     for (uint8_t X = 0; X < 4; X++) {
    //       if (pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X]) == 0) continue;
    //       if (cursorX + X < 0) {
    //         cursorX++;
    //         continue;
    //       }
    //       if (cursorX + X > 7) {
    //         cursorX--;
    //         continue;
    //       }
    //       done = 1;
    //     }
    //   }
    // }
    // pieceX = cursorX + 2;
  }

  bool dropPiece(void) {
    bool above = 0;
    if (pieceRot > 3) pieceRot = 0;

    if (isOverlapped(cursorX, cursorY + 1, pieceRot, board) || isOutOfBounds(cursorX, cursorY + 1, pieceRot)) {
      for (uint8_t Y = 0; Y < 4; Y++) {
        for (uint8_t X = 0; X < 4; X++) {
          if (pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X]) == 0) continue;
          board[cursorY + Y][cursorX + X] = pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X]);
        }
      }
      return 1;
    } else {
      pieceY++;
      cursorY = pieceY - 2;
      return 0;
    }
  }

  bool isOverlapped(int x, int y, uint8_t rotation, uint8_t board[32][8]) {
    for (uint8_t Y = 0; Y < 4; Y++) {
      for (uint8_t X = 0; X < 4; X++) {
        //ignores tile if no mino occupies it
        if (pgm_read_byte(&tetrominos[pieceNum][Y][rotation][X]) == 0) continue;
        
        //if a mino exists, return true if it is touching another mino on the board
        if (board[y + Y][x + X] != 0) return true;
      }
    }
    //return false if all minos pass the test
      return false;
  }
    
  bool isOutOfBounds(int x, int y, uint8_t rotation) {
    for (uint8_t Y = 0; Y < 4; Y++) {
      for (uint8_t X = 0; X < 4; X++) {
        //ignores tile if no mino occupies it
        if (pgm_read_byte(&tetrominos[pieceNum][Y][rotation][X]) == 0) continue;
        //if a mino exists, return true if it is outside the left right or bottom bounds
        if (x + X < 0 || 
            x + X > 7 || 
            y + Y > 31) return true;
      }
    }
    //return false if all minos pass the test
    return false;
  }


  int piecePosX(void) {
    return pieceX;
  }

  int piecePosY(void) {
    return pieceY;
  }

  int pieceCoordX(void) {
    return cursorX;
  }

  int pieceCoordY(void) {
    return cursorY;
  }

  uint8_t pieceRotation(void) {
    return pieceRot;
  }

  uint8_t pieceType(void) {
    return pieceNum;
  }


  bool lockedAboveBoard(void) {
        for (int indexX = 0; indexX < 8; indexX++) {
            if (board[7][indexX] != 0) {
                return true;
            }
        }
        return false;
    }


  void identifyRow(void) {
    for (uint8_t Y = 31; Y >= 7; Y--) {
      bool rowFull = 1;
      bool rowEmpty = 1;
      for (uint8_t X = 0; X < 8; X++) {
        if (board[Y][X] == 0) rowFull = 0;
        if (board[Y][X] != 0) rowEmpty = 0;
      }
      if (rowEmpty == 1) break;
      if (rowFull == 1) {
        for (uint8_t X = 0; X < 8; X++) board[Y][X] = 8;
      }
    }
  }


  uint8_t clearLine(void) {
    uint8_t Lines = 0;
    for (uint8_t Y = 7; Y < 32; Y++) {
      bool rowFull = 1;
      for (uint8_t X = 0; X < 8; X++) {
        if (board[Y][X] == 0) rowFull = 0;
      }
      if (rowFull == 1) {
        for (uint8_t writeY = Y; writeY >= 0; writeY--) {
          bool rowFinish = 1;
          for (uint8_t X = 0; X < 8; X++) {
            board[writeY][X] = board[writeY - 1][X];
            if (board[writeY + 1][X] != 0) rowFinish = 0;
          }
          if (rowFinish == 1) break;
        }
        Lines++;
      }
    }
    if (Lines > 0) {
      delay(200);
      return Lines;
    }
    return 0;
  }
};