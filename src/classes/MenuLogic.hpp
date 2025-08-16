// MenuLogic.hpp
#ifndef MENULOGIC_H 
#define MENULOGIC_H

#include <string>
#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "MenuUI.hpp"
#include "MenuInput.hpp"
#include "Database.hpp"
#include <filesystem>
#include "../utils.hpp"

#define MAIN_MENU_ID 0
#define MANAGE_DATABASES_MENU_ID 1
#define CREATE_DATABASE_MENU_ID 2
#define MANAGE_DATABASE_SUBMENU_ID 3
#define DELETE_DATABASE_MENU_ID 4
#define PRINT_AVAILABLE_DATABASES_MENU_ID 5
#define PRINT_CURRENT_DATABASE_MENU_ID 6
#define SETTINGS_MENU_ID 7
#define EXIT_PROGRAM 20

#define ASCII_1 49
#define ASCII_2 50
#define ASCII_3 51
#define ASCII_4 52
#define ASCII_5 53
#define ASCII_6 54
#define ASCII_7 55
#define ASCII_ENTER 10

#define ASCII_y 121
#define ASCII_Y 89
#define ASCII_n 110
#define ASCII_N 78

class MenuLogic{
public:
    MenuLogic();

    // MAIN FUNCTIONS
    int main_menu();                // Main Menu
    int manage_databases_menu();    // Manage Databases Menu
    int create_database_menu();     // Create Database Menu
    int load_database_menu();       // Load Database Menu
    int delete_database_menu();     // Delete Database Menu
    int available_databases_menu(); // Print all available databases Menu
    int current_database_menu();    // Current Database Menu
    int settings_menu();            // Settings Menu

private:
    // SUB-MENU FUNCTIONS
    
    // Manage Databases Functions
    bool edit_database();
    void find_database();
    bool choose_database();

    // Delete Database Functions
    bool delete_database();
    
    // Current Database Functions
    int add_object_menu();          // Add Object Menu
    bool delete_object_menu();      // Delete Object Menu
    void edit_object_menu();
    void find_object_menu();

    // SPECIAL FUNCTIONS
    void decrement_page(bool is_database);
    void increment_page(bool is_database);
 
    std::vector<Database> match_word(const std::string& input_word);
    void update_index_manager();
    void get_new_database_index();
    void save_settings();          // Save settings (to file)
    void reload_database_vector(); // Reload database vector
    
    // VARIABLES, OBJECTS AND VECTORS

    MenuUI menu_ui;       
    MenuInput menu_input;                                      
    std::vector<Database> database_vector;          // holds the available databases
    Database current_database;                      // currently active database that can be worked on
    int current_database_index = 0;                 // index used when creating a new database
    int page_number = 0;                            // currently active page which is displayed
             
    // SETINGS

    char decorator_type = '*';            // Decorator type used for drawing     
                                          // 1 - * (Default)
                                          // 2 - ~
                                          // 3 - +
                                          // 4 - /

    bool text_position = 0;               // Text aligmnent
                                          // 1 - Left side (Default)
                                          // 0 - Centered
    
    bool sound = 0;                       // *annoying* Sound on key press
                                          // 1 - ON 
                                          // 0 - OFF (Default)

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

#endif // MENU LOGIC