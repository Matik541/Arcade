#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "../Game.h"
#include <vector>
#include <ctime>

class MineSweeper : public Game {
private:
    int width, height, numMines;
    int cursorX, cursorY;
    int difficulty;
    bool firstClick;
    time_t startTime;

    std::vector<std::vector<int>> board; 
    std::vector<std::vector<bool>> revealed;
    std::vector<std::vector<bool>> flagged;

    bool setupOptions();
    void resetBoard();
    void placeMines(int safeX, int safeY);
    void calculateNumbers();
    void drawBoard(bool gameOver, bool isWin);
    void revealCell(int x, int y);
    bool checkWin();

public:
    MineSweeper();
    void play() override;
};

#endif