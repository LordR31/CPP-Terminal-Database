cmake_minimum_required(VERSION 3.10) 

project(TerminalDatabaseApp CXX) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")

include_directories(src/classes) 

# Main app

if(WIN32)
    find_package(unofficial-pdcurses CONFIG REQUIRED)
    set(CURSES_LIBRARY unofficial::pdcurses::pdcurses)
else()
    find_package(Curses REQUIRED)
    set(CURSES_LIBRARY ${CURSES_LIBRARIES})
endif()

add_executable(TerminalDatabaseApp
    src/main.cpp
    src/classes/Object.cpp
    src/classes/Database.cpp
    src/classes/MenuUI.cpp
    src/classes/MenuLogic.cpp
    src/utils.cpp
) 

target_link_libraries(TerminalDatabaseApp PRIVATE ${CURSES_LIBRARY}) 

if(WIN32)
    add_custom_command(
        TARGET TerminalDatabaseApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:TerminalDatabaseApp>
            ${CMAKE_CURRENT_SOURCE_DIR}

        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_PDB_FILE:TerminalDatabaseApp>
            ${CMAKE_CURRENT_SOURCE_DIR}

        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/Debug/pdcurses.dll
            ${CMAKE_CURRENT_SOURCE_DIR}

        COMMAND ${CMAKE_COMMAND} -E remove
            ${CMAKE_CURRENT_BINARY_DIR}/Debug/TerminalDatabaseApp.exe
            ${CMAKE_CURRENT_BINARY_DIR}/Debug/TerminalDatabaseApp.pdb
            ${CMAKE_CURRENT_BINARY_DIR}/Debug/pdcurses.dll

        COMMENT "Copying main app to project root and removing them from the build directory."
    )
else()
    add_custom_command(TARGET TerminalDatabaseApp POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:TerminalDatabaseApp>
          "${CMAKE_SOURCE_DIR}"
          COMMENT "Copying executable to project root."
    )
    add_custom_command(TARGET TerminalDatabaseApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove
        $<TARGET_FILE:TerminalDatabaseApp>
        COMMENT "Removing original executable."
    )
endif()

# 

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

set(gtest_force_shared_crt ON)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES "tests/*.cpp")

add_executable(database_manager_tests ${TEST_SOURCES}
    src/classes/Object.cpp
    src/classes/Database.cpp
)

target_include_directories(database_manager_tests
    PRIVATE
        ${googletest_SOURCE_DIR}/googletest/include
        ${googletest_SOURCE_DIR}/googlemock/include
)

target_link_libraries(database_manager_tests
    PRIVATE
        gtest_main
)

enable_testing()
add_test(NAME database_manager_tests COMMAND database_manager_tests)
