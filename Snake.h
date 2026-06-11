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
    
    // Stores the snake body from head to tail.
    std::deque<std::pair<int, int>> snake;

    // Builds the difficulty selection menu.
    bool setupOptions();
    // Resets the snake, score, and direction for a new run.
    void resetGame();
    // Places food in a cell not occupied by the snake.
    void spawnFood();
    // Draws the board, snake, and food.
    void drawBoard(bool gameOver);

public:
    // Sets up the Snake game state and metadata.
    Snake();
    // Runs the full Snake gameplay loop.
    void play() override;
};

#endif