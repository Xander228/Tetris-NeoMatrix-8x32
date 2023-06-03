#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class pieceQueue {
private:
  uint8_t bagIndex = 0;
  uint8_t currentPiece;
  uint8_t nextPiece;

  void shuffle(void) {
    for (uint8_t i = 0; i < 30; i++) {
      uint8_t buffer = 0;
      uint8_t index1 = random(7);
      uint8_t index2 = random(7);
      buffer = pieceBag[index1];
      pieceBag[index1] = pieceBag[index2];
      pieceBag[index2] = buffer;
    }
  }
public:
  uint8_t pieceBag[7];

  pieceQueue(void){};

  void begin(void) {
    for (uint8_t i = 0; i < 7; i++) pieceBag[i] = i;
    randomSeed(analogRead(0) * millis());
    shuffle();
    indexPiece();
    indexPiece();
  }

  void indexPiece(void) {
    currentPiece = nextPiece;
    nextPiece = pieceBag[bagIndex];
    bagIndex++;
    if (bagIndex >= 7) {
      shuffle();
      bagIndex = 0;
    }
  }
  uint8_t current(void) {
    return currentPiece;
  }
  uint8_t next(void) {
    return nextPiece;
  }
};