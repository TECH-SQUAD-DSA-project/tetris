# Tetris Game - Team Tech-Squad

## Description

This is a console-based Tetris game built using C++ and the Windows API. The game includes various Tetrimino shapes, different difficulty levels, grid size customization, a pause menu, and a high score system.

## Features

- **Classic Tetris Gameplay**: Rotate, move, and drop Tetrominoes to clear lines.
- **Difficulty Levels**: Easy, Normal, and Hard difficulty settings.
- **Grid Size Customization**: Players can set custom grid width and height within limits.
- **High Score Tracking**: Saves and loads the highest score from a file.
- **Pause Menu**: Players can pause and resume the game anytime.
- **Developer Mode**: Enabled for debugging and testing.

## Controls

- **Arrow Keys**: Move Tetromino left, right, or down.
- **Spacebar**: Hard drop Tetromino.
- **R**: Rotate Tetromino.
- **ESC**: Pause menu.
- **E**: Exit game.
- **M**: Return to main menu.

## Installation & Setup

1. **Download & Compile**

   - Ensure you have a C++ compiler (MinGW or Visual Studio) installed.
   - Compile using: `g++ -o tetris tetris.cpp -static-libstdc++ -static-libgcc`
   - Run the game using: `tetris.exe`

2. **User Data File**

   - The game reads and writes to `userdata.txt` to save the highest score and grid settings.

## Customization

- **Changing Grid Size**:

  - Select "Options" from the main menu.
  - Enter a width (10-60) and height (10-30).

- **Changing Difficulty**:

  - Select "Change Difficulty" in the Options menu.
  - Choose between Easy (1), Normal (2), and Hard (4).

## Game Over & Restart

- When the blocks reach the top, the game ends.
- Players can restart, go back to the main menu, or exit.

## Known Issues & Fixes

- **Game crashes on invalid input**: If invalid input occurs, restart the game.
- **Cursor flickering**: This is due to console refresh rates. Running in full-screen mode may help.

## Credits

- Developed by **Team Tech-Squad**

## License

This project is open-source and available for modification and enhancement.

If it requires improvement free feel to tell us
