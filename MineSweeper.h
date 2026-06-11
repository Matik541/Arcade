#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "Game.h"
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
    std::vector<std::vector<int>> cellState; // 0=Normal, 1=Flag, 2=Question

    // Builds the setup menu and records the selected board size.
    bool setupOptions();
    // Allocates and resets the board state for a new round.
    void resetBoard();
    // Places mines while keeping the first click and its neighbors safe.
    void placeMines(int safeX, int safeY);
    // Computes adjacent mine counts for every non-mine cell.
    void calculateNumbers();
    // Draws the visible board, cursor, and status bar.
    void drawBoard(bool gameOver, bool isWin);
    // Reveals one cell and recursively expands zero-value areas.
    void revealCell(int x, int y);
    // Opens adjacent cells when the visible flags match the number.
    bool chordCell(int x, int y);
    // Checks whether every safe cell has been revealed.
    bool checkWin();

public:
    // Sets up the MineSweeper game state and menu metadata.
    MineSweeper();
    // Runs the full MineSweeper gameplay loop.
    void play() override;
};

#endif