cmake_minimum_required(VERSION 3.10)
project(TerminalArcade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ustawienie katalogu wyjściowego bezpośrednio w build, bez podkatalogów Debug/Release
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTCFG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTCFG} ${CMAKE_BINARY_DIR})
endforeach()

# Znajdź wszystkie pliki .cpp w katalogu
file(GLOB SOURCES "*.cpp")

add_executable(TerminalArcade ${SOURCES})

if(MSVC)
    add_compile_options(/utf-8 /W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()