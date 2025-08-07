#ifndef UTILS_H
#define UTILS_H

#include <curses.h>
#include <csignal>
#include "stdbool.h"

void set_window_size();
bool check_resize();
void draw_main_box();
void draw_line();

#endif // UTILS