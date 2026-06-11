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
    
    Database(const std::string& file) : filename(file) {}

public:
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    static Database& getInstance() {
        static Database instance("scores.txt");
        return instance;
    }
    
    void saveScore(const std::string& gameName, const std::string& playerName, int score, bool higherIsBetter);
    std::vector<ScoreEntry> getTopScores(const std::string& gameName, bool higherIsBetter, int limit = 3);
};

#endif