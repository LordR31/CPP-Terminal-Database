cmake_minimum_required(VERSION 3.10)

project(TerminalDatabaseApp CXX)

include_directories(classes)

find_package(Curses REQUIRED)

add_executable(TerminalDatabaseApp
main.cpp
classes/Object.cpp
classes/Database.cpp
classes/Menu.cpp
utils.cpp
)

target_link_libraries(TerminalDatabaseApp PRIVATE ${CURSES_LIBRARIES})