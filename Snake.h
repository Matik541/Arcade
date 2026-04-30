#ifndef SNAKE_H
#define SNAKE_H

#include "Game.h"
#include <deque>
#include <utility>

class Snake : public Game {
private:
    int width, height;
    int speedMs;
    int score;
    bool isScored;

    int dirX, dirY; // Velocity
    int foodX, foodY;
    
    // Deque of {X, Y} coordinates. Index 0 is the head.
    std::deque<std::pair<int, int>> snake;

    bool setupOptions();
    void resetGame();
    void spawnFood();
    void drawBoard(bool gameOver);

public:
    Snake();
    void play() override;
};

#endif