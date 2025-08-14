// main.cpp
#include <iostream>
#include "classes/Menu.hpp"
#include "utils.hpp"

// for the start-up animaiton
#include "chrono"
#include "thread"

#define MAIN_MENU_ID 0
#define MANAGE_DATABASE_MENU_ID 1
#define CREATE_DATABASE_MENU_ID 2
#define LOAD_DATABASE_MENU_ID 3
#define DELETE_DATABASE_MENU_ID 4
#define PRINT_AVAILABLE_DATABASES_MENU_ID 5
#define PRINT_CURRENT_DATABASE_MENU_ID 6
#define SETTINGS_MENU_ID 7
#define EXIT_PROGRAM 20

#define END_BRACKET_POSITION_FOR_ANIMATION 24

#define FILES_LOADING_ANIMATION_LINE_POSITION 17
#define MENU_LOADING_ANIMATION_LINE_POSITION 14
#define DATABASE_LOADING_ANIMATION_LINE_POSITION 11

#define FILES_LOADING_ANIMATION_TEXT_DELAY 100
#define MENU_LOADING_ANIMATION_TEXT_DELAY 25
#define DATABASE_LOADING_ANIMATION_TEXT_DELAY 80

#define LOADING_FINISHED_TEXT_POSITION 8
#define WAIT_USER_INPUT_LINE_POSITION 3
using namespace std;

void loading_animation(int position, int delay_ms){
    move(LINES - position, LINE_TEXT_START_POSITION);
    printw("[");
    move(LINES - position, END_BRACKET_POSITION_FOR_ANIMATION);
    printw("]");
    for(int i = 0; i < 20; i++){
            move(LINES - position, 4 + i);
            printw("#");
            refresh();
            this_thread::sleep_for(chrono::milliseconds(delay_ms));
        }
}

int main() {
    initscr();            // init ncurses
    curs_set(0);          // 0 - hidden, 1 - visible and underlined, 2 - block
    cbreak();             // get char imidiately
    noecho();             // no echo
    keypad(stdscr, TRUE); // enable special keys

    // Start-up Animation
    set_window_size();
    clear();
    refresh();
    move(1, COLS - 28);
    printw("Terminal Database Manager");

    move(LINES - FILES_LOADING_ANIMATION_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Loading files...");
    move(LINES - FILES_LOADING_ANIMATION_LINE_POSITION + 1, LINE_TEXT_START_POSITION);
    // loading_animation(16, FILES_LOADING_ANIMATION_TEXT_DELAY);

    move(LINES - MENU_LOADING_ANIMATION_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Menu loading...");
    move(LINES - MENU_LOADING_ANIMATION_LINE_POSITION + 1, LINE_TEXT_START_POSITION);
    // loading_animation(13, MENU_LOADING_ANIMATION_TEXT_DELAY);

    move(LINES - DATABASE_LOADING_ANIMATION_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Database loading...");
    move(LINES - DATABASE_LOADING_ANIMATION_LINE_POSITION + 1, LINE_TEXT_START_POSITION);
    // loading_animation(10, DATABASE_LOADING_ANIMATION_TEXT_DELAY);

    move(LINES - LOADING_FINISHED_TEXT_POSITION, LINE_TEXT_START_POSITION);
    printw("Loading finished!");

    move(LINES - WAIT_USER_INPUT_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Press any key to continue...");
    getch();
    clear();

    Menu main_menu; // init main_menu

    int current_menu = 0;
    while(true){ // access the right menu, depending on each function's return
        switch (current_menu){
        case MAIN_MENU_ID:
            current_menu = main_menu.main_menu();    
            break;
        case MANAGE_DATABASE_MENU_ID:
            current_menu = main_menu.manage_database_menu();
            break;
        case CREATE_DATABASE_MENU_ID:
            current_menu = main_menu.create_database();
            break;
        case LOAD_DATABASE_MENU_ID:
            current_menu = main_menu.load_database_menu();
            break;
        case DELETE_DATABASE_MENU_ID:
            current_menu = main_menu.delete_database_menu();
            break;
        case PRINT_AVAILABLE_DATABASES_MENU_ID:
            current_menu = main_menu.available_databases_menu();
            break;
        case PRINT_CURRENT_DATABASE_MENU_ID:
            current_menu = main_menu.current_database_menu();
            break;
        case SETTINGS_MENU_ID:
            current_menu = main_menu.settings_menu();
            break;
        case EXIT_PROGRAM:
            endwin(); // terminate ncurses mode
            return 0;
        default:
            continue;
        }
    }

    endwin(); // terminate ncurses mode
    return 0;
}