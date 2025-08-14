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
#include <filesystem>

class Menu{
public:
    Menu();

    int print_menu();              // Main Menu
    int manage_database();         // Manage Databases Menu
    int load_database();           // Load Database Menu
    int create_database();         // Create Database Menu
    int delete_database();         // Delete Database Menu
    int print_databases();         // Print all available databases Menu
    int print_current_database();  // Print the current database and work on it
    int add_object_menu();        // Add Object Menu
    int delete_object_menu();      // Delete Object Menu
    int settings();                // Settings Menu
private:
    void save_settings();          // Save settings (to file)
    void reload_database_vector(); // Reload database vector
    std::vector<Database> match_word(const std::string& input_word);
    void print_found_databases(std::vector<Database> found_databases);
    // VARIABLES, OBJECTS AND VECTORS

                                                    // holds the available databases
    std::vector<Database> database_vector;          // cppcheck-suppress unusedStructMember
    Database current_database;                      // currently active database that can be worked on
    int current_database_index = 0;                 // index used when creating a new database
    int current_database_page_number = 0;           // currently active page which is displayed
    int all_database_page_number = 0;               // page number used in print_databases(), load_database() & delete_database()
    // SETINGS

    char decorator_type = '*';            // Decorator type used for drawing     
                                          // 1 - * (Default)
                                          // 2 - ~
                                          // 3 - +
                                          // 4 - /

    bool text_position = 1;               // Text aligmnent
                                          // 1 - Left side (Default)
                                          // 2 - Centered
    
    bool sound = 2;                       // *annoying* Sound on key press
                                          // 1 - ON 
                                          // 2 - OFF (Default)

    int number_of_entries = 5;            // Number of entries shown, can be:
                                          // 3
                                          // 5 (Default)
                                          // 7
                                          // TODO: Implement 9, 12, 15 (For bigger screens or for using fullscreen)
   
    //TODO: Implement compact mode
    //bool compact_mode = false;          // Compact mode: (Use whole terminal width or not)
                                          //               OFF (Default)
                                          //               ON  
    
    bool continuous_mode = false;         // Enable going from the last page to the first
                                          // and vice-versa by going next or previous

};

#endif // MENU