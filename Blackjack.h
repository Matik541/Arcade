#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "Game.h"
#include <vector>
#include <string>

/**
 * @struct Card
 * @brief Struktura reprezentująca kartę w grze Blackjack.
 */
struct Card {
    std::string rank; ///< Ranga karty (np. "2", "10", "A", "K").
    std::string suit; ///< Kolor/symbol karty (np. "♥", "♦", "♣", "♠").
    int value;        ///< Wartość punktowa w Blackjacku (np. 11 dla Asa, 10 dla figur).
};

/**
 * @class Blackjack
 * @brief Klasa implementująca grę w Blackjacka (Oczko) w konsoli.
 * @details Obsługuje obstawianie zakładów, dobieranie kart (Hit), pasowanie (Stand) oraz sztuczną inteligencję krupiera.
 */
class Blackjack : public Game {
private:
    friend void runArcadeTests();
    std::vector<Card> deck;       ///< Talia kart używana w bieżącej rundzie.
    std::vector<Card> playerHand; ///< Ręka gracza.
    std::vector<Card> dealerHand; ///< Ręka krupiera.
    
    int bankroll;                 ///< Aktualny stan konta gracza (żetony).
    int lastBet;                  ///< Wysokość ostatniego zakładu.
    int currentBet;               ///< Wysokość bieżącego zakładu w tej rundzie.
    int startingBankroll;         ///< Początkowy stan konta wybrany przez gracza.
    bool isScored;                ///< Czy gra ma zapisywać najlepsze wyniki (czas/stan konta).

    /**
     * @brief Tworzy standardową talię 52 kart i tasuje ją.
     */
    void buildAndShuffleDeck();

    /**
     * @brief Pobiera kartę z góry talii.
     * @return Wylosowany obiekt struktury Card.
     */
    Card drawCard();

    /**
     * @brief Oblicza łączną wartość punktową kart na ręce, optymalizując wartość Asów (1 lub 11).
     * @param hand Referencja do wektora kart na ręce.
     * @return Sumaryczna wartość punktowa ręki.
     */
    int getHandValue(const std::vector<Card>& hand);

    /**
     * @brief Konfiguruje początkowe opcje gry (np. budżet początkowy).
     * @return true Jeśli wybrano parametry pomyślnie, false w przypadku wyjścia.
     */
    bool setupOptions();

    /**
     * @brief Rysuje aktualny stół do gry z kartami krupiera i gracza oraz komunikatami.
     * @param hideDealerCard Czy ukrywać pierwszą kartę krupiera.
     * @param message Dodatkowy komunikat dla gracza (np. "Hit or Stand?").
     */
    void drawTable(bool hideDealerCard, const std::string& message = "");

    /**
     * @brief Przeprowadza fazę obstawiania zakładu przed rundą.
     * @return Wysokość obstawionego zakładu lub 0 w przypadku wyjścia.
     */
    int bettingPhase(); 

public:
    /**
     * @brief Konstruktor klasy Blackjack.
     */
    Blackjack();

    /**
     * @brief Uruchamia rozgrywkę Blackjacka.
     */
    void play() override;
};

#endif