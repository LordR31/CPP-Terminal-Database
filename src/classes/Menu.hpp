// Menu.h
#ifndef MENU_H 
#define MENU_H

#include <string>
#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "Database.hpp"
#include "../utils.hpp"
#include <filesystem>

#define PROGRAM_TITTLE_COLUMN_POSTION COLS - 28
#define MENU_TITTLE_LINE_POSITION 1
#define LINE_TEXT_START_POSITION 3
#define CENTERED_LINE_TEXT_START_POSITION COLS / 2
#define USER_INPUT_LINE LINES - 2
#define AVAILABLE_OPTIONS_START_LINE LINES - 4
#define CENTERED_AVAILABLE_OPTIONS_START_LINE LINES / 2
#define LAST_COLUMN_OF_TEXT COLS - 1

class Menu{
public:
    Menu();

    int main_menu();             // Main Menu
    int manage_database_menu();  // Manage Databases Menu
    int load_database_menu();    // Load Database Menu
    int create_database();       // Create Database Menu
    int delete_database_menu();  // Delete Database Menu
    int available_databases_menu();       // Print all available databases Menu
    int current_database_menu(); // Current Database Menu
    int settings_menu();         // Settings Menu
private:
    // PRINT MENUS
    void print_program_tittle();

    void print_main_menu();        // Print Main Menu
    
    void print_manage_database();  // Print Manage Databases Menu
    
    // Load Database Menu
    void print_load_database(bool is_empty, bool is_paged); // Print Load Database Menu
    void print_choose_database();
    void print_found_databases(std::vector<Database> found_databases);
    void print_find_menu();
    void print_search_word(const std::string& search_word);
    void print_load_database_options(bool is_paged);
    
    // Available Databases Menu
    void print_available_databases_menu(bool is_empty, bool is_paged);
    void print_databases_options(bool is_paged);

    // Delete Database Menu
    bool delete_database();

    void print_delete_database_menu(bool is_empty, bool is_paged);
    void print_delete_database_menu_options(bool is_paged);
    void print_delete_database_choose_menu();
    void print_delete_database_confirm_deletion_menu();
    void print_delete_database_confirm_deletion_prompt(bool is_confirmed);
    
    // Print Current Database Menu
    int add_object_menu();          // Add Object Menu
    bool delete_object_menu();      // Delete Object Menu
    
    void print_current_database_menu(bool is_empty, bool is_paged);
    void print_current_database_menu_options(bool is_empty, bool is_paged);
    void print_current_database_menu_delete_object_prompt(int number_of_elements_deleted);
    void print_add_object_menu();
    void print_add_object_result(bool result);
    void print_delete_object_menu();
    void print_delete_by_id();
    void print_delete_by_name();
    void print_delete_by_type();

    // Settings Menu
    void print_settings_menu();
    void print_decorator_settings();
    void print_text_position_settings();
    void print_sound_settings();
    void print_number_of_entries_settings();
    void print_continuous_mode_settings();

    // MENU FUNCTIONS
    void decrement_page();
    void increment_page();
    void clear_user_input_zone();
    void clear_search_results();
    bool choose_database();
    std::vector<Database> match_word(const std::string& input_word);
    void find_database();
    void update_index_manager();

    void print_invalid_input();
    

    void save_settings();          // Save settings (to file)
    void reload_database_vector(); // Reload database vector
    // VARIABLES, OBJECTS AND VECTORS

                                                    // holds the available databases
    std::vector<Database> database_vector;          // cppcheck-suppress unusedStructMember
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
                                          // 0 - Left side (Default)
                                          // 1 - Centered
    
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

#endif // MENU