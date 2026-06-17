#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "Game.h"
#include <vector>
#include <ctime>

/**
 * @class MineSweeper
 * @brief Klasa implementująca klasyczną grę Saper (MineSweeper) w konsoli.
 * @details Wspiera ruch kursorem, oznaczanie flagami, pytajnikami oraz zaawansowaną mechanikę chordingu (szybkiego odkrywania pól sąsiednich).
 */
class MineSweeper : public Game {
private:
    friend void runArcadeTests();
    int width, height, numMines;  ///< Wymiary planszy i liczba min.
    int cursorX, cursorY;         ///< Aktualne współrzędne kursora.
    int difficulty;               ///< Wybrany poziom trudności (1-4).
    bool firstClick;              ///< Zapewnia, że pierwszy ruch nigdy nie trafi na minę.
    time_t startTime;             ///< Czas rozpoczęcia gry w sekundach.

    std::vector<std::vector<int>> board;       ///< Siatka planszy: -1 oznacza minę, >=0 to liczba min w sąsiedztwie.
    std::vector<std::vector<bool>> revealed;   ///< Czy dane pole zostało już odkryte.
    std::vector<std::vector<int>> cellState;   ///< Stan wyświetlania komórki: 0=ukryta, 1=flaga, 2=pytajnik.

    /**
     * @brief Wyświetla menu ustawień i konfiguracji planszy.
     * @return true Jeśli wybrano parametry pomyślnie, false w przypadku wyjścia.
     */
    bool setupOptions();

    /**
     * @brief Resetuje planszę i zmienne stanu gry do stanu początkowego.
     */
    void resetBoard();

    /**
     * @brief Rozmieszcza miny na planszy z pominięciem bezpiecznej strefy wokół pierwszego kliknięcia.
     * @param safeX Współrzędna X bezpiecznego kliknięcia.
     * @param safeY Współrzędna Y bezpiecznego kliknięcia.
     */
    void placeMines(int safeX, int safeY);

    /**
     * @brief Oblicza liczby min sąsiadujących dla każdego wolnego pola planszy.
     */
    void calculateNumbers();

    /**
     * @brief Rysuje całą planszę Sapera w terminalu.
     * @param gameOver Flaga końca gry (odsłania wszystkie miny).
     * @param isWin Czy gra została wygrana.
     */
    void drawBoard(bool gameOver, bool isWin);

    /**
     * @brief Rekurencyjnie odkrywa pole planszy i sąsiednie puste pola (powódź).
     * @param x Współrzędna X pola.
     * @param y Współrzędna Y pola.
     */
    void revealCell(int x, int y);

    /**
     * @brief Mechanika chordingu - odkrywa wszystkie nieoznaczone pola wokół odkrytej komórki, jeśli liczba flag się zgadza.
     * @param x Współrzędna X.
     * @param y Współrzędna Y.
     * @return true Jeśli ruch był bezpieczny, false jeśli trafiono na minę.
     */
    bool chordCell(int x, int y);

    /**
     * @brief Sprawdza, czy gracz odkrył wszystkie pola oprócz min.
     * @return true Jeśli gracz wygrał.
     */
    bool checkWin();

public:
    /**
     * @brief Konstruktor klasy MineSweeper.
     */
    MineSweeper();

    /**
     * @brief Uruchamia rozgrywkę Sapera.
     */
    void play() override;
};

#endif