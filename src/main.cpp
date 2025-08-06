// main.cpp
#include <iostream>
#include "classes/Object.h"   
#include "classes/Database.h"
#include "classes/Menu.h"

using namespace std;

int main() {
    initscr();            // init ncurses
    cbreak();             // get char imidiately
    noecho();             // no echo
    keypad(stdscr, TRUE); // enable special keys

    Menu main_menu;
    while(true)
        main_menu.print_menu();

    endwin();
    return 0;
}