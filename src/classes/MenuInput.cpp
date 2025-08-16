//MenuInput.cpp

#include "MenuInput.hpp"

using namespace std;

char MenuInput::get_char(){
    char user_input = getch();
    return user_input;
}

string MenuInput::get_string(int read_size){
    echo();                                                                                                                            // enable echo 
    nocbreak();                                                                                                                        // enable buffering
    char database_name [256];
    getnstr(database_name, read_size);
    noecho();                                                                                                                          // no echo
    cbreak(); 

    return (string)database_name;
}
