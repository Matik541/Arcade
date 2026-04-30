#ifndef GAME_H
#define GAME_H

#include <string>
#include "Database.h"

class Game {
protected:
    std::string name;
    std::string description;
    bool hasScoreboard;
    bool higherIsBetter;
    IDatabase* db; 

public:
    Game(std::string n, std::string desc, bool sc, bool hib = true) : name(n), description(desc), hasScoreboard(sc), higherIsBetter(hib), db(nullptr) {}
    virtual ~Game() = default;

    virtual void play() = 0; 
    
    void setDatabase(IDatabase* database) { db = database; }

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    bool usesScoreboard() const { return hasScoreboard; }
    bool isHigherScoreBetter() const { return higherIsBetter; }
};

#endif