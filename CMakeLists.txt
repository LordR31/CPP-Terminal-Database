cmake_minimum_required(VERSION 3.10) 

project(TerminalDatabaseApp CXX) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

include_directories(src/classes) 

# --- Curses Library Configuration ---
# This part of your CMake file remains unchanged
if(WIN32)
    find_package(unofficial-pdcurses CONFIG REQUIRED)
    set(CURSES_LIBRARY unofficial::pdcurses::pdcurses)
else()
    find_package(Curses REQUIRED)
    set(CURSES_LIBRARY ${CURSES_LIBRARIES})
endif()
# --- End of Curses Library Configuration --- 

# --- Main Application Build ---
# The target for your main application remains the same
add_executable(TerminalDatabaseApp
    src/main.cpp
    src/classes/Object.cpp
    src/classes/Database.cpp
    src/classes/Menu.cpp
    src/utils.cpp
) 

target_link_libraries(TerminalDatabaseApp PRIVATE ${CURSES_LIBRARY}) 

# --- Post-build commands for main application ---
# This part of your CMake file remains unchanged
if(WIN32)
    add_custom_command(TARGET TerminalDatabaseApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:TerminalDatabaseApp>
        "${CMAKE_SOURCE_DIR}"
        COMMENT "Copying build folder to project root."
    )
    add_custom_command(TARGET TerminalDatabaseApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "${CMAKE_BINARY_DIR}/$<CONFIG>"
        COMMENT "Removing original build folder."
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
# --- End of post-build commands --- 

# --- GTest Integration for Unit Tests ---

# 1. Download and configure Google Test using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# 2. Define the source files for your test suite
file(GLOB TEST_SOURCES "tests/*.cpp")

# 3. Create a new executable target for your tests
add_executable(database_manager_tests ${TEST_SOURCES}
    src/classes/Object.cpp
    src/classes/Database.cpp
    # src/classes/Menu.cpp
    # src/utils.cpp
)

# 4. Link the test executable to gtest and your project's code
# This is the crucial part that tells your test executable where to find gtest headers
target_include_directories(database_manager_tests
    PRIVATE
        ${googletest_SOURCE_DIR}/googletest/include
        ${googletest_SOURCE_DIR}/googlemock/include
)

target_link_libraries(database_manager_tests
    PRIVATE
        gtest_main
)

# 5. Add CTest commands to enable running tests with `ctest`
enable_testing()
add_test(NAME database_manager_tests COMMAND database_manager_tests)