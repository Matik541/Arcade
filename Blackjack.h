#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "Game.h"
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

    // Builds and shuffles a fresh shoe of cards.
    void buildAndShuffleDeck();
    // Removes the top card from the deck, reshuffling if needed.
    Card drawCard();
    // Computes the best blackjack value for one hand.
    int getHandValue(const std::vector<Card>& hand);
    
    // Builds the bankroll selection screen.
    bool setupOptions();
    // Draws the table layout, hands, and optional message.
    void drawTable(bool hideDealerCard, const std::string& message = "");
    // Runs the betting screen and returns the selected wager.
    int bettingPhase(); 

public:
    // Sets up the Blackjack game state and metadata.
    Blackjack();
    // Runs the full Blackjack loop across rounds and bets.
    void play() override;
};

#endif