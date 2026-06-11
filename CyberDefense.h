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

    // Checks whether a tower already exists on a tile.
    bool hasTowerAt(int x, int y);
    // Returns the index of the tower at a tile, or -1 if none exists.
    int getTowerIndex(int x, int y);
    // Checks whether an enemy has crossed into a lane range.
    bool isEnemyInLane(int y, int minX);
    
    // Spawns one enemy for the current wave.
    void spawnEnemy();
    // Advances towers, enemies, and projectiles by one tick.
    void updateLogic();
    // Draws the lane map, towers, enemies, and HUD.
    void drawBoard();
    // Restores the default wave and currency state.
    void resetGame();

public:
    // Sets up the Cyber Defense game state and metadata.
    CyberDefense();
    // Runs the full lane-defense gameplay loop.
    void play() override;
};

#endif