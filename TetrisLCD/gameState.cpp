#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef TETROMINOS_H
#define TETROMINOS_H
#include "Tetrominos.cpp"
#endif

class GameState {
private:
  uint8_t pieceNum; //stores the current piece's type
  int cursorX, cursorY; //coordinates of the piece's top-left corner
  int pieceX, pieceY; //coordinates of the piece's center
  int changeX, changeY; //stores movement changes for the piece
  uint8_t pieceRot;  //stores the current rotation state of the piece

public:
  uint8_t board[32][8]; //2D array representing the game board

  GameState(void) {};

  void resetPiece(uint8_t pieceNum, int pieceX, int pieceY, uint8_t pieceRot) {
    // Resets the piece with new type, position, and rotation
    this->pieceNum = pieceNum; //set the global pieceNum variable to the pieceNum parameter
    this->pieceX = pieceX; //set the global pieceX variable to the pieceX parameter
    this->pieceY = pieceY; //set the global pieceY variable to the pieceY parameter
    this->pieceRot = pieceRot; //set the global pieceRot variable to the pieceRot parameter
    cursorX = pieceX - 2; //calculate cursorX position
    cursorY = pieceY - 2; //calculate cursorY position
  }

  void movePiece(int changeX, bool changeRot) {
    int desX = cursorX + changeX; //desired new X position
    uint8_t desRot = changeRot ? pieceRot++ : pieceRot; //set desired new rotation
    if (desRot > 3) desRot = 0; //wrap around rotation value

    //check for overlap or out-of-bounds conditions and return if true
    if (isOverlapped(desX, cursorY, desRot, board)) return;
    if (isOutOfBounds(desX, cursorY, desRot)) return;

    
    cursorX = desX; //apply x move
    pieceX = cursorX + 2; //update piece's center position
    pieceRot = desRot; //apply rotation
  }

  bool dropPiece(void) {
    bool above = false; //set the above check to false

    //if the piece's position after drop intersects another cell or is out of bounds
    if (isOverlapped(cursorX, cursorY + 1, pieceRot, board) || 
        isOutOfBounds(cursorX, cursorY + 1, pieceRot)) {
      
      //iterates through the y axis
      for (uint8_t Y = 0; Y < 4; Y++) {
        //iterates through the x axis
        for (uint8_t X = 0; X < 4; X++) {
          //if the cell is empty skip writing to matrix
          if (pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X]) == 0) continue;

          //set the current cell of the board to that of the piece
          board[cursorY + Y][cursorX + X] = pgm_read_byte(&tetrominos[pieceNum][Y][pieceRot][X]);
        }
      }
      return 1; //return true indicating the piece has locked
    } else {
      pieceY++; //move piece down one row
      cursorY = pieceY - 2; //update piece's center position
      return 0; //return false indicating the piece has not locked
    }
  }

  bool isOverlapped(int x, int y, uint8_t rotation, uint8_t board[32][8]) {
    //iterates through the y axis
    for (uint8_t Y = 0; Y < 4; Y++) {
      //iterates through the x axis
      for (uint8_t X = 0; X < 4; X++) {
        //if the cell is empty skip writing to matrix
        if (pgm_read_byte(&tetrominos[pieceNum][Y][rotation][X]) == 0) continue; 
        //if the cell of the piece intersects with one on the board, return true
        if (board[y + Y][x + X] != 0) return true; 
      }
    }
    return false; //no overlap
  }

  bool isOutOfBounds(int x, int y, uint8_t rotation) {
    //iterates through the y axis
    for (uint8_t Y = 0; Y < 4; Y++) {
      //iterates through the x axis
      for (uint8_t X = 0; X < 4; X++) {
        //if the cell is empty skip writing to matrix
        if (pgm_read_byte(&tetrominos[pieceNum][Y][rotation][X]) == 0) continue; 
        //if the cell of the piece is outside the board, return true
        if (x + X < 0 || x + X > 7 || y + Y > 31) return true; 
      }
    }
    return false; //within bounds
  }

  int piecePosX(void) {
    return pieceX; //returns the piece's center X position
  }

  int piecePosY(void) {
    return pieceY; //returns the piece's center Y position
  }

  int pieceCoordX(void) {
    return cursorX; //returns the piece's top-left X position
  }

  int pieceCoordY(void) {
    return cursorY; //returns the piece's top-left Y position
  }

  uint8_t pieceRotation(void) {
    return pieceRot; //returns the piece's current rotation state
  }

  uint8_t pieceType(void) {
    return pieceNum; //returns the type of the current piece
  }

  bool lockedAboveBoard(void) {
    //for each cell along the x axis
    for (int indexX = 0; indexX < 8; indexX++) {
      //if the board is not empty at y = 6
      if (board[6][indexX] != 0) {
        return true; //return that a piece has locked above the board
      }
    }
    return false;//return that a piece hasn't locked above the board
  }

  void identifyRow(void) {
    //iterates through the board's y axis
    for (uint8_t Y = 31; Y >= 7; Y--) {
      bool rowFull = true; //set the row full check true
      bool rowEmpty = true; //set the row empty check true
      
      //iterates through the board's x axis
      for (uint8_t X = 0; X < 8; X++) {
        //if the board contains any empty cells, set rowFull false
        if (board[Y][X] == 0) rowFull = false; 
        //if the board contains any filled cells, set rowEmpty false
        if (board[Y][X] != 0) rowEmpty = false;
      }
      if (rowEmpty == 1) break; //stop if the rows above are empty

      //of the row was full
      if (rowFull == 1) {
        //for each cell in the row, turn the color white
        for (uint8_t X = 0; X < 8; X++) board[Y][X] = 8;
      }
    }
  }

  uint8_t clearLine(void) {
    uint8_t lines = 0; //set cleared lines to 0
    
    //iterates through the board's y axis
    for (uint8_t Y = 7; Y < 32; Y++) {
      bool rowFull = true; //set the rowFull check true

      //iterates through the board's x axis
      for (uint8_t X = 0; X < 8; X++) {
        //if the board contains any empty cells, set rowFull false
        if (board[Y][X] == 0) rowFull = 0; 
      }

      //if the row is full
      if (rowFull == 1) {
        //iterates through all Y above the locked row
        for (uint8_t writeY = Y; writeY >= 0; writeY--) {
          bool rowFinish = 1; //set the rowFinish check true

          //iterates through the board's x axis
          for (uint8_t X = 0; X < 8; X++) {
            //set the cell below the current cell to itself
            board[writeY][X] = board[writeY - 1][X]; 

            //if the cell above the current cell is not empty, set rowFinish false
            if (board[writeY + 1][X] != 0) rowFinish = false;
          }
          if (rowFinish == 1) break; //if all rows have been shifted, escape the for loop
        }
        lines++; //increment lines by one for each row cleared
      }
    }

    //if any lines have been cleared
    if (lines > 0) {
      delay(200); //small delay after clearing lines
      return lines; //return the number of cleared lines
    }
    return 0; //return no lines cleared
  }
};
