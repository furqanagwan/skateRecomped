cmake_minimum_required(VERSION 3.25)
project(skate_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(generated/rexglue.cmake)
include(../framework/cmake/Recomp.cmake)

recomp_add_game(skate_2
    OUTPUT_NAME "Skate 2"
    SOURCES
        src/main.cpp
    SETTINGS "${CMAKE_CURRENT_SOURCE_DIR}/settings/skate_2.toml"
    ICON "${CMAKE_CURRENT_SOURCE_DIR}/metadata/skate_2.ico"
    RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/skate_2.rc"
    DEVELOPMENT_GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/assets"
)
