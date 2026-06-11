#ifndef CYBERDEFENSE_H
#define CYBERDEFENSE_H

#include "Game.h"
#include <vector>

/**
 * @struct Tower
 * @brief Struktura reprezentująca wieżę obronną w grze Cyber Defense.
 */
struct Tower {
    int x, y;   ///< Współrzędne wieży (X, Y).
    int type;   ///< Typ wieży: 1=Górnik (Generuje walutę), 2=Działko (Strzela), 3=Firewall (Dużo HP).
    int hp;     ///< Aktualne punkty życia wieży.
    int timer;  ///< Licznik czasu odnowienia (cooldown/generowanie).
};

/**
 * @struct Enemy
 * @brief Struktura reprezentująca wroga wędrującego po linii.
 */
struct Enemy {
    int x, y;      ///< Współrzędne wroga (X, Y).
    int hp;        ///< Bieżące punkty życia wroga.
    int maxHp;     ///< Maksymalne punkty życia wroga.
    char symbol;   ///< Symbol reprezentujący wroga w konsoli (np. 'V', 'W').
    int moveTimer; ///< Czas pozostały do wykonania kolejnego kroku.
    int speed;     ///< Liczba cykli gry wymagana do przesunięcia się o 1 pole.
};

/**
 * @struct Projectile
 * @brief Struktura reprezentująca pocisk wystrzelony przez wieżę.
 */
struct Projectile {
    int x, y;   ///< Współrzędne pocisku (X, Y).
    int damage; ///< Obrażenia zadawane przy uderzeniu we wroga.
};

/**
 * @class CyberDefense
 * @brief Klasa implementująca grę w obronę linii (Tower Defense) w terminalu.
 * @details Gracz broni 5 linii (ludzkich serwerów) przed nadchodzącymi wirusami, rozstawiając wieże w czasie rzeczywistym.
 */
class CyberDefense : public Game {
private:
    int width = 15;        ///< Szerokość planszy obrony.
    int height = 5;        ///< Wysokość planszy (liczba linii/ścieżek).
    int bits;              ///< Zgromadzone bity (waluta gry).
    int cursorX, cursorY;  ///< Koordynaty kursora budowania.
    int tickCounter;       ///< Licznik cykli gry.
    bool isGameOver;       ///< Flaga przegranej.
    bool playerWon;        ///< Flaga wygranej.
    int waveDuration;      ///< Czas trwania fali w cyklach gry.

    std::vector<Tower> towers;           ///< Lista aktywnych wież obronnych.
    std::vector<Enemy> enemies;          ///< Lista aktywnych wirusów (wrogów).
    std::vector<Projectile> projectiles;  ///< Lista pocisków w locie.

    /**
     * @brief Sprawdza, czy na danej pozycji stoi już wieża.
     * @return true Jeśli pozycja (x, y) jest zajęta przez wieżę.
     */
    bool hasTowerAt(int x, int y);

    /**
     * @brief Pobiera indeks wieży na danej pozycji.
     * @return Indeks wieży w wektorze towers lub -1 jeśli jej brak.
     */
    int getTowerIndex(int x, int y);

    /**
     * @brief Sprawdza, czy w danej linii (y) znajduje się wróg.
     */
    bool isEnemyInLane(int y, int minX);
    
    /**
     * @brief Tworzy (spawnuje) nowego wroga na końcu jednej z linii.
     */
    void spawnEnemy();

    /**
     * @brief Aktualizuje logikę gry o jeden krok czasowy (ruchy pocisków, wrogów, ataki wież).
     */
    void updateLogic();

    /**
     * @brief Rysuje interfejs planszy, linie obrony, wieże, wrogów i HUD.
     */
    void drawBoard();

    /**
     * @brief Resetuje stan gry, fali i waluty do domyślnych wartości.
     */
    void resetGame();

public:
    /**
     * @brief Konstruktor gry CyberDefense.
     */
    CyberDefense();

    /**
     * @brief Uruchamia rozgrywkę Cyber Defense.
     */
    void play() override;
};

#endif