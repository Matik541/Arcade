cmake_minimum_required(VERSION 3.10)
project(TerminalArcade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Znajdź wszystkie pliki .cpp w katalogu
file(GLOB SOURCES "*.cpp")

add_executable(TerminalArcade ${SOURCES})

if(MSVC)
    add_compile_options(/utf-8)
endif()