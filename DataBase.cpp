#include "Database.h"

std::vector<ScoreEntry> MockDatabase::getTop5(const std::string& gameName) {
    // Mock data - in a real app, query your SQL/JSON here
    if (gameName == "Minesweeper") return { {"Alice", 500}, {"Bob", 450}, {"Charlie", 300} };
    if (gameName == "2048") return { {"Dave", 20480}, {"Eve", 10240} };
    return {}; // No scores for others yet
}

void MockDatabase::saveScore(const std::string& gameName, const std::string& player, int score) {
    // Implement save logic here
}