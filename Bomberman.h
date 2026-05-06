#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include "Game.h"
#include <vector>

struct Bomb {
    int x, y;
    int ticksLeft;
    int radius;
    int owner; // 1 (P1) or 2 (P2)
};

struct Explosion {
    int x, y;
    int ticksLeft;
};

class Bomberman : public Game {
private:
    int width = 15;
    int height = 15;
    
    // 0 = Empty, 1 = Indestructible Wall, 2 = Destructible Block
    std::vector<std::vector<int>> map; 
    
    int p1X, p1Y;
    bool p1Alive;
    int p1MaxBombs;
    int p1BombsActive;
    
    int p2X, p2Y;
    bool p2Alive;
    int p2MaxBombs;
    int p2BombsActive;

    std::vector<Bomb> bombs;
    std::vector<Explosion> explosions;

    void generateMap();
    void resetGame();
    void explodeBomb(int index);
    bool hasBombAt(int x, int y);
    bool hasExplosionAt(int x, int y);
    void drawBoard();

public:
    Bomberman();
    void play() override;
};

#endif