cmake_minimum_required(VERSION 3.25...4.4)

# CMP0155 became NEW with the 4.4 policy bump, which turns on C++ module
# scanning for every target. clang-scan-deps runs outside the compiler's own
# environment and does not find the MSVC headers, so scanning fails on the
# first <filesystem> it meets. Nothing here uses modules, and the SDK opts out
# the same way; this has to sit before project() to reach thirdparty targets.
if(POLICY CMP0155)
    cmake_policy(SET CMP0155 OLD)
endif()

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
