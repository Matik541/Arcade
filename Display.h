#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>

/**
 * @namespace Color
 * @brief Definicje sekwencji ANSI Escape Codes do kolorowania tekstu w konsoli.
 */
namespace Color {
    const std::string RESET = "\033[0m";     ///< Resetuje formatowanie i kolory.
    const std::string RED = "\033[31m";       ///< Kolor czerwony.
    const std::string GREEN = "\033[32m";     ///< Kolor zielony.
    const std::string YELLOW = "\033[33m";    ///< Kolor żółty.
    const std::string BLUE = "\033[34m";      ///< Kolor niebieski.
    const std::string MAGENTA = "\033[35m";   ///< Kolor fioletowy.
    const std::string CYAN = "\033[36m";      ///< Kolor błękitny.
}

/**
 * @class Display
 * @brief Klasa zarządzająca wyświetlaniem i buforowaniem klatek w terminalu.
 * @details Umożliwia pisanie flicker-free za pomocą bufora ramki oraz kolorowanie znaków ANSI.
 */
class Display {
public:
    /**
     * @brief Czyści ekran (kończy poprzednią ramkę i zaczyna nową).
     */
    static void clearScreen();

    /**
     * @brief Drukuje tekst w wybranym kolorze ANSI.
     * @param text Tekst do wyświetlenia.
     * @param color Kolor z przestrzeni Color.
     */
    static void printColored(const std::string& text, const std::string& color);

    /**
     * @brief Rysuje linię separatora o określonej długości i znaku.
     * @param width Szerokość linii.
     * @param c Znak rysowania linii (domyślnie '-').
     */
    static void drawLine(int width, char c = '-');

    /**
     * @brief Pokazuje kursor w terminalu.
     */
    static void showCursor();

    /**
     * @brief Ukrywa kursor w terminalu.
     */
    static void hideCursor();

    /**
     * @brief Rozpoczyna przechwytywanie strumienia std::cout do bufora ramki.
     */
    static void startFrame();

    /**
     * @brief Kończy buforowanie ramki i wysyła przetworzoną, stabilną ramkę na ekran.
     */
    static void endFrame();
};

#endif