#ifndef ARCADE_H
#define ARCADE_H

#include <vector>
#include <memory>
#include "Game.h"
#include "Database.h"

class Arcade {
private:
    std::vector<std::shared_ptr<Game>> games;
    std::unique_ptr<Database> db;
    
    // Renders the main menu, selected game, and scoreboard preview.
    void drawMenu(int selected);

public:
    // Builds the game list and loads the shared score database.
    Arcade();
    // Drives the top-level arcade menu and launches games.
    void run();
};

#endif