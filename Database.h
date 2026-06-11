#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

/**
 * @struct ScoreEntry
 * @brief Struktura reprezentująca wpis wyniku w bazie danych.
 */
struct ScoreEntry {
    std::string playerName;  ///< Nazwa gracza.
    int score;               ///< Osiągnięty wynik.
};

/**
 * @class Database
 * @brief Klasa obsługująca system zapisu i odczytu wyników z pliku tekstowego.
 * @details Zaimplementowana jako wzorzec projektowy Singleton w celu globalnego i unikalnego dostępu do bazy.
 */
class Database {
private:
    std::string filename;    ///< Nazwa pliku z wynikami.
    
    /**
     * @brief Prywatny konstruktor klasy Database w celu realizacji wzorca Singleton.
     * @param file Ścieżka do pliku bazy danych.
     */
    Database(const std::string& file) : filename(file) {}

public:
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    /**
     * @brief Pobiera globalną instancję bazy danych.
     * @return Referencja do jedynej instancji klasy Database.
     */
    static Database& getInstance() {
        static Database instance("scores.txt");
        return instance;
    }
    
    /**
     * @brief Zapisuje wynik do pliku.
     * @param gameName Nazwa gry.
     * @param playerName Nazwa gracza.
     * @param score Wynik gracza.
     * @param higherIsBetter Czy wyższy wynik jest lepszy.
     */
    void saveScore(const std::string& gameName, const std::string& playerName, int score, bool higherIsBetter);

    /**
     * @brief Pobiera listę najlepszych wyników dla danej gry.
     * @param gameName Nazwa gry.
     * @param higherIsBetter Czy wyższy wynik jest lepszy (sortowanie malejąco/rosnąco).
     * @param limit Maksymalna liczba zwracanych wyników (domyślnie 3).
     * @return Wektor struktur ScoreEntry zawierający najlepsze wyniki.
     */
    std::vector<ScoreEntry> getTopScores(const std::string& gameName, bool higherIsBetter, int limit = 3);
};

#endif