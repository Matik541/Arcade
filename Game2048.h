#ifndef GAME2048_H
#define GAME2048_H

#include "Game.h"

class Game2048 : public Game {
public:
    Game2048();
    void play() override;
};

#endif
