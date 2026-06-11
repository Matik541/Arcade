#ifndef SNAKE_H
#define SNAKE_H

#include "Game.h"
#include <deque>
#include <utility>

/**
 * @class Snake
 * @brief Klasa obsługująca klasyczną grę Snake w terminalu.
 * @details Sterowanie odbywa się w czasie rzeczywistym z regulowanym opóźnieniem ruchu.
 */
class Snake : public Game {
private:
    int width, height;            ///< Wymiary planszy.
    int speedMs;                  ///< Prędkość gry (opóźnienie pętli w ms).
    int score;                    ///< Aktualny wynik.
    bool isScored;                ///< Czy gra zapisuje wynik w tabeli rekordów.

    int dirX, dirY;               ///< Kierunek ruchu węża (X, Y).
    int foodX, foodY;             ///< Pozycja jedzenia na planszy (X, Y).
    
    std::deque<std::pair<int, int>> snake; ///< Segmenty ciała węża (kolejka par współrzędnych).

    /**
     * @brief Konfiguruje ustawienia gry (np. poziom trudności/prędkość).
     * @return true Jeśli ustawiono poprawnie, false jeśli przerwano.
     */
    bool setupOptions();

    /**
     * @brief Resetuje stan gry do wartości domyślnych przed nowym startem.
     */
    void resetGame();

    /**
     * @brief Losuje nową pozycję jedzenia na planszy, unikając kolizji z wężem.
     */
    void spawnFood();

    /**
     * @brief Rysuje planszę gry z wężem i jedzeniem w konsoli.
     * @param gameOver Czy gra się zakończyła.
     */
    void drawBoard(bool gameOver);

public:
    /**
     * @brief Konstruktor klasy Snake.
     */
    Snake();

    /**
     * @brief Uruchamia grę Snake.
     */
    void play() override;
};

#endif