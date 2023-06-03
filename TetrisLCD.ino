#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include "display.cpp"
#include "gameState.cpp"
#include "UserInput.cpp"
#include "pieceQueue.cpp"
#include "points.cpp"

userInput User;
gameState Game;
pieceQueue Queue;
display Draw;
points Score;



void setup() {
  Serial.begin(115200);
  Draw.begin();
  Draw.end();
  User.begin();
  delay(30);
  while (User.input() == 0) {
    Draw.idle();
    
  }
  Queue.begin();
  Game.resetPiece(Queue.current(), 4, 7, 0);
}

unsigned long dropStart;
int dropTime = 100;
bool dropping = 0;

void loop() {
  User.update();
  if (Draw.paused == 0) {
    Draw.begin();
    Score.countLines(Game.clearLine());
    Game.movePiece(User.horizontal(), User.rotation());
    if (User.droping() == 1) {
      dropTime = (long)20 * pow((0.8-((Score.level-1)*0.007)),(Score.level-1));;
      dropping = 1;
    } else {
      dropTime = (long)500 * pow((0.8-((Score.level-1)*0.007)),(Score.level-1));
      dropping = 0;
    }
    Serial.println(pow((0.8-((Score.level-1)*0.007)),(Score.level-1)));
    if ((millis() - dropStart) >= dropTime) {
      dropStart = millis();
      if (dropping == 1) Score.score++;
      if (Game.dropPiece() == 1) {
        Queue.indexPiece();
        Game.resetPiece(Queue.current(), 4, 7, 0);
      }
      Draw.score(Score.score, Score.lines, Score.level);
    }


    Game.identifyRow();
    Draw.board(Game.board);
    Draw.ghost(Game.pieceType(), Game.pieceRotation(), Game.pieceCoordX(), Game.pieceCoordY(), Game.board);
    Draw.piece(Game.pieceType(), Game.pieceRotation(), Game.pieceCoordX(), Game.pieceCoordY());
    Draw.next(Queue.next());
    Draw.end();
  }
  Draw.pause(User.pausing(), Game.board, Game.pieceType());
}
