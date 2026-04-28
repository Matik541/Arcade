#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <map>

struct ScoreEntry {
    std::string playerName;
    int score;
};

// The Interface (Contract)
class IDatabase {
public:
    virtual ~IDatabase() = default;
    // Both functions now take the sorting rule
    virtual std::vector<ScoreEntry> getTop5(const std::string& gameName, bool higherIsBetter) = 0;
    virtual void saveScore(const std::string& gameName, const std::string& player, int score, bool higherIsBetter) = 0;
};

// The Local File Implementation
class FileDatabase : public IDatabase {
private:
    std::string filename;
    std::map<std::string, std::vector<ScoreEntry>> loadAllScores();
    void saveAllScores(const std::map<std::string, std::vector<ScoreEntry>>& allData);

public:
    FileDatabase(std::string file = "scores.txt");
    std::vector<ScoreEntry> getTop5(const std::string& gameName, bool higherIsBetter) override;
    void saveScore(const std::string& gameName, const std::string& player, int score, bool higherIsBetter) override;
};

#endif