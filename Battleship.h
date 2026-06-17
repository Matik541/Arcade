#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include "Game.h"
#include <vector>
#include <string>

/**
 * @struct ShipData
 * @brief Struktura reprezentująca dane o pojedynczym okręcie w grze Statki.
 */
struct ShipData {
    std::string name; ///< Nazwa typu okrętu (np. "Masztowiec", "Pancernik").
    int length;       ///< Długość okrętu (liczba pól).
    bool sunk;        ///< Flaga określająca, czy okręt został zatopiony.
    int startX;       ///< Pozycja początkowa X okrętu.
    int startY;       ///< Pozycja początkowa Y okrętu.
    bool horizontal;  ///< Czy okręt leży poziomo (true) czy pionowo (false).
};

/**
 * @class Battleship
 * @brief Klasa implementująca grę w Statki (Battleship) z botem.
 * @details Obsługuje ręczne/automatyczne rozmieszczanie statków oraz ruchy gracza i AI na planszy 10x10.
 */
class Battleship : public Game {
private:
    friend void runArcadeTests();
    int botDifficulty;         ///< Poziom trudności bota (1 - losowy, 2 - polujący na trafienia).
    bool manualPlacement;       ///< Czy gracz wybrał ręczne rozmieszczanie statków.
    int pCursorX, pCursorY;    ///< Koordynaty kursora celownika gracza.
    std::string actionMessage;  ///< Ostatni komunikat o wykonanej akcji (np. trafiony/pudło).
    
    std::vector<std::vector<int>> playerBoard; ///< Plansza statków gracza (identyfikatory statków lub 0).
    std::vector<std::vector<int>> botBoard;    ///< Plansza statków bota.
 
    std::vector<std::vector<int>> playerState; ///< Stan strzałów gracza: 0=nieodkryte, 1=pudło, 2=trafienie.
    std::vector<std::vector<int>> botState;    ///< Stan strzałów bota.

    std::vector<ShipData> pShips; ///< Flota okrętów gracza.
    std::vector<ShipData> bShips; ///< Flota okrętów bota.

    std::vector<std::vector<bool>> botFired;        ///< Zapamiętane współrzędne, na które strzelał bot.
    std::vector<std::pair<int, int>> botTargetQueue; ///< Kolejka pól docelowych bota (do śledzenia trafionego okrętu).

    /**
     * @brief Konfiguruje ustawienia gry przed startem.
     * @return true Jeśli skonfigurowano pomyślnie.
     */
    bool setupOptions();

    /**
     * @brief Resetuje plansze i ustawienia flot przed nową rozgrywką.
     */
    void resetBoards();

    /**
     * @brief Sprawdza, czy można umieścić statek na danej planszy w określonej pozycji.
     */
    bool canPlaceShip(const std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal);

    /**
     * @brief Umieszcza statek na planszy, ustawiając jego ID.
     */
    void placeShip(std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal, int shipId);

    /**
     * @brief Automatycznie (losowo) rozmieszcza statki na planszy.
     */
    void autoPlaceShips(std::vector<std::vector<int>>& board, std::vector<ShipData>& fleet);

    /**
     * @brief Uruchamia interaktywny proces ręcznego rozmieszczania statków przez gracza za pomocą WASD.
     */
    void manualPlaceShips();
    
    /**
     * @brief Zwraca reprezentację graficzną części statku.
     */
    std::string getShipPart(int x, int y, int startX, int startY, int length, bool horizontal);
    
    /**
     * @brief Rysuje dwie plansze (gracza i radaru/strzałów) obok siebie.
     */
    void drawBoards();

    /**
     * @brief Wykonuje turę bota (losową lub z algorytmem poszukiwania).
     * @return Komunikat opisujący ruch bota (np. "Bot trafia w Twój Pancernik!").
     */
    std::string botTurn();

    /**
     * @brief Sprawdza, czy wszystkie okręty we flocie zostały zatopione.
     * @param fleet Flota statków do sprawdzenia.
     * @return true Jeśli flota została w całości zatopiona.
     */
    bool checkWin(const std::vector<ShipData>& fleet);

public:
    /**
     * @brief Konstruktor gry Battleship.
     */
    Battleship();

    /**
     * @brief Uruchamia rozgrywkę w Statki.
     */
    void play() override;
};

#endif