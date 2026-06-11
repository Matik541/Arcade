#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "Game.h"
#include <vector>

/**
 * @class TicTacToe
 * @brief Klasa implementująca klasyczną grę Kółko i Krzyżyk (Tic-Tac-Toe) na planszy 3x3.
 * @details Umożliwia rozgrywkę dla dwóch graczy na jednym komputerze lub grę jednoosobową z botem o 3 poziomach trudności (Minimax).
 */
class TicTacToe : public Game {
private:
    std::vector<char> board;  ///< Plansza do gry reprezentowana jako wektor 9 znaków.
    char currentPlayer;       ///< Znak gracza, którego jest ruch ('X' lub 'O').
    bool vsBot;               ///< Flaga określająca, czy gra toczy się przeciwko botowi.
    char botPiece;            ///< Znak, którym gra bot ('X' lub 'O').
    char playerPiece;         ///< Znak, którym gra człowiek w trybie vs BOT ('X' lub 'O').
    int botDifficulty;        ///< Poziom trudności bota (1 = Łatwy, 2 = Średni, 3 = Trudny).
    int cursorX;              ///< Aktualna współrzędna X kursora (0-2).
    int cursorY;              ///< Aktualna współrzędna Y kursora (0-2).

    /**
     * @brief Resetuje stan planszy i ustawia gracza rozpoczynającego.
     */
    void resetBoard();

    /**
     * @brief Rysuje powiększoną planszę do gry na ekranie z zaznaczeniem kursora.
     */
    void drawBoard();
    
    /**
     * @brief Sprawdza, czy na planszy występuje remis.
     * @param b Plansza do sprawdzenia.
     * @return true Jeśli nie ma wolnych pól i nikt nie wygrał.
     */
    bool checkDraw(const std::vector<char>& b);

    /**
     * @brief Wykonuje ruch bota w zależności od poziomu trudności.
     */
    void botMove();

    /**
     * @brief Wybiera losowy wolny ruch.
     * @return Indeks pola (0-8) lub -1.
     */
    int getRandomMove();

    /**
     * @brief Szuka ruchu, który natychmiast wygrywa lub blokuje przeciwnika.
     * @param piece Znak do sprawdzenia.
     * @return Indeks pola (0-8) lub -1.
     */
    int getWinningOrBlockingMove(char piece);

    /**
     * @brief Wyznacza optymalny ruch bota przy użyciu algorytmu minimax.
     * @return Optymalny indeks pola (0-8).
     */
    int getBestMove();

    /**
     * @brief Rekurencyjna funkcja algorytmu Minimax służąca do oceny stanów gry.
     * @param b Kopia planszy gry.
     * @param depth Aktualna głębokość drzewa gry.
     * @param isMaximizing Czy szukamy maksimum (ruch bota), czy minimum.
     * @return Ocena punktowa stanu gry.
     */
    int minimax(std::vector<char>& b, int depth, bool isMaximizing);

    /**
     * @brief Wyświetla menu konfiguracji rozgrywki (wybór trybu, trudności i koloru).
     * @return true Jeśli konfiguracja przebiegła pomyślnie, false jeśli wybrano wyjście.
     */
    bool setupOptions(); 

public:
    /**
     * @brief Tworzy nowy obiekt gry TicTacToe i ustawia metadane gry.
     */
    TicTacToe();

    /**
     * @brief Uruchamia główną pętlę gry TicTacToe.
     */
    void play() override;

    /**
     * @brief Sprawdza, czy dany gracz wygrał grę na podanej planszy.
     * @param b Plansza do sprawdzenia.
     * @param player Znak gracza do sprawdzenia.
     * @return true Jeśli gracz wygrał.
     * @return false W przeciwnym wypadku.
     */
    bool checkWin(const std::vector<char>& b, char player);
};

#endif