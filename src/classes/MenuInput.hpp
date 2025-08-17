// MenuInput.hpp
#ifndef MENUINPUT_H 
#define MENUINPUT_H

#include <string>
#include <stdio.h>
#include <curses.h>

class MenuInput{
public:
    virtual int get_char();
    virtual std::string get_string(int read_size);
};

#endif // MENU Input
