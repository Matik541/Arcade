#include "Blackjack.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

Blackjack::Blackjack() : Game("Blackjack", "Beat the dealer to 21. Highest bankroll wins!", true, true) {}

void Blackjack::buildAndShuffleDeck() {
    deck.clear();
    // Using real UTF-8 symbols instead of words
    std::string suits[] = {"♥", "♦", "♣", "♠"};
    std::string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    for (int d = 0; d < 4; ++d) {
        for (int s = 0; s < 4; ++s) {
            for (int r = 0; r < 13; ++r) {
                deck.push_back({ranks[r], suits[s], values[r]});
            }
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
}

Card Blackjack::drawCard() {
    if (deck.empty()) buildAndShuffleDeck(); // Reshuffle if we run out
    Card c = deck.back();
    deck.pop_back();
    return c;
}

int Blackjack::getHandValue(const std::vector<Card>& hand) {
    int value = 0;
    int aces = 0;

    for (const auto& card : hand) {
        value += card.value;
        if (card.rank == "A") aces++;
    }

    // Adjust for Aces if we bust
    while (value > 21 && aces > 0) {
        value -= 10;
        aces--;
    }
    return value;
}

bool Blackjack::setupOptions() {
    int selected = 0; 
    std::string options[] = {
        "Standard ($100 Bankroll) <- SCORED",
        "High Roller ($1000 Bankroll)",
        "Whale ($10000 Bankroll)"
    };

    while (true) {
        Display::clearScreen();
        Display::printColored("--- BLACKJACK SETUP ---\n\n", Color::YELLOW);
        
        for (int i = 0; i < 3; i++) {
            if (i == selected) Display::printColored("> " + options[i] + "\n", Color::GREEN);
            else std::cout << "  " << options[i] << "\n";
        }
        
        std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

        int input = getInput(); 
        if (input == 'Q') return false;
        if (input == 'W' && selected > 0) selected--;
        if (input == 'S' && selected < 2) selected++;
        
        if (input == ' ' || input == '\r' || input == '\n') {
            if (selected == 0) { startingBankroll = 100; isScored = true; break; }
            if (selected == 1) { startingBankroll = 1000; isScored = false; break; }
            if (selected == 2) { startingBankroll = 10000; isScored = false; break; }
        }
    }
    bankroll = startingBankroll;
    return true;
}

int Blackjack::bettingPhase() {
    int bet = 10;
    if (lastBet > 0 && lastBet <= bankroll) {
        bet = lastBet; // Start with last bet if valid
    }
    if (bet > bankroll) bet = bankroll;

    while (true) {
        Display::clearScreen();
        
        // Render Header & Legend Side-by-Side
        std::string title = "=== CASINO BLACKJACK ===";
        Display::printColored(title, Color::YELLOW);
        std::cout << std::string(45 - title.length(), ' ') << "| LEGEND\n";
        
        std::string info = "Bankroll: $" + std::to_string(bankroll);
        std::cout << info << std::string(45 - info.length(), ' ') << "| 2-10  : Face Value\n";
        
        std::string sep(45, '-');
        std::cout << sep << "| J,Q,K : 10\n";
        std::cout << std::string(45, ' ') << "| A     : 1 or 11\n\n";
        
        std::cout << "Current Bet: ";
        Display::printColored("$" + std::to_string(bet) + "\n\n", Color::GREEN);
        
        std::cout << "[W/S] +/- $10 | [A/D] +/- $100 | [X] All-in | [SPACE] Deal | [Q] Cash Out\n> ";

        int input = getInput();
        
        if (input == 'Q') {
            std::cout << "\nCash out and return to menu? (Y/N): ";
            while (true) {
                int ans = getInput();
                if (ans == 'Y') return -1; 
                if (ans == 'N') break;
            }
        }
        
        if (input == 'W') bet += 10;
        if (input == 'S') bet -= 10;
        if (input == 'D') bet += 100;
        if (input == 'A') bet -= 100;

        if (input == 'X') bet = bankroll;

        if (bet < 10) bet = 10;
        if (bet > bankroll) bet = bankroll;

        if (input == ' ' || input == '\r' || input == '\n') {
            lastBet = bet;
            return bet;
        }
    }
}

void Blackjack::drawTable(bool hideDealerCard, const std::string& message) {
    Display::clearScreen();

    // Render Header & Legend Side-by-Side
    std::string title = "=== CASINO BLACKJACK ===";
    Display::printColored(title, Color::YELLOW);
    std::cout << std::string(45 - title.length(), ' ') << "| LEGEND\n";

    std::string info = "Bankroll: $" + std::to_string(bankroll) + " | Bet: $" + std::to_string(currentBet);
    std::cout << info << std::string(45 - info.length(), ' ') << "| 2-10  : Face Value\n";

    std::string sep(45, '-');
    std::cout << sep << "| J,Q,K : 10\n";

    std::string dHand = "DEALER HAND: " + (hideDealerCard ? "(?)" : "(" + std::to_string(getHandValue(dealerHand)) + ")");
    std::cout << dHand << std::string(45 - dHand.length(), ' ') << "| A     : 1 or 11\n";

    // Draw Dealer Hand
    for (size_t i = 0; i < dealerHand.size(); i++) {
        if (i == 1 && hideDealerCard) {
            Display::printColored("[ ??? ] ", Color::MAGENTA);
        } else {
            std::string color = (dealerHand[i].suit == "♥" || dealerHand[i].suit == "♦") ? Color::RED : Color::CYAN;
            // Pad single-digit ranks with an extra space so cards are identical widths
            std::string rankPad = (dealerHand[i].rank.length() == 1) ? " " : ""; 
            Display::printColored("[ " + rankPad + dealerHand[i].rank + " " + dealerHand[i].suit + " ] ", color);
        }
    }
    std::cout << "\n\n";

    // Draw Player Hand
    std::cout << "YOUR HAND: (" << getHandValue(playerHand) << ")\n";
    for (const auto& card : playerHand) {
        std::string color = (card.suit == "♥" || card.suit == "♦") ? Color::RED : Color::CYAN;
        std::string rankPad = (card.rank.length() == 1) ? " " : "";
        Display::printColored("[ " + rankPad + card.rank + " " + card.suit + " ] ", color);
    }
    std::cout << "\n\n";
    Display::drawLine(45, '-');
    
    if (!message.empty()) {
        std::cout << message << "\n\n";
    }
}

void Blackjack::play() {
    bool skipSetup = false;

    while (true) { // Main Game Loop
        if (!skipSetup && !setupOptions()) return;
        
        buildAndShuffleDeck();

        while (bankroll > 0) { // Rounds Loop
            currentBet = bettingPhase();
            if (currentBet == -1) break; // Player chose to Cash Out

            playerHand.clear();
            dealerHand.clear();

            // Initial Deal
            playerHand.push_back(drawCard());
            dealerHand.push_back(drawCard());
            playerHand.push_back(drawCard());
            dealerHand.push_back(drawCard());

            bool playerBusted = false;
            bool roundOver = false;

            // Player Turn
            while (!roundOver) {
                drawTable(true, "[H] Hit | [S] Stand | [Q] Cash Out");

                int input = getInput();

                if (input == 'Q') {
                    std::cout << "Cash out mid-hand and forfeit your bet? (Y/N): ";
                    bool confirmQuit = false;
                    while (true) {
                        int ans = getInput();
                        if (ans == 'Y') { confirmQuit = true; break; }
                        if (ans == 'N') { break; }
                    }
                    if (confirmQuit) {
                        bankroll -= currentBet; // Forfeit bet
                        roundOver = true; 
                        break; 
                    }
                    continue; 
                }

                if (input == 'H') {
                    playerHand.push_back(drawCard());
                    if (getHandValue(playerHand) > 21) {
                        playerBusted = true;
                        roundOver = true;
                    }
                } 
                else if (input == 'S') {
                    roundOver = true;
                }
            }

            // Dealer Turn (If player didn't quit and didn't bust)
            if (!playerBusted && currentBet != -1) {
                while (getHandValue(dealerHand) < 17) {
                    dealerHand.push_back(drawCard());
                }
            }

            // Resolution
            if (currentBet != -1) {
                int pScore = getHandValue(playerHand);
                int dScore = getHandValue(dealerHand);
                std::string msg;

                if (playerBusted) {
                    msg = Color::RED + "BUST! You lose $" + std::to_string(currentBet) + Color::RESET;
                    bankroll -= currentBet;
                } 
                else if (dScore > 21) {
                    msg = Color::GREEN + "DEALER BUSTS! You win $" + std::to_string(currentBet) + Color::RESET;
                    bankroll += currentBet;
                } 
                else if (pScore > dScore) {
                    msg = Color::GREEN + "YOU WIN $" + std::to_string(currentBet) + "!" + Color::RESET;
                    bankroll += currentBet;
                } 
                else if (pScore < dScore) {
                    msg = Color::RED + "DEALER WINS. You lose $" + std::to_string(currentBet) + Color::RESET;
                    bankroll -= currentBet;
                } 
                else {
                    msg = Color::CYAN + "PUSH. It's a tie. Bet returned." + Color::RESET;
                }

                drawTable(false, msg + "\nPress ANY KEY for next hand...");
                getInput();
            }
        } // End of Rounds Loop

        // Post-Game Save & Menu
        Display::clearScreen();
        Display::printColored("=== CASH OUT ===\n\n", Color::YELLOW);
        std::cout << "Final Bankroll: $" << bankroll << "\n\n";

        if (bankroll <= 0) {
            Display::printColored("BANKRUPT! The casino took everything.\n\n", Color::RED);
        }

        // Save Score
        if (isScored && db != nullptr && bankroll > startingBankroll) {
            std::cout << "You made a profit! Enter your name for the Scoreboard: ";
            std::string playerName;
            std::cin >> playerName;
            db->saveScore(name, playerName, bankroll, isHigherBetter());
            Display::printColored("Score Saved!\n\n", Color::GREEN);
            while((getchar()) != '\n'); 
        }

        // NEW INTERACTIVE MENU
        int endSel = 0;
        std::string endOpts[] = {
            "Play Again (Same Setup)",
            "Change Setup",
            "Quit to Main Menu"
        };

        bool madeChoice = false; 
        
        while (!madeChoice) {
            Display::clearScreen();
            
            std::cout << "\n--- POST GAME ---\n";
            for (int i = 0; i < 3; i++) {
                if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
                else std::cout << "  " << endOpts[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

            int choice = getInput();
            if (choice == 'Q' ) return;
            if (choice == 'W' && endSel > 0) endSel--;
            if (choice == 'S' && endSel < 2) endSel++;
            if (choice == ' ' || choice == '\r' || choice == '\n') {
                if (endSel == 0) { skipSetup = true; madeChoice = true; bankroll = startingBankroll; lastBet = 0;}
                if (endSel == 1) { skipSetup = false; madeChoice = true; }
                if (endSel == 2) { return; }
            }
        }
    }
}