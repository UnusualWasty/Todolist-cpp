cmake_minimum_required(VERSION 4.2)
project(Projekt_C__)

set(CMAKE_CXX_STANDARD 20)

add_executable(Projekt_C__ main.cpp
        losowanie.cpp)

set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++"
)
