#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>

namespace Color {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
}

class Display {
public:
    // Clears the terminal and resets the cursor to the top-left corner.
    static void clearScreen();
    // Prints colored text and restores the default terminal color.
    static void printColored(const std::string& text, const std::string& color);
    // Draws a repeated line character for separators and borders.
    static void drawLine(int width, char c = '-');
};

#endif