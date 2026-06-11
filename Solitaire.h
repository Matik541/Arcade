#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#include "Game.h"
#include <vector>
#include <string>

struct SolCard {
    std::string rank;
    std::string suit;
    int value; // 1 (Ace) to 13 (King)
    int color; // 0 = Black, 1 = Red
    bool faceUp;
};

class Solitaire : public Game {
private:
    int drawMode; // 1 or 3
    int pCursorX, pCursorY; // Y = -1 is Top Bar. Y >= 0 is Tableau.
    
    // Selection Tracker (-1 = Nothing selected)
    int selArea; // 0 = Waste, 1 = Tableau, 2 = Foundation
    int selX;
    int selY;

    std::vector<SolCard> stock;
    std::vector<SolCard> waste;
    std::vector<std::vector<SolCard>> foundations; // 4 piles
    std::vector<std::vector<SolCard>> tableau;     // 7 columns

    // Builds the draw-mode selection menu.
    bool setupOptions();
    // Shuffles a full deck into the stock pile.
    void buildAndShuffleDeck();
    // Sets up a new Klondike layout.
    void resetBoard();
    // Draws one or more cards from stock into waste.
    void drawCardsFromStock();
    // Snaps the cursor to the first visible card in the current column.
    void snapCursorToRevealed();
    // Reveals the top card of each tableau pile if needed.
    void autoReveal();
    
    // Validates a move onto a tableau pile.
    bool isValidTableauMove(const SolCard& src, int targetCol);
    // Validates a move onto a foundation pile.
    bool isValidFoundationMove(const SolCard& src, int fIndex);
    
    // Draws the full solitaire table.
    void drawTable();
    // Formats a card with cursor and selection highlights.
    std::string formatCard(const SolCard& c, bool isCursor, bool isSelected, bool isStackSelected);
    // Checks whether all foundations are complete.
    bool checkWin();

public:
    // Sets up the Solitaire game state and metadata.
    Solitaire();
    // Runs the full Solitaire gameplay loop.
    void play() override;
};

#endif