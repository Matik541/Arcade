#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include "Game.h"
#include <vector>
#include <string>

class Battleship : public Game {
private:
    int botDifficulty; // 1=Easy, 2=Mid, 3=Hard
    bool manualPlacement;
    int pCursorX, pCursorY;
    std::string actionMessage;

    // 0 = Water (~), 1 = Ship (#), 2 = Miss (O), 3 = Hit (X)
    std::vector<std::vector<int>> playerBoard;
    std::vector<std::vector<int>> botBoard;

    // Bot AI tracking
    std::vector<std::vector<bool>> botFired;
    std::vector<std::pair<int, int>> botTargetQueue;

    bool setupOptions();
    void resetBoards();
    bool canPlaceShip(const std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal);
    void placeShip(std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal);
    void autoPlaceShips(std::vector<std::vector<int>>& board);
    void manualPlaceShips();

    void drawBoards();
    std::string botTurn();
    bool checkWin(const std::vector<std::vector<int>>& board);

public:
    Battleship();
    void play() override;
};

#endif