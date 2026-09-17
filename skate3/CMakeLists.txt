cmake_minimum_required(VERSION 3.25)
project(skate_3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(generated/rexglue.cmake)
include(../framework/cmake/Recomp.cmake)

recomp_add_game(skate_3
    OUTPUT_NAME "Skate 3"
    SOURCES
        src/main.cpp
    SETTINGS "${CMAKE_CURRENT_SOURCE_DIR}/settings/skate_3.toml"
    ICON "${CMAKE_CURRENT_SOURCE_DIR}/metadata/skate_3.ico"
    RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/skate_3.rc"
    DEVELOPMENT_GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/assets"
)
