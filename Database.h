#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

struct ScoreEntry {
    std::string playerName;
    int score;
};

class Database {
private:
    std::string filename;

public:
    // Opens the score storage file used by the arcade.
    Database(const std::string& file);
    
    // Appends a score entry to the backing score file.
    void saveScore(const std::string& gameName, const std::string& playerName, int score, bool higherIsBetter);
    
    // Loads and sorts the best scores for one game.
    std::vector<ScoreEntry> getTopScores(const std::string& gameName, bool higherIsBetter, int limit = 3);
};

#endif