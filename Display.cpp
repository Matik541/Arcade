#include "Display.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

void Display::clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (consoleHandle == INVALID_HANDLE_VALUE) {
        std::cout << "\033[2J\033[1;1H";
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo)) {
        std::cout << "\033[2J\033[1;1H";
        return;
    }

    DWORD cellCount = static_cast<DWORD>(bufferInfo.dwSize.X) * static_cast<DWORD>(bufferInfo.dwSize.Y);
    DWORD written = 0;
    COORD homePosition = {0, 0};

    FillConsoleOutputCharacterA(consoleHandle, ' ', cellCount, homePosition, &written);
    FillConsoleOutputAttribute(consoleHandle, bufferInfo.wAttributes, cellCount, homePosition, &written);
    SetConsoleCursorPosition(consoleHandle, homePosition);
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void Display::printColored(const std::string& text, const std::string& color) {
    std::cout << color << text << Color::RESET;
}

void Display::drawLine(int width, char c) {
    std::cout << std::string(width, c) << "\n";
}