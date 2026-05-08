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
    Database(const std::string& file);
    
    // Save a new score to the text file
    void saveScore(const std::string& gameName, const std::string& playerName, int score, bool higherIsBetter);
    
    // Fetch and sort the top scores for a specific game
    std::vector<ScoreEntry> getTopScores(const std::string& gameName, bool higherIsBetter, int limit = 3);
};

#endif