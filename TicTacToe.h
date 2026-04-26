#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "Game.h"

class TicTacToe : public Game {
public:
    TicTacToe();
    void play() override;
};

#endif