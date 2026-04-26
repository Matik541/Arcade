#ifndef ARCADE_H
#define ARCADE_H

#include "Game.h"
#include "Database.h"
#include <vector>
#include <memory>

class Arcade {
private:
    std::vector<std::shared_ptr<Game>> soloGames;
    std::vector<std::shared_ptr<Game>> pvpGames;
    std::unique_ptr<IDatabase> db;

    int selectedIndex;
    bool running;

    void renderFrame();
    void handleInput();

public:
    Arcade();
    void run();
};

#endif