#ifndef CYBERDEFENSE_H
#define CYBERDEFENSE_H

#include "Game.h"
#include <vector>

struct Tower {
    int x, y;
    int type; // 1 = Miner (Generates Bits), 2 = Turret (Shoots), 3 = Firewall (High HP)
    int hp;
    int timer; // Tracks cooldowns for generating or shooting
};

struct Enemy {
    int x, y;
    int hp;
    int maxHp;
    char symbol; 
    int moveTimer;
    int speed; // Ticks required to move 1 space
};

struct Projectile {
    int x, y;
    int damage;
};

class CyberDefense : public Game {
private:
    int width = 15;
    int height = 5; // 5 lanes
    int bits; // Your currency
    int cursorX, cursorY;
    int tickCounter;
    bool isGameOver;
    bool playerWon;
    int waveDuration;

    std::vector<Tower> towers;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    bool hasTowerAt(int x, int y);
    int getTowerIndex(int x, int y);
    bool isEnemyInLane(int y, int minX);
    
    void spawnEnemy();
    void updateLogic();
    void drawBoard();
    void resetGame();

public:
    CyberDefense();
    void play() override;
};

#endif