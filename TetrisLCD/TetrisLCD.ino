#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include "Display.cpp"
#include "GameState.cpp"
#include "UserInput.cpp"
#include "PieceQueue.cpp"
#include "Score.cpp"

UserInput user; //creates user object to handle user input
GameState gameState; //creates gameState object to handle the state of the game
PieceQueue queue; //creates queue object to generate pieces
Display display; //creates display object to handle the neoMatrix and LCD displays
Score score; //creates a score object to store and calulate score, lines, and level
volatile bool unpause; //lets the ISR know to unpause
volatile bool paused; //stores whether the game is currently paused


void setup() {
  Serial.begin(115200); //begin the serial monitor
  display.clear(); //clear the display
  display.show(); //show the cleared display
  user.begin(); //initialize user class
  
  delay(30); //wait for initialization

  //while no buttons are pressed
  while (user.input() == 0) {
    display.idle(); //show the start screen
    
  }
  queue.begin(); //setup the piece queue
  gameState.resetPiece(queue.current(), 4, 7, 0); //set the current piece in hand to the first piece in the queue

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), pause, FALLING);
}

void pause(void){
  delayMicroseconds(10000);
  if(paused) {
    if(!digitalRead(2)) unpause = true;
    return;
  }
  paused = true;
  while(!unpause){
    Serial.println("paused");
    display.pause();
  }
  display.countdown();
  paused = false;
  unpause = false;
}


unsigned long dropLast; //the last successful drop
int dropTime; //time it should take for the piece to drop once
bool dropping = false; //whether the piece is currently dropping

void loop() {
  display.clear(); //clear the display
  user.update(); //update user values with current button states

  //if the display is not paused
  //if (display.paused == 0) {
    gameState.movePiece(user.horizontal(), user.rotation()); //move piece based on user input

    //if the user is dropping
    if (user.droping()) {
      dropTime = (long)20 * pow((0.8-((score.level-1)*0.007)),(score.level-1));; //set the drop time quicker
      dropping = true; //set dropping true
    } else {
      dropTime = (long)500 * pow((0.8-((score.level-1)*0.007)),(score.level-1)); //set the drop time normal
      dropping = false; //set dropping false
    }

    //if the difference between when the piece was last dropped and now is greater than or equal to dropTime
    if ((millis() - dropLast) >= dropTime) {
      dropLast = millis(); //reset the last drop time
      if (dropping) score.score++; //give a bonus score while dropping for every time it drops

      //if dropping the piece results in a lock
      if (gameState.dropPiece()) {
        queue.indexPiece(); //get the next piece in queue
        gameState.resetPiece(queue.current(), 4, 7, 0); //set the  current piece in hand to the piece pulled from the queue
      }
      display.score(score.score, score.lines, score.level); //display the score on the lcd
    }


    gameState.identifyRow(); //set the color of the cells to white if they are in a row
    display.board(gameState.board); //send the board to the display to show

    //display the current piece in hand
    display.ghost(gameState.pieceType(), gameState.pieceRotation(), gameState.pieceCoordX(), gameState.pieceCoordY(), gameState.board);

    //display the guide ghost piece
    display.piece(gameState.pieceType(), gameState.pieceRotation(), gameState.pieceCoordX(), gameState.pieceCoordY());

    display.next(queue.next()); //display the next piece in queue in the queue area
    display.show(); //show the display
    score.countLines(gameState.clearLine()); //attempt to clear line from the board 
  //}

  //check if the user is pausing
  //display.checkPause(user.pausing(), gameState.board, gameState.pieceType()); 
}
