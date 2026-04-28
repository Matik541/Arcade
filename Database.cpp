#include "Database.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

FileDatabase::FileDatabase(std::string file) : filename(file) {}

// Helper: Read the entire file into a map
std::map<std::string, std::vector<ScoreEntry>> FileDatabase::loadAllScores() {
    std::map<std::string, std::vector<ScoreEntry>> db;
    std::ifstream file(filename);
    
    if (!file.is_open()) return db; // File doesn't exist yet, return empty db

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string game, player, scoreStr;
        
        // Parse CSV format: GameName,PlayerName,Score
        if (std::getline(ss, game, ',') && 
            std::getline(ss, player, ',') && 
            std::getline(ss, scoreStr, ',')) {
            db[game].push_back({player, std::stoi(scoreStr)});
        }
    }
    file.close();
    return db;
}

// Helper: Overwrite the file with the updated map
void FileDatabase::saveAllScores(const std::map<std::string, std::vector<ScoreEntry>>& allData) {
    std::ofstream file(filename);
    for (const auto& pair : allData) {
        for (const auto& entry : pair.second) {
            file << pair.first << "," << entry.playerName << "," << entry.score << "\n";
        }
    }
    file.close();
}

std::vector<ScoreEntry> FileDatabase::getTop5(const std::string& gameName, bool higherIsBetter) {
    auto db = loadAllScores();
    return db[gameName]; // It's already sorted and trimmed from when it was saved
}

void FileDatabase::saveScore(const std::string& gameName, const std::string& player, int score, bool higherIsBetter) {
    auto db = loadAllScores();
    
    // Add the new score
    db[gameName].push_back({player, score});
    
    // Sort dynamically based on the game's rule
    std::sort(db[gameName].begin(), db[gameName].end(), 
        [higherIsBetter](const ScoreEntry& a, const ScoreEntry& b) {
            if (higherIsBetter) return a.score > b.score; // DESC (e.g., 2048)
            else return a.score < b.score;                // ASC (e.g., MineSweeper)
        });

    // Keep only top 5
    if (db[gameName].size() > 5) {
        db[gameName].pop_back();
    }

    // Save everything back to the file
    saveAllScores(db);
}