#include "Database.h"

std::vector<ScoreEntry> MockDatabase::getTop5(const std::string& gameName) {
    if (memoryDb.find(gameName) != memoryDb.end()) {
        return memoryDb[gameName];
    }
    return {}; 
}

void MockDatabase::saveScore(const std::string& gameName, const std::string& player, int score) {
    memoryDb[gameName].push_back({player, score});
    
    std::sort(memoryDb[gameName].begin(), memoryDb[gameName].end(), 
        [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score < b.score;
        });

    if (memoryDb[gameName].size() > 5) {
        memoryDb[gameName].pop_back();
    }
}