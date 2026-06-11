#include "CyberDefense.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

CyberDefense::CyberDefense() : Game("Cyber Defense", "Deploy Nodes to defend the CPU from Malware!", false, false) {
    srand(static_cast<unsigned int>(time(0)));
}

void CyberDefense::resetGame() {
    bits = 50; // Starting currency
    cursorX = 0;
    cursorY = 2;
    tickCounter = 0;
    waveDuration = 2000; // 100 seconds at 50ms per tick
    isGameOver = false;
    playerWon = false;

    towers.clear();
    enemies.clear();
    projectiles.clear();
}

bool CyberDefense::hasTowerAt(int x, int y) {
    for (const auto& t : towers) {
        if (t.x == x && t.y == y) return true;
    }
    return false;
}

int CyberDefense::getTowerIndex(int x, int y) {
    for (size_t i = 0; i < towers.size(); i++) {
        if (towers[i].x == x && towers[i].y == y) return i;
    }
    return -1;
}

bool CyberDefense::isEnemyInLane(int y, int minX) {
    for (const auto& e : enemies) {
        if (e.y == y && e.x > minX) return true; // Only shoot if enemy is in front of turret
    }
    return false;
}

void CyberDefense::spawnEnemy() {
    int lane = rand() % height;
    int type = rand() % 10;
    
    if (type < 6) {
        // Basic Virus (Normal speed, Normal HP)
        enemies.push_back({width - 1, lane, 3, 3, 'V', 0, 30});
    } else if (type < 9) {
        // Worm (Fast, Low HP)
        enemies.push_back({width - 1, lane, 2, 2, 'W', 0, 20});
    } else {
        // Trojan (Slow, High HP)
        enemies.push_back({width - 1, lane, 8, 8, 'T', 0, 80});
    }
}

void CyberDefense::updateLogic() {
    tickCounter++;

    // 1. Spawner Logic (Gets harder as time goes on)
    int spawnRate = 150;
    if (tickCounter > 500) spawnRate = 100;
    if (tickCounter > 1000) spawnRate = 50;
    if (tickCounter > 1500) spawnRate = 25;
    
    if (tickCounter < waveDuration && tickCounter % spawnRate == 0) {
        spawnEnemy();
    }

    // Check Win Condition
    if (tickCounter >= waveDuration && enemies.empty()) {
        isGameOver = true;
        playerWon = true;
    }

    // 2. Tower Logic
    for (int i = towers.size() - 1; i >= 0; i--) {
        towers[i].timer++;
        
        if (towers[i].type == 1) { // Miner (Generates bits)
            if (towers[i].timer >= 60) { // Every 3 seconds
                bits += 25;
                towers[i].timer = 0;
            }
        } 
        else if (towers[i].type == 2) { // Turret (Shoots lasers)
            if (towers[i].timer >= 20) { // Every 1 second
                if (isEnemyInLane(towers[i].y, towers[i].x)) {
                    projectiles.push_back({towers[i].x + 1, towers[i].y, 1}); // 1 Damage
                    towers[i].timer = 0;
                }
            }
        }
        
        // Remove dead towers
        if (towers[i].hp <= 0) {
            towers.erase(towers.begin() + i);
        }
    }

    // 3. Projectile Logic
    for (int i = projectiles.size() - 1; i >= 0; i--) {
        // Move lasers fast
        projectiles[i].x++; 
        
        bool hit = false;
        // Check collision with enemies
        for (int j = enemies.size() - 1; j >= 0; j--) {
            if (enemies[j].x == projectiles[i].x && enemies[j].y == projectiles[i].y) {
                enemies[j].hp -= projectiles[i].damage;
                hit = true;
                break; // Laser destroyed
            }
        }

        if (hit || projectiles[i].x >= width) {
            projectiles.erase(projectiles.begin() + i);
        }
    }

    // 4. Enemy Logic
    for (int i = enemies.size() - 1; i >= 0; i--) {
        if (enemies[i].hp <= 0) {
            bits += 10; // Reward for killing
            enemies.erase(enemies.begin() + i);
            continue;
        }

        enemies[i].moveTimer++;
        if (enemies[i].moveTimer >= enemies[i].speed) {
            // Check if blocked by a tower
            int towerIdx = getTowerIndex(enemies[i].x - 1, enemies[i].y);
            if (towerIdx != -1) {
                towers[towerIdx].hp -= 1; // Attack the tower
                enemies[i].moveTimer = 0; // Reset timer, don't move
            } else {
                // Move forward
                enemies[i].x--;
                enemies[i].moveTimer = 0;
                
                // Game Over if they reach the left edge (CPU breached)
                if (enemies[i].x < 0) {
                    isGameOver = true;
                    playerWon = false;
                }
            }
        }
    }
}

void CyberDefense::drawBoard() {
    Display::clearScreen();
    Display::printColored("=== CYBER DEFENSE ===\n\n", Color::YELLOW);
    
    std::cout << "Data Bits: ";
    Display::printColored(std::to_string(bits) + "\n", Color::CYAN);
    
    int timeRemaining = (waveDuration - tickCounter) / 20; // 20 ticks = 1 sec
    if (timeRemaining < 0) timeRemaining = 0;
    std::cout << "System Scan: " << timeRemaining << "s until threat neutralized.\n\n";

    std::cout << "Deployment Deck:\n";
    std::cout << "[1] Data Miner (50 Bits)  - Generates Bits over time.\n";
    std::cout << "[2] Blaster   (100 Bits)  - Shoots lasers down the lane.\n";
    std::cout << "[3] Firewall   (50 Bits)  - High defense, stalls enemies.\n";
    std::cout << "[X] Delete Node | [Q] Quit Match\n\n";

    // Draw Grid
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            bool drawCursor = (x == cursorX && y == cursorY);
            
            if (drawCursor) Display::printColored("[", Color::YELLOW);
            else std::cout << " ";

            // Check what is in this cell
            bool cellFilled = false;
            
            // 1. Enemies
            for (const auto& e : enemies) {
                if (e.x == x && e.y == y) {
                    Display::printColored(std::string(1, e.symbol), Color::RED);
                    cellFilled = true; break;
                }
            }
            
            // 2. Towers
            if (!cellFilled) {
                for (const auto& t : towers) {
                    if (t.x == x && t.y == y) {
                        if (t.type == 1) Display::printColored("$", Color::GREEN);
                        else if (t.type == 2) Display::printColored("►", Color::CYAN);
                        else if (t.type == 3) Display::printColored("█", Color::MAGENTA);
                        cellFilled = true; break;
                    }
                }
            }

            // 3. Lasers
            if (!cellFilled) {
                for (const auto& p : projectiles) {
                    if (p.x == x && p.y == y) {
                        Display::printColored("-", Color::YELLOW);
                        cellFilled = true; break;
                    }
                }
            }

            // 4. Empty Space
            if (!cellFilled) {
                std::cout << ".";
            }

            if (drawCursor) Display::printColored("]", Color::YELLOW);
            else std::cout << " ";
        }
        std::cout << "\n";
    }
    Display::endFrame();
}

void CyberDefense::play() {
    resetGame();
    
    while (!isGameOver) {
        
        // 1. INPUT PROCESSING
        while (hasInput()) {
            int in = getInput();
            
            if (in == 'Q') {
                std::cout << "\nQuit match? (Y/N): ";
                while (true) {
                    int ans = getInput();
                    if (ans == 'Y') return;
                    if (ans == 'N') break;
                }
            }

            if (in == 'W' && cursorY > 0) cursorY--;
            if (in == 'S' && cursorY < height - 1) cursorY++;
            if (in == 'A' && cursorX > 0) cursorX--;
            // Restrict cursor from the enemy spawn zone (rightmost column)
            if (in == 'D' && cursorX < width - 2) cursorX++; 

            // Building Logic
            if (in == '1' && bits >= 50 && !hasTowerAt(cursorX, cursorY)) {
                bits -= 50;
                towers.push_back({cursorX, cursorY, 1, 5, 0});
            }
            if (in == '2' && bits >= 100 && !hasTowerAt(cursorX, cursorY)) {
                bits -= 100;
                towers.push_back({cursorX, cursorY, 2, 5, 0});
            }
            if (in == '3' && bits >= 50 && !hasTowerAt(cursorX, cursorY)) {
                bits -= 50;
                towers.push_back({cursorX, cursorY, 3, 20, 0}); // High HP
            }
            
            // Selling / Deleting
            if (in == 'X') {
                int idx = getTowerIndex(cursorX, cursorY);
                if (idx != -1) {
                    // Give back half the cost
                    if (towers[idx].type == 1) bits += 25;
                    if (towers[idx].type == 2) bits += 50;
                    if (towers[idx].type == 3) bits += 25;
                    towers.erase(towers.begin() + idx);
                }
            }
        }

        // 2. GAME LOGIC
        updateLogic();

        // 3. RENDER & DELAY
        drawBoard();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // --- POST GAME ---
    drawBoard();
    
    std::cout << "\n";
    if (playerWon) Display::printColored("SYSTEM SECURE! You successfully repelled the malware.\n\n", Color::GREEN);
    else Display::printColored("FATAL ERROR! CPU BREACHED by Malware.\n\n", Color::RED);

    int endSel = 0;
    std::string endOpts[] = {"Play Again", "Quit to Main Menu"};
    bool madeChoice = false; 

    while (!madeChoice) {
        std::cout << "\n--- POST GAME ---\n";
        for (int i = 0; i < 2; i++) {
            if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
            else std::cout << "  " << endOpts[i] << "\n";
        }
        std::cout << "\n[W/S] Navigate | [SPACE/ENTER] Select | [Q] Quit\n> ";

        int choice = getInput();
        if (choice == 'Q') return;
        if (choice == 'W' && endSel > 0) endSel--;
        if (choice == 'S' && endSel < 1) endSel++;
        if (choice == ' ' || choice == '\r' || choice == '\n') {
            if (endSel == 0) { play(); return; } // Recurse to restart
            if (endSel == 1) { return; }
        }
        if (!madeChoice) drawBoard(); 
    }
}