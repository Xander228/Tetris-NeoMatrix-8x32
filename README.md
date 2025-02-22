# Tetris Game for Arduino with NeoMatrix and LCD Display

## Overview

This project implements a Tetris game on an Arduino-based platform with the help of a NeoMatrix LED display and an LCD screen. It also includes features such as pausing, game over detection, and a dynamic difficulty system where the drop speed of pieces increases with the player's score.

## Features

- **NeoMatrix LED Display**: Displays the game board, active pieces, ghost pieces, and next pieces.
- **LCD Display**: Shows the player's current score, lines cleared, and level.
- **Piece Queue**: Keeps track of upcoming pieces, allowing the player to anticipate the next piece.
- **Dynamic Drop Speed**: The time it takes for pieces to drop shortens as the player advances through levels.
- **Pause and Resume**: The game can be paused and resumed with a button press.
- **Game Over Detection**: If a piece locks above the board, the game will reset.

## Hardware Required

- **Arduino Board** (e.g., Arduino Mega, Arduino Uno or other boards need to change pin nuumbers)
- **NeoMatrix LED Display**: For displaying the Tetris game board.
- **LCD Display**: For showing the score and game status.
- **IR Remote**: For controlling the game (optional).
- **Push Button**: For pausing the game.
- **Wires** and **Breadboard** (optional, for setting up connections).

## Pin Configuration
**All buttons use an internal pull-up resistor meaning buttons should be connected to ground**
- **Move Muttons**: On Arduino Mega, connect left to D19, down to D21, right to D18, and up to D20
- **NeoMatrix LED Display**: Connect the neoMatrix display to pin D6
- **LCD Display**: Connect to the corresponding pins on the LCD to rs = D13, en = D12, d4 = D11, d5 = D10, d6 = D9, d7 = D8
- **Pause Button**: Connect the button to pin 2, configured with a pull-up resistor.

## Gameplay Instructions

1. **Start the Game**: When the game initializes, it will show a start screen. Press any button or use the IR remote to begin the game.
2. **Move Pieces**: Use the button or IR remote to control the movement of the active Tetris piece:
   - **Left / Right**: Move the piece horizontally.
   - **Rotate**: Rotate the piece clockwise.
   - **Down**: Move the piece down faster.
   - **Drop**: Press the drop button to instantly drop the piece to the bottom.
3. **Pause the Game**: Press the pause button to pause the game. Press it again to unpause.
4. **Game Over**: The game ends when a piece locks above the top of the board. The board is cleared, and the game restarts.

## Installation

1. **Clone or Download** this repository.
2. **Install the Libraries**:
   - Download and install the following libraries from the Arduino Library Manager or GitHub:
     - `Adafruit_GFX`
     - `Adafruit_NeoMatrix`
     - `Adafruit_NeoPixel`
     - `LiquidCrystal`
     - `IRremote`
3. **Upload the Code** to your Arduino board using the Arduino IDE.
4. **Connect Hardware** according to the pin configuration mentioned above.
5. **Power on** the Arduino and enjoy the game!

