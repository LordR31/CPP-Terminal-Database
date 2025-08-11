// utils.cpp
#include "utils.h"

int old_cols = 0;  // stores window number of columns before window resize
int old_lines = 0; // stores window number of lines before window resize

void set_window_size(){
    old_cols = COLS;   // set initial number of columns
    old_lines = LINES; // set initial number of lines
}

bool check_resize(){
    if(old_cols != COLS || old_lines != LINES){ // check if the old number of columns and lines matches with the current one
        old_cols = COLS;                        // if it doesn't, get the new number of columns
        old_lines = LINES;                      // if it doesn't, get the new number of lines
        return 1;                               // return 1 (there needs to be a redraw, using the new lines and columns)
    }
    return 0;                                   // return 0 (there is no need for a redraw)
}

void draw_main_box(char decorator) {
    // draw top and bottom borders
    for (int x = 0; x <= COLS; x++) {
        mvaddch(0, x, decorator);         // top row
        mvaddch(LINES - 1, x, decorator); // bottom row
    }

    // draw left and right borders
    for (int y = 0; y <= LINES; y++) {
        mvaddch(y, 0, decorator);        // left column
        mvaddch(y, COLS - 1, decorator); // right column
    }
}

void draw_line(char decorator){
    // draw the bottom line used for user input
    for(int x = 0; x < COLS; x++)
        mvaddch(LINES - 3, x, decorator);
}