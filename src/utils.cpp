// utils.cpp
#include "utils.h"

int old_cols = 0;
int old_lines = 0;

void set_window_size(){
    old_cols = COLS;
    old_lines = LINES;
}

bool check_resize(){
    if(old_cols != COLS || old_lines != LINES){
        old_cols = COLS;
        old_lines = LINES;
        return 1;
    }
    return 0;
}

void draw_main_box() {
    // Draw top and bottom borders
    for (int x = 0; x < COLS; ++x) {
        mvaddch(0, x, '*'); // Top row
        mvaddch(LINES - 1, x, '*'); // Bottom row
    }

    // Draw left and right borders
    for (int y = 0; y < LINES; ++y) {
        mvaddch(y, 0, '*'); // Left column
        mvaddch(y, COLS - 1, '*'); // Right column
    }
}

void draw_line(){
    for(int x = 0; x < COLS; x++)
        mvaddch(LINES - 3, x, '*');
}