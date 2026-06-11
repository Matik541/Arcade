#include "Solitaire.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

Solitaire::Solitaire() : Game("Solitaire", "Classic Klondike. Stack suits A to K to win!", false, false) {}

bool Solitaire::setupOptions() {
    int sel = 0;
    std::string diffs[] = {"Standard (Draw 1 Card)", "Expert (Draw 3 Cards)"};
    
    while (true) {
        Display::clearScreen();
        Display::printColored("--- SOLITAIRE SETUP ---\n\n", Color::YELLOW);
        std::cout << "Select Difficulty:\n";
        for (int i = 0; i < 2; i++) {
            if (i == sel) Display::printColored("> " + diffs[i] + "\n", Color::GREEN);
            else std::cout << "  " + diffs[i] << "\n";
        }
        std::cout << "\n[W/S] Navigate | [SPACE/ENTER] Select | [Q] Quit\n> ";

        int input = getInput();
        if (input == 'Q') return false;
        if (input == 'W' && sel > 0) sel--;
        if (input == 'S' && sel < 1) sel++;
        if (input == ' ' || input == '\r' || input == '\n') {
            drawMode = (sel == 0) ? 1 : 3;
            break;
        }
    }
    return true;
}

void Solitaire::buildAndShuffleDeck() {
    stock.clear();
    std::string suits[] = {"♥", "♦", "♣", "♠"};
    int colors[] = {1, 1, 0, 0}; // 1=Red, 0=Black
    std::string ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    for (int s = 0; s < 4; ++s) {
        for (int r = 0; r < 13; ++r) {
            stock.push_back({ranks[r], suits[s], r + 1, colors[s], false});
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(stock.begin(), stock.end(), std::default_random_engine(seed));
}

void Solitaire::resetBoard() {
    buildAndShuffleDeck();
    waste.clear();
    foundations.assign(4, std::vector<SolCard>());
    tableau.assign(7, std::vector<SolCard>());

    // Deal Klondike Style
    for (int col = 0; col < 7; col++) {
        for (int row = 0; row <= col; row++) {
            tableau[col].push_back(stock.back());
            stock.pop_back();
        }
        tableau[col].back().faceUp = true; // Reveal top card
    }

    pCursorX = 0;
    pCursorY = 0;
    selArea = -1; // Nothing selected
    snapCursorToRevealed();
}

void Solitaire::drawCardsFromStock() {
    if (stock.empty()) {
        // Recycle waste back into stock
        while (!waste.empty()) {
            stock.push_back(waste.back());
            stock.back().faceUp = false;
            waste.pop_back();
        }
    } else {
        // Draw cards
        int toDraw = std::min(drawMode, (int)stock.size());
        for (int i = 0; i < toDraw; i++) {
            waste.push_back(stock.back());
            waste.back().faceUp = true;
            stock.pop_back();
        }
    }
}

void Solitaire::snapCursorToRevealed() {
    if (pCursorY >= 0) { // If in Tableau
        int firstFaceUp = 0;
        for (size_t i = 0; i < tableau[pCursorX].size(); i++) {
            if (tableau[pCursorX][i].faceUp) {
                firstFaceUp = i;
                break;
            }
        }
        pCursorY = tableau[pCursorX].empty() ? 0 : firstFaceUp;
    }
}

void Solitaire::autoReveal() {
    for (int i = 0; i < 7; i++) {
        if (!tableau[i].empty() && !tableau[i].back().faceUp) {
            tableau[i].back().faceUp = true;
        }
    }
}

bool Solitaire::isValidTableauMove(const SolCard& src, int targetCol) {
    if (tableau[targetCol].empty()) return src.value == 13; // Only Kings on empty cols
    SolCard target = tableau[targetCol].back();
    return (src.color != target.color && src.value == target.value - 1);
}

bool Solitaire::isValidFoundationMove(const SolCard& src, int fIndex) {
    if (foundations[fIndex].empty()) return src.value == 1; // Only Aces on empty foundations
    SolCard target = foundations[fIndex].back();
    return (src.suit == target.suit && src.value == target.value + 1);
}

std::string Solitaire::formatCard(const SolCard& c, bool isCursor, bool isSelected, bool isStackSelected) {
    if (!c.faceUp) return "[ #### ]";
    
    std::string tColor = (c.color == 1) ? Color::RED : Color::CYAN;
    std::string pad = (c.rank.length() == 1) ? " " : "";
    
    std::string bracketL = "[";
    std::string bracketR = "]";
    std::string bColor = Color::RESET;

    if (isSelected || isStackSelected) bColor = Color::MAGENTA;
    else if (isCursor) bColor = Color::GREEN;

    return bColor + bracketL + " " + tColor + pad + c.rank + " " + c.suit + " " + bColor + bracketR + Color::RESET;
}

void Solitaire::drawTable() {
    Display::clearScreen();
    Display::printColored("=== KLONDIKE SOLITAIRE ===\n\n", Color::YELLOW);

    // --- TOP BAR ROW (Y = -1) ---
    std::cout << " ";
    // Col 0: Stock (Not selectable, strictly hotkey)
    if (stock.empty()) std::cout << "[ R:   ]  ";
    else std::cout << "[ R:## ]  ";

    // Col 1: Waste
    if (waste.empty()) {
        std::cout << (pCursorY == -1 && pCursorX == 1 ? Color::GREEN + "[      ]" + Color::RESET : "[      ]") << "  ";
    } else {
        std::cout << formatCard(waste.back(), (pCursorY == -1 && pCursorX == 1), (selArea == 0), false) << "  ";
    }

    // Col 2: Empty Space Gap
    std::cout << "          ";

    // Col 3-6: Foundations
    for (int i = 0; i < 4; i++) {
        int fX = i + 3;
        bool isCursor = (pCursorY == -1 && pCursorX == fX);
        bool isSel = (selArea == 2 && selX == i);
        
        if (foundations[i].empty()) {
            std::string emptyStr = "[  " + std::string("♥♦♣♠").substr(i*3, 3) + "  ]"; // UTF-8 suits are 3 bytes
            std::cout << (isCursor ? Color::GREEN + emptyStr + Color::RESET : emptyStr) << "  ";
        } else {
            std::cout << formatCard(foundations[i].back(), isCursor, isSel, false) << "  ";
        }
    }
    std::cout << "\n\n";

    // --- TABLEAU ---
    int maxHeight = 0;
    for (int i = 0; i < 7; i++) {
        if (tableau[i].size() > maxHeight) maxHeight = tableau[i].size();
    }
    if (maxHeight == 0) maxHeight = 1; // Draw at least one empty row

    for (int y = 0; y < maxHeight; y++) {
        std::cout << " ";
        for (int x = 0; x < 7; x++) {
            if (y < tableau[x].size()) {
                bool isCursor = (pCursorY == y && pCursorX == x);
                bool isSel = (selArea == 1 && selX == x && selY == y);
                bool isStackSel = (selArea == 1 && selX == x && y > selY); // Highlight cards under selection
                
                std::cout << formatCard(tableau[x][y], isCursor, isSel, isStackSel);
            } else if (y == 0 && tableau[x].empty()) {
                // Draw empty slot indicator for empty columns
                bool isCursor = (pCursorY == 0 && pCursorX == x);
                std::cout << (isCursor ? Color::GREEN + "[      ]" + Color::RESET : "[      ]");
            } else {
                std::cout << "        "; // 8 Spaces (Width of a card)
            }
            std::cout << "  "; // Gap between columns
        }
        std::cout << "\n";
    }

    std::cout << "\n[WASD] Navigate | [SPACE] Select/Move | [C] Cancel Sel | [R] Draw Deck | [Q] Quit\n\n";
    Display::endFrame();
}

bool Solitaire::checkWin() {
    for (int i = 0; i < 4; i++) {
        if (foundations[i].size() != 13) return false;
    }
    return true;
}

void Solitaire::play() {
    bool skipSetup = false;

    while (true) {
        if (!skipSetup && !setupOptions()) return;

        resetBoard();
        bool gameRunning = true;
        bool playerWon = false;

        while (gameRunning) {
            drawTable();

            int input = getInput();

            if (input == 'Q') {
                std::cout << "Quit to menu? (Y/N): ";
                bool confirmQuit = false;
                while (true) {
                    int ans = getInput();
                    if (ans == 'Y') { confirmQuit = true; break; }
                    if (ans == 'N') { break; }
                }
                if (confirmQuit) return;
                continue; 
            }

            // Universal Hotkeys
            if (input == 'R') drawCardsFromStock();
            if (input == 'C') selArea = -1; // Cancel selection

            // Movement
            if (input == 'W') {
                if (pCursorY > 0) {
                    pCursorY--;
                    if (!tableau[pCursorX][pCursorY].faceUp) pCursorY = -1; // Jump to Top Bar if we hit hidden cards
                } else if (pCursorY == 0) {
                    pCursorY = -1;
                    if (pCursorX <= 2) pCursorX = 1; // Map left side to Waste
                }
            }
            if (input == 'S') {
                if (pCursorY == -1) {
                    pCursorY = 0;
                    snapCursorToRevealed(); // Drop into tableau
                } else if (pCursorX >= 0 && pCursorY < tableau[pCursorX].size() - 1) {
                    pCursorY++;
                }
            }
            if (input == 'A') {
                if (pCursorY == -1) {
                    if (pCursorX == 3) pCursorX = 1;
                    else if (pCursorX > 3) pCursorX--;
                } else {
                    if (pCursorX > 0) pCursorX--;
                    snapCursorToRevealed();
                }
            }
            if (input == 'D') {
                if (pCursorY == -1) {
                    if (pCursorX == 1) pCursorX = 3;
                    else if (pCursorX >= 3 && pCursorX < 6) pCursorX++;
                } else {
                    if (pCursorX < 6) pCursorX++;
                    snapCursorToRevealed();
                }
            }

            // Action Logic (SPACE)
            if (input == ' ' || input == '\r' || input == '\n') {
                
                // PICK UP PHASE
                if (selArea == -1) { 
                    if (pCursorY == -1) {
                        if (pCursorX == 1 && !waste.empty()) { // Grab Waste
                            selArea = 0; selX = 0; selY = 0;
                        } else if (pCursorX >= 3 && !foundations[pCursorX-3].empty()) { // Grab Foundation
                            selArea = 2; selX = pCursorX - 3; selY = 0;
                        }
                    } else if (pCursorY >= 0 && !tableau[pCursorX].empty() && tableau[pCursorX][pCursorY].faceUp) {
                        // Grab Tableau Stack
                        selArea = 1; selX = pCursorX; selY = pCursorY;
                    }
                } 
                // DROP PHASE
                else { 
                    bool validMove = false;
                    SolCard srcCard;

                    // 1. Identify what we are holding
                    if (selArea == 0) srcCard = waste.back();
                    else if (selArea == 2) srcCard = foundations[selX].back();
                    else if (selArea == 1) srcCard = tableau[selX][selY];

                    // 2. Validate dropping on Tableau
                    if (pCursorY >= 0) {
                        if (isValidTableauMove(srcCard, pCursorX)) {
                            // Execute Move
                            if (selArea == 0) {
                                tableau[pCursorX].push_back(srcCard);
                                waste.pop_back();
                            } else if (selArea == 2) {
                                tableau[pCursorX].push_back(srcCard);
                                foundations[selX].pop_back();
                            } else if (selArea == 1) {
                                // Move entire sub-stack
                                tableau[pCursorX].insert(tableau[pCursorX].end(), tableau[selX].begin() + selY, tableau[selX].end());
                                tableau[selX].erase(tableau[selX].begin() + selY, tableau[selX].end());
                            }
                            validMove = true;
                        }
                    } 
                    // 3. Validate dropping on Foundation
                    else if (pCursorY == -1 && pCursorX >= 3) {
                        int fIndex = pCursorX - 3;
                        // Can only drop ONE card to foundation
                        if ((selArea == 0) || (selArea == 1 && selY == tableau[selX].size() - 1)) {
                            if (isValidFoundationMove(srcCard, fIndex)) {
                                foundations[fIndex].push_back(srcCard);
                                if (selArea == 0) waste.pop_back();
                                if (selArea == 1) tableau[selX].pop_back();
                                validMove = true;
                            }
                        }
                    }

                    if (validMove) autoReveal();
                    selArea = -1; // Clear selection whether successful or cancelled
                }

                if (checkWin()) {
                    playerWon = true;
                    gameRunning = false;
                }
            }
        }

        // --- END GAME MENU ---
        drawTable();
        
        if (playerWon) Display::printColored("VICTORY! You sorted the entire deck.\n\n", Color::GREEN);

        int endSel = 0;
        std::string endOpts[] = {"Play Again (Same Setup)", "Change Setup", "Quit to Main Menu"};
        bool madeChoice = false; 

        while (!madeChoice) {
            std::cout << "\n--- POST GAME ---\n";
            for (int i = 0; i < 3; i++) {
                if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
                else std::cout << "  " << endOpts[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE/ENTER] Select | [Q] Quit\n> ";

            int choice = getInput();
            if (choice == 'Q') return;
            if (choice == 'W' && endSel > 0) endSel--;
            if (choice == 'S' && endSel < 2) endSel++;
            if (choice == ' ' || choice == '\r' || choice == '\n') {
                if (endSel == 0) { skipSetup = true; madeChoice = true; }
                if (endSel == 1) { skipSetup = false; madeChoice = true; }
                if (endSel == 2) { return; }
            }
            if (!madeChoice) drawTable(); 
        }
    }
}