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

    bool setupOptions();
    void buildAndShuffleDeck();
    void resetBoard();
    void drawCardsFromStock();
    void snapCursorToRevealed();
    void autoReveal();
    
    bool isValidTableauMove(const SolCard& src, int targetCol);
    bool isValidFoundationMove(const SolCard& src, int fIndex);
    
    void drawTable();
    std::string formatCard(const SolCard& c, bool isCursor, bool isSelected, bool isStackSelected);
    bool checkWin();

public:
    Solitaire();
    void play() override;
};

#endif