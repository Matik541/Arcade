#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct ScoreEntry {
    std::string playerName;
    int score;
};

// Abstract Base Class (Interface)
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual std::vector<ScoreEntry> getTop5(const std::string& gameName) = 0;
    virtual void saveScore(const std::string& gameName, const std::string& player, int score) = 0;
};

class MockDatabase : public IDatabase {
private:
    std::map<std::string, std::vector<ScoreEntry>> memoryDb;
public:
    std::vector<ScoreEntry> getTop5(const std::string& gameName) override;
    void saveScore(const std::string& gameName, const std::string& player, int score) override;
};

#endif