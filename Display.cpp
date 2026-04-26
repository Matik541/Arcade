#include "Display.h"

void Display::clearScreen() {
    // Cross-platform ANSI clear screen
    std::cout << "\033[2J\033[1;1H";
}

void Display::printColored(const std::string& text, const std::string& color) {
    std::cout << color << text << Color::RESET;
}

void Display::drawLine(int width, char c) {
    std::cout << std::string(width, c) << "\n";
}