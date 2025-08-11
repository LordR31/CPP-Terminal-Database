cmake_minimum_required(VERSION 3.10)

project(TerminalDatabaseApp CXX)

include_directories(classes)

if(WIN32)
    find_package(unofficial-pdcurses CONFIG REQUIRED)
    set(CURSES_LIBRARY unofficial::pdcurses::pdcurses)
else()
    find_package(Curses REQUIRED)
    set(CURSES_LIBRARY ${CURSES_LIBRARIES})
endif()

add_executable(TerminalDatabaseApp
    main.cpp
    classes/Object.cpp
    classes/Database.cpp
    classes/Menu.cpp
    utils.cpp
)

target_link_libraries(TerminalDatabaseApp PRIVATE ${CURSES_LIBRARY})

if(WIN32)
    add_custom_command(TARGET TerminalDatabaseApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:TerminalDatabaseApp>
        "${CMAKE_SOURCE_DIR}/.."
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
        "${CMAKE_SOURCE_DIR}/.."
        COMMENT "Copying executable to project root."
    )
    add_custom_command(TARGET TerminalDatabaseApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove
        $<TARGET_FILE:TerminalDatabaseApp>
        COMMENT "Removing original executable."
    )
endif()