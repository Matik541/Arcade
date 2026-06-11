#ifndef ARCADE_H
#define ARCADE_H

#include <vector>
#include <memory>
#include "Game.h"
#include "Database.h"

class Arcade {
private:
    std::vector<std::shared_ptr<Game>> games;
    
    std::string inputHistory; 
    bool rainbowMode = false;

    void drawMenu(int selected);

public:
    Arcade();
    void run();
};

#endif
