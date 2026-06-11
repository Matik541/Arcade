#ifndef GAME_H
#define GAME_H

#include <string>
#include "Database.h"

/**
 * @class Game
 * @brief Klasa bazowa reprezentująca pojedynczą grę w Arcade.
 * @details Wszystkie gry w projekcie dziedziczą po tej klasie i implementują metodę play().
 */
class Game {
protected:
    std::string name;          ///< Nazwa gry.
    std::string description;   ///< Krótki opis zasad lub celów gry.
    bool hasScoreboard;        ///< Flaga określająca, czy gra wspiera zapisywanie wyników.
    bool higherIsBetter;       ///< Czy wyższy wynik oznacza lepszy rezultat (np. punkty vs czas w sekundach).
    Database* db;              ///< Wskaźnik do bazy danych wyników.

public:
    /**
     * @brief Konstruktor klasy Game.
     * @param n Nazwa gry.
     * @param desc Opis gry.
     * @param sc Czy gra posiada tablicę wyników.
     * @param hib Czy wyższy wynik jest lepszy (domyślnie true).
     */
    Game(std::string n, std::string desc, bool sc, bool hib = true) 
        : name(n), description(desc), hasScoreboard(sc), higherIsBetter(hib), db(nullptr) {}

    /**
     * @brief Wirtualny destruktor klasy Game.
     */
    virtual ~Game() = default;

    /**
     * @brief Uruchamia rozgrywkę danej gry. Musi być zaimplementowana w klasach pochodnych.
     */
    virtual void play() = 0; 
    
    /**
     * @brief Ustawia wskaźnik do bazy danych wyników.
     * @param database Wskaźnik do obiektu bazy danych.
     */
    void setDatabase(Database* database) { db = database; }

    /**
     * @brief Pobiera nazwę gry.
     * @return Nazwa gry jako std::string.
     */
    std::string getName() const { return name; }

    /**
     * @brief Pobiera opis gry.
     * @return Opis gry jako std::string.
     */
    std::string getDescription() const { return description; }

    /**
     * @brief Sprawdza, czy gra korzysta z tablicy wyników.
     * @return true Jeśli korzysta, false w przeciwnym wypadku.
     */
    bool usesScoreboard() const { return hasScoreboard; }

    /**
     * @brief Sprawdza, czy wyższy wynik jest lepszy.
     * @return true Jeśli tak, false jeśli niższy wynik (np. czas) jest lepszy.
     */
    bool isHigherScoreBetter() const { return higherIsBetter; }
};

#endif