cmake_minimum_required(VERSION 3.16)

project(flipper VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use Vcpkg toolchain if available
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(flipper
    src/main.cpp
    src/main_window.cpp
    src/hex_grid.cpp
    src/game_board.cpp
    src/flipper.cpp
)

target_link_libraries(
    flipper PRIVATE 
    Qt6::Widgets  
)

set_target_properties(flipper PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)