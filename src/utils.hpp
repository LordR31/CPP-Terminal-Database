#ifndef UTILS_H
#define UTILS_H

#include <curses.h>
#include <csignal>
#include "stdbool.h"
#include <iostream>

void set_window_size();             // sets the initial window size       
bool check_resize();                // check if the user resized the window to redraw everything
void draw_main_box(char decorator); // draws the main outline box of the menu
void draw_line(char decorator);     // draws the line at the bottom used for user input

#endif // UTILS