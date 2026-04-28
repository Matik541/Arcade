#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "../Game.h"
#include <vector>
#include <string>

struct Card {
    std::string rank;
    std::string suit;
    int value; // 11 for Ace, 10 for Face cards, etc.
};

class Blackjack : public Game {
private:
    std::vector<Card> deck;
    std::vector<Card> playerHand;
    std::vector<Card> dealerHand;
    
    int bankroll;
    int lastBet;
    int currentBet;
    int startingBankroll;
    bool isScored;

    void buildAndShuffleDeck();
    Card drawCard();
    int getHandValue(const std::vector<Card>& hand);
    
    bool setupOptions();
    void drawTable(bool hideDealerCard, const std::string& message = "");
    int bettingPhase(); 

public:
    Blackjack();
    void play() override;
};

#endif