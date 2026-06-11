#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#include "Game.h"
#include <vector>
#include <string>

/**
 * @struct SolCard
 * @brief Struktura reprezentująca pojedynczą kartę w grze Pasjans (Solitaire).
 */
struct SolCard {
    std::string rank; ///< Ranga karty (np. "A", "2", "K").
    std::string suit; ///< Kolor/symbol karty (np. "♠", "♥", "♦", "♣").
    int value;        ///< Wartość numeryczna: 1 (As) do 13 (Król).
    int color;        ///< Barwa karty: 0 = Czarna (Pik/Trefl), 1 = Czerwona (Kier/Karo).
    bool faceUp;      ///< Czy karta leży koszulką do dołu (jest widoczna).
};

/**
 * @class Solitaire
 * @brief Klasa implementująca klasycznego Pasjansa Klondike w konsoli.
 * @details Sterowanie odbywa się interaktywnym kursorem celownika z możliwością zaznaczania stref tableau, waste oraz fundamentów.
 */
class Solitaire : public Game {
private:
    int drawMode;           ///< Tryb ciągnienia kart: 1 lub 3 karty na raz.
    int pCursorX, pCursorY; ///< Współrzędne kursora (Y = -1 oznacza górną sekcję, Y >= 0 to kolumny Tableau).
    
    int selArea;            ///< Indeks wybranej sekcji źródłowej (-1=brak, 0=Waste, 1=Tableau, 2=Fundamenty).
    int selX;               ///< Koordynat X wybranego źródła (np. numer kolumny).
    int selY;               ///< Koordynat Y wybranego źródła (np. indeks karty w kolumnie).

    std::vector<SolCard> stock;                 ///< Stos stock (zakryty).
    std::vector<SolCard> waste;                 ///< Stos waste (odkryte pociągnięte karty).
    std::vector<std::vector<SolCard>> foundations; ///< Cztery stosy fundamentów (docelowe dla Asów do Króli).
    std::vector<std::vector<SolCard>> tableau;     ///< Siedem kolumn roboczych planszy.

    /**
     * @brief Konfiguruje tryb gry (ciągnięcie 1 lub 3 kart).
     * @return true Jeśli ustawiono poprawnie.
     */
    bool setupOptions();

    /**
     * @brief Tworzy talię 52 kart i losowo ją tasuje w stosie stock.
     */
    void buildAndShuffleDeck();

    /**
     * @brief Resetuje planszę i rozdaje karty w układzie Klondike.
     */
    void resetBoard();

    /**
     * @brief Ciągnie karty ze stosu stock do stosu waste.
     */
    void drawCardsFromStock();

    /**
     * @brief Przesuwa kursor do pierwszej widocznej (odkrytej) karty w wybranej kolumnie tableau.
     */
    void snapCursorToRevealed();

    /**
     * @brief Automatycznie odkrywa wierzchnią kartę w kolumnie tableau, jeśli jest zakryta.
     */
    void autoReveal();
    
    /**
     * @brief Sprawdza, czy ruch karty na kolumnę Tableau jest zgodny z zasadami (naprzemienne kolory, malejąco).
     */
    bool isValidTableauMove(const SolCard& src, int targetCol);

    /**
     * @brief Sprawdza, czy ruch karty na stos Fundamentów jest poprawny (ten sam kolor, rosnąco od Asa).
     */
    bool isValidFoundationMove(const SolCard& src, int fIndex);
    
    /**
     * @brief Rysuje całą planszę pasjansa na ekranie.
     */
    void drawTable();

    /**
     * @brief Formatuje graficzną reprezentację karty (podświetlenie kursora/zaznaczenia).
     */
    std::string formatCard(const SolCard& c, bool isCursor, bool isSelected, bool isStackSelected);

    /**
     * @brief Sprawdza, czy wszystkie 4 stosy fundamentów są kompletne.
     * @return true Jeśli gracz ułożył pasjansa.
     */
    bool checkWin();

public:
    /**
     * @brief Konstruktor gry Solitaire.
     */
    Solitaire();

    /**
     * @brief Uruchamia rozgrywkę Pasjansa.
     */
    void play() override;
};

#endif