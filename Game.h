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
    Database* db; 

public:
    // Stores the shared game metadata and initial database state.
    Game(std::string n, std::string desc, bool sc, bool hib = true) : name(n), description(desc), hasScoreboard(sc), higherIsBetter(hib), db(nullptr) {}
    // Keeps the base class lightweight for derived game implementations.
    virtual ~Game() = default;

    // Runs the complete gameplay loop for a specific game.
    virtual void play() = 0; 
    
    // Provides the shared score database to the game.
    void setDatabase(Database* database) { db = database; }

    // Returns the public game name shown in menus and scoreboards.
    std::string getName() const { return name; }
    // Returns the short description displayed in the arcade menu.
    std::string getDescription() const { return description; }
    // Indicates whether this game records high scores.
    bool usesScoreboard() const { return hasScoreboard; }
    // Indicates whether higher or lower values rank better.
    bool isHigherScoreBetter() const { return higherIsBetter; }
};

#endif