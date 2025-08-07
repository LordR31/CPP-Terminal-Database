// main.cpp
#include <iostream>
// #include "classes/Object.h"   
// #include "classes/Database.h"
#include "classes/Menu.h"
#include "utils.h"
#include "chrono"
#include "thread"

using namespace std;

int main() {
    initscr();            // init ncurses
    curs_set(0);          // 0 - hidden, 1 - visible and underlined, 2 - block
    cbreak();             // get char imidiately
    noecho();             // no echo
    keypad(stdscr, TRUE); // enable special keys

    set_window_size();
    clear();
    refresh();
    move(1, COLS - 28);
    printw("Database Terminal Manager");
    move(LINES - 17,3);
    printw("Loading files...");
    move(LINES - 16,3);
    printw("[");
    move(LINES - 16,24);
    printw("]");
    for(int i = 0; i < 20; i++){
        move(LINES - 16, 4 + i);
        printw("#");
        refresh();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    move(LINES - 14,3);
    printw("Menu loading...");
    move(LINES - 13,3);
    printw("[");
    move(LINES - 13,24);
    printw("]");
    for(int i = 0; i < 20; i++){
        move(LINES - 13, 4 + i);
        printw("#");
        refresh();
        this_thread::sleep_for(chrono::milliseconds(25));
    }

    move(LINES - 11, 3);
    printw("Database loading...");
    move(LINES - 10,3);
    printw("[");
    move(LINES - 10,24);
    printw("]");
    for(int i = 0; i < 20; i++){
        move(LINES - 10, 4 + i);
        printw("#");
        refresh();
        this_thread::sleep_for(chrono::milliseconds(80));
    }

    move(LINES - 8,3);
    printw("Loading finished!");

    move(LINES - 3,3);
    printw("Press any key to continue...");
    getch();
    clear();

    Menu main_menu;

    int current_menu = 0;
        while(true){
        switch (current_menu){
        case 0:
            current_menu = main_menu.print_menu();    
            break;
        case 1:
            current_menu = main_menu.manage_database();
            break;
        case 2:
            current_menu = main_menu.create_database();
            break;
        case 3:
            current_menu = main_menu.load_database();
            break;
        case 4:
            current_menu = main_menu.delete_database();
            break;
        case 5:
            current_menu = main_menu.print_databases();
            break;
        case 6:
            current_menu = main_menu.print_current_database();
            break;
        case 7:
            current_menu = main_menu.settings();
            break;
        default:
            break;
        }
    }

    endwin();
    return 0;
}