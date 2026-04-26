#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "Game.h"

class MineSweeper : public Game {
public:
    MineSweeper();
    void play() override;
};

#endif
