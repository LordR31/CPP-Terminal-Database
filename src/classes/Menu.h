// Menu.h
#ifndef MENU_H 
#define MENU_H

#include <string>
#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "Database.h"

class Menu{
public:
    Menu();

    void print_menu();
    void manage_database();
    void load_database();
    void create_database();
    void delete_database();
    void print_databases();
    void settings();
    void save_settings();

private:
    void reload_database_vector();
    std::vector<Database> database_vector; // Holds the available databases
    Database current_database;

    int current_database_index = 0;

    int decorator_type = 0; // 0 - * (Default)
                            // 1 - ~
                            // 2 - +
                            // 3 - /

    bool text_position = 0; // 0 - Left side (Default)
                            // 1 - Centered
    
    bool sound = 0;         // 0 - ON (Default)
                            // 1 - OFF 

    int number_of_entries = 5;    // Number of entries shown
   
    bool compact_mode = false;    // Compact mode: (Use whole terminal width or not)
                                  //               OFF (Default)
                                  //               ON  
    
    bool continuous_mode = false; // Enable going from the last page to the first
                                  // and vice-versa by going next or previous

};

#endif // MENU