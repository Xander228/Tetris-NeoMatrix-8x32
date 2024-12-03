#include <Adafruit_GFX.h>        
#include <Adafruit_NeoMatrix.h>  
#include <Adafruit_NeoPixel.h>   
#include <Arduino.h>             

class PieceQueue {
private:
  uint8_t bagIndex = 0; //index to keep track of the position in the bag of pieces
  uint8_t currentPiece; //currently active piece
  uint8_t nextPiece; //next piece in the queue

  void shuffle(void) {
    //performs 30 random swaps
    for (uint8_t i = 0; i < 30; i++) {   
      uint8_t buffer = 0; //temporary storage for swapping
      uint8_t index1 = random(7); //pick a random index in the range [0, 6]
      uint8_t index2 = random(7); //pick another random index
      buffer = pieceBag[index1]; //set the buffer to the 1st index piece
      pieceBag[index1] = pieceBag[index2]; //set the 1st index piece to the 2nd index piece
      pieceBag[index2] = buffer; //set the 2nd index piece to buffer (1st index piece)
    }
  }

public:
  uint8_t pieceBag[7];; //array representing the bag of 7 pieces

  PieceQueue(void) {};

  void begin(void) {
    //populate the piece bag with unique piece
    for (uint8_t i = 0; i < 7; i++) pieceBag[i] = i;
    
    //seed the random number generator using analog input and time to ensure a different shuffle each game
    randomSeed(analogRead(0) * millis());
    
    shuffle(); //shuffle the piece bag to randomize the order
    
    indexPiece(); //prepare the current piece
    indexPiece(); //prepare the next piece
  }

  void indexPiece(void) {
    currentPiece = nextPiece; //set the current piece to the previously queued next piece
    nextPiece = pieceBag[bagIndex]; //retrieve the next piece from the bag
    bagIndex++; //move to the next index in the bag

    //if the end of the bag is reached
    if (bagIndex >= 7) {
      shuffle(); //shuffle
      bagIndex = 0; //reset the index
    }
  }

  uint8_t current(void) {
    return currentPiece; //returns the current piece in the queue
  }

  uint8_t next(void) {
    return nextPiece; //returns the next piece in the queue
  }
};
