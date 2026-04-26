#ifndef GAME_H
#define GAME_H

#include <string>

class Game {
protected:
    std::string name;
    std::string description;
    bool hasScoreboard;

public:
    Game(std::string n, std::string desc, bool sc) : name(n), description(desc), hasScoreboard(sc) {}
    virtual ~Game() = default;

    virtual void play() = 0; // Pure virtual function

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    bool usesScoreboard() const { return hasScoreboard; }
};

#endif