# 🕹️ Terminal Arcade 🕹️

Witaj w **Terminal Arcade** — kompaktowym zbiorze dziewięciu klasycznych i zręcznościowych gier, napisanych w nowoczesnym, zorientowanym obiektowo języku C++ i grywalnych bezpośrednio w terminalu. 

## 👥 Autorzy projektu 👥
* **Mateusz Kowalski** (Sekcja: )
* **Paweł Kowolik** (Sekcja: )
* **Damian Feter** (Sekcja: )

## 🌟 Główne Funkcje 🌟
* **Interaktywne Menu**: Nawiguj po grach i menu za pomocą klawiszy `W/A/S/D` lub strzałek — bez konieczności wpisywania cyfr.
* **Brak migotania ekranu (Flicker-Free)**: Renderowanie w konsoli zoptymalizowano poprzez nadpisywanie bufora zamiast jego czyszczenia, co zapewnia płynną rozgrywkę w czasie rzeczywistym w około 20 FPS.
* **Globalna Tablica Wyników**: Najlepsze wyniki są zapisywane w pliku `scores.txt`, a Top 3 dla każdej gry wyświetla się w menu głównym (system zaimplementowano z wykorzystaniem wzorca Singleton).
* **Cross-Platform**: Kod kompiluje się bezbłędnie na Windows (Visual Studio) oraz systemach uniksowych (CMake/Make). Obsługa kodowania UTF-8 i wejścia klawiatury automatycznie dostosowuje się do platformy.

## 🎮 Lista Gier 🎮
1. **Tic-Tac-Toe (Kółko i Krzyżyk)** — Zagraj PvP lub zmierz się z botem opartym na algorytmie Minimax.
2. **MineSweeper (Saper)** — Klasyczny saper ze wsparciem dla mechaniki szybkiego odkrywania ("chording").
3. **2048** — Przesuwaj i łącz kafelki; zawiera tryb nieskończony (Endless).
4. **Blackjack** — Obstawiaj przeciwko krupierowi wykorzystując prosty system wirtualnego portfela (bankroll).
5. **Battleship (Statki)** — Rozstawiaj i poluj na statki wroga; wspiera ręczne rozmieszczanie i grę z zaawansowanym AI.
6. **Snake** — Zręcznościowy wąż w czasie rzeczywistym z systemem punktacji.
7. **Solitaire (Pasjans Klondike)** — Pełne doświadczenie klasycznego pasjansa z uwzględnieniem zasad dla stref tableau i fundamentów.
8. **Bomberman (PvP)** — Lokalna arena dla dwóch graczy (WASD kontra IJKL).
9. **Cyber Defense** — Gra czasu rzeczywistego inspirowana gatunkiem tower-defense.

## 🚀 Szybki start (CMake) 🚀
Zbuduj i uruchom aplikację z głównego katalogu projektu:

```bash
cmake -B build
cmake --build build
./build/TerminalArcade
```

Lub uruchom wszystko jedną komendą:

```bash
cmake -B build && cmake --build build && ./build/TerminalArcade
```

## ⌨️ Sterowanie (Instrukcja Użytkownika) ⌨️

* **Ruch / Nawigacja**: `W`, `A`, `S`, `D` (lub strzałki) 

* **Wybór / Akcja**: `Spacja` lub `Enter` 

* **Wyjście / Powrót**: `Q` 

*(Szczegółowe sterowanie dla poszczególnych gier widoczne jest zawsze bezpośrednio na ekranie podczas rozgrywki).* 