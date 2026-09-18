cmake_minimum_required(VERSION 3.25...4.4)
project(skate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(generated/rexglue.cmake)
include(../framework/cmake/Recomp.cmake)

recomp_add_game(skate
    OUTPUT_NAME "Skate"
    SOURCES
        src/main.cpp
    SETTINGS "${CMAKE_CURRENT_SOURCE_DIR}/settings/skate.toml"
    ICON "${CMAKE_CURRENT_SOURCE_DIR}/metadata/skate.ico"
    RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/skate.rc"
    DEVELOPMENT_GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/assets"
)
