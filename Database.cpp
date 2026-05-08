#include "Database.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

Database::Database(const std::string& file) : filename(file) {}

void Database::saveScore(const std::string& gameName, const std::string& playerName, int score, bool higherIsBetter) {
    // Open in append mode so we don't overwrite existing scores
    std::ofstream outFile(filename, std::ios::app);
    if (outFile.is_open()) {
        // Format: GameName,PlayerName,Score
        outFile << gameName << "," << playerName << "," << score << "\n";
        outFile.close();
    }
}

std::vector<ScoreEntry> Database::getTopScores(const std::string& gameName, bool higherIsBetter, int limit) {
    std::vector<ScoreEntry> scores;
    std::ifstream inFile(filename);
    std::string line;

    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string gName, pName, scoreStr;
            
            std::getline(ss, gName, ',');
            std::getline(ss, pName, ',');
            std::getline(ss, scoreStr, ',');

            // Only grab scores for the currently requested game
            if (gName == gameName) {
                try {
                    scores.push_back({pName, std::stoi(scoreStr)});
                } catch (...) {
                    // Ignore corrupted lines
                }
            }
        }
        inFile.close();
    }

    // Sort the scores based on the game's rules
    if (higherIsBetter) {
        std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });
    } else {
        std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score < b.score;
        });
    }

    // Shrink the list to only the top 'limit' (e.g., Top 3)
    if (scores.size() > static_cast<size_t>(limit)) {
        scores.resize(limit);
    }

    return scores;
}