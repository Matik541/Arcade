#include "Display.h"
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

namespace {
    std::stringstream frameBuffer;
    std::streambuf* oldCoutBuffer = nullptr;
    bool isBuffering = false;

    /**
     * @brief Dokleja sekwencję czyszczącą \033[K na końcu każdej linii bufora.
     * @details Zapobiega to pozostawaniu artefaktów z poprzednich klatek.
     */
    std::string processFrame(const std::string& input) {
        std::string result;
        result.reserve(input.size() * 1.1);
        
        size_t pos = 0;
        size_t prev = 0;
        while ((pos = input.find('\n', prev)) != std::string::npos) {
            result.append(input, prev, pos - prev);
            result.append("\033[K\n");
            prev = pos + 1;
        }
        if (prev < input.size()) {
            result.append(input, prev, input.size() - prev);
            result.append("\033[K");
        }
        return result;
    }
}

void Display::startFrame() {
    std::cout << "\033[?25l"; // Ukrycie kursora dla zapobiegania migotaniu
    
    if (!isBuffering) {
        frameBuffer.str("");
        frameBuffer.clear();
        oldCoutBuffer = std::cout.rdbuf(frameBuffer.rdbuf());
        isBuffering = true;
    }
}

void Display::endFrame() {
    if (isBuffering) {
        std::cout.rdbuf(oldCoutBuffer); // Przywrócenie standardowego bufora std::cout
        oldCoutBuffer = nullptr;
        isBuffering = false;
        
        std::string processed = processFrame(frameBuffer.str());
        
        // Przesunięcie kursora na początek (0,0) i wypisanie stabilnej ramki
        std::cout << "\033[H" << processed << "\033[J" << std::flush;
    }
}

void Display::clearScreen() {
    endFrame(); // Wypchnięcie ewentualnej wcześniejszej klatki
    startFrame(); // Rozpoczęcie nowej
}

void Display::showCursor() {
    endFrame(); // Wypchnięcie ramki przed pokazaniem kursora
    std::cout << "\033[?25h" << std::flush;
}

void Display::hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

void Display::printColored(const std::string& text, const std::string& color) {
    std::cout << color << text << Color::RESET;
}

void Display::drawLine(int width, char c) {
    std::cout << std::string(width, c) << "\n";
}