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

    // Builds the setup screens for AI difficulty and fleet placement.
    bool setupOptions();
    // Clears board state and prepares both fleets for a new match.
    void resetBoards();
    // Verifies that a ship fits entirely inside the target cells.
    bool canPlaceShip(const std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal);
    // Writes one ship into a board using its internal ship ID.
    void placeShip(std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal, int shipId);
    // Randomly places an entire fleet without overlap.
    void autoPlaceShips(std::vector<std::vector<int>>& board, std::vector<ShipData>& fleet);
    // Runs the interactive placement screen for the player's fleet.
    void manualPlaceShips();
    
    // Returns the correct ship glyph for the given segment and orientation.
    std::string getShipPart(int x, int y, int startX, int startY, int length, bool horizontal);
    
    // Draws both boards and the current status message.
    void drawBoards();
    // Chooses the bot's next target based on the selected difficulty.
    std::string botTurn();
    // Checks whether every ship in a fleet has been sunk.
    bool checkWin(const std::vector<ShipData>& fleet);

public:
    // Sets up the Battleship game state and menu metadata.
    Battleship();
    // Runs the full Battleship match loop.
    void play() override;
};

#endif