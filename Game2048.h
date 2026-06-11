#ifndef GAME2048_H
#define GAME2048_H

#include "Game.h"
#include <vector>

class Game2048 : public Game {
private:
    int size;
    std::vector<std::vector<int>> board;
    int score;
    bool hasWon;
    bool endlessMode;

    // Builds the board size and mode selection menu.
    bool setupOptions();
    // Resets the board and spawns the starting tiles.
    void resetBoard();
    // Spawns one new tile in a random empty cell.
    void spawnTile();
    // Draws the board, score, and current game state.
    void drawBoard(bool gameOver);
    
    // Slides and merges one line according to 2048 rules.
    bool slideLine(std::vector<int>& line); 
    // Applies a move in one of the four directions.
    bool move(int direction); // 0=Up, 1=Down, 2=Left, 3=Right
    // Detects whether no legal moves remain.
    bool isGameOver();

public:
    // Sets up the 2048 game state and metadata.
    Game2048();
    // Runs the full 2048 gameplay loop.
    void play() override;
};

#endif