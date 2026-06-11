#ifndef GAME2048_H
#define GAME2048_H

#include "Game.h"
#include <vector>

/**
 * @class Game2048
 * @brief Klasa implementująca grę logiczną 2048 w konsoli.
 * @details Obsługuje standardowe przesuwanie planszy w 4 kierunkach, łączenie kafelków, punktację oraz opcjonalny tryb nieskończony.
 */
class Game2048 : public Game {
private:
    int size;                            ///< Rozmiar planszy (zazwyczaj 4x4).
    std::vector<std::vector<int>> board; ///< Macierz reprezentująca kafelki na planszy.
    int score;                           ///< Bieżący wynik gracza.
    bool hasWon;                         ///< Flaga oznaczająca, czy gracz osiągnął kafelek 2048.
    bool endlessMode;                    ///< Flaga trybu nieskończonego (kontynuacja po 2048).

    /**
     * @brief Konfiguruje opcje gry przed startem.
     * @return true Jeśli ustawiono pomyślnie, false w przypadku wyjścia.
     */
    bool setupOptions();

    /**
     * @brief Resetuje planszę i zmienne stanu do wartości domyślnych.
     */
    void resetBoard();

    /**
     * @brief Losuje i wstawia nowy kafelek (2 lub 4) na wolnym miejscu planszy.
     */
    void spawnTile();

    /**
     * @brief Rysuje aktualny stan planszy 2048 w konsoli z kolorowaniem wartości.
     * @param gameOver Flaga zakończenia gry.
     */
    void drawBoard(bool gameOver);
    
    /**
     * @brief Przesuwa i łączy elementy w pojedynczym wierszu/kolumnie.
     * @param line Referencja do wektora elementów wiersza/kolumny.
     * @return true Jeśli nastąpił jakikolwiek ruch lub łączenie.
     */
    bool slideLine(std::vector<int>& line); 

    /**
     * @brief Przesuwa kafelki w zadanym kierunku (0-góra, 1-dół, 2-lewo, 3-prawo).
     * @param direction Kod kierunku ruchu.
     * @return true Jeśli nastąpiła zmiana stanu planszy.
     */
    bool move(int direction);

    /**
     * @brief Sprawdza, czy na planszy są możliwe jakiekolwiek ruchy.
     * @return true Jeśli nie ma wolnych miejsc i żadne sąsiednie kafelki nie mogą się połączyć.
     */
    bool isGameOver();

public:
    /**
     * @brief Konstruktor klasy Game2048.
     */
    Game2048();

    /**
     * @brief Uruchamia rozgrywkę 2048.
     */
    void play() override;
};

#endif