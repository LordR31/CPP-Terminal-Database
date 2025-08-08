#ifndef UTILS_H
#define UTILS_H

#include <curses.h>
#include <csignal>
#include "stdbool.h"
#include <iostream>

void set_window_size();
bool check_resize();
void draw_main_box(char decorator);
void draw_line(char decorator);

#endif // UTILS