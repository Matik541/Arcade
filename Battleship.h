#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include "Game.h"
#include <vector>
#include <string>

struct ShipData {
    std::string name;
    int length;
    bool sunk;
    int startX;
    int startY;
    bool horizontal;
};

class Battleship : public Game {
private:
    int botDifficulty; 
    bool manualPlacement;
    int pCursorX, pCursorY;
    std::string actionMessage;
    
    // -1 = Water, 0-4 = Ship IDs
    std::vector<std::vector<int>> playerBoard; 
    std::vector<std::vector<int>> botBoard; 

    // 0 = Hidden/Untouched, 1 = Miss, 2 = Hit
    std::vector<std::vector<int>> playerState; 
    std::vector<std::vector<int>> botState; 

    std::vector<ShipData> pShips;
    std::vector<ShipData> bShips;

    std::vector<std::vector<bool>> botFired;
    std::vector<std::pair<int, int>> botTargetQueue;

    bool setupOptions();
    void resetBoards();
    bool canPlaceShip(const std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal);
    void placeShip(std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal, int shipId);
    void autoPlaceShips(std::vector<std::vector<int>>& board, std::vector<ShipData>& fleet);
    void manualPlaceShips();
    
    // NEW: Determines which shape to draw based on orientation
    std::string getShipPart(int x, int y, int startX, int startY, int length, bool horizontal);
    
    void drawBoards();
    std::string botTurn();
    bool checkWin(const std::vector<ShipData>& fleet);

public:
    Battleship();
    void play() override;
};

#endif