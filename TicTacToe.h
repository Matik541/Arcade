#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "Game.h"
#include <vector>

class TicTacToe : public Game {
private:
    std::vector<char> board;
    char currentPlayer;
    bool vsBot;
    char botPiece;

    void resetBoard();
    void drawBoard();
    bool checkWin(char player);
    bool checkDraw();
    void botMove();
    bool setupOptions();

public:
    TicTacToe();
    void play() override;
};

#endif