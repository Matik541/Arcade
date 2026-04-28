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

    bool setupOptions();
    void resetBoard();
    void spawnTile();
    void drawBoard(bool gameOver);
    
    // Core logic
    bool slideLine(std::vector<int>& line); 
    bool move(int direction); // 0=Up, 1=Down, 2=Left, 3=Right
    bool isGameOver();

public:
    Game2048();
    void play() override;
};

#endif