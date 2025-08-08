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
#include "../utils.h"

class Menu{
public:
    Menu();

    int print_menu();
    int manage_database();
    int load_database();
    int create_database();
    int delete_database();
    int print_databases();
    int print_current_database();
    int settings();
    void save_settings();
private:
    void reload_database_vector();
    
    // Holds the available databases
    std::vector<Database> database_vector; // cppcheck-suppress unusedStructMember
    Database current_database;
    int current_database_index = 0;
    int current_database_page_number = 0;
    char decorator_type = '*'; // 1 - * (Default)
                            // 2 - ~
                            // 3 - +
                            // 4 - /

    bool text_position = 1; // 1 - Left side (Default)
                            // 2 - Centered
    
    bool sound = 2;         // 1 - ON 
                            // 2 - OFF (Default)

    int number_of_entries = 5;    // Number of entries shown
   
    bool compact_mode = false;    // Compact mode: (Use whole terminal width or not)
                                  //               OFF (Default)
                                  //               ON  
    
    bool continuous_mode = false; // Enable going from the last page to the first
                                  // and vice-versa by going next or previous

};

#endif // MENU