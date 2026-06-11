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
    
    bool slideLine(std::vector<int>& line); 
    bool move(int direction);
    bool isGameOver();

public:
    Game2048();
    void play() override;
};

#endif