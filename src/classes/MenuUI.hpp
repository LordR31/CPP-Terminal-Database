// MenuUI.hpp
#ifndef MENUUI_H 
#define MENUUI_H

#define PROGRAM_TITTLE_COLUMN_POSTION COLS - 28
#define MENU_TITTLE_LINE_POSITION 1
#define LINE_TEXT_START_POSITION 3
#define CENTERED_LINE_TEXT_START_POSITION COLS / 2
#define USER_INPUT_LINE LINES - 2
#define AVAILABLE_OPTIONS_START_LINE LINES - 4
#define CENTERED_AVAILABLE_OPTIONS_START_LINE LINES / 2
#define LAST_COLUMN_OF_TEXT COLS - 1

#include "Database.hpp"
#include <vector>
#include "../utils.hpp"

#define LEFT_ALIGNED 1
#define CENTERED 0

class MenuUI{
public:
    MenuUI();

    // PRINT MENUS
    virtual void print_main_menu(char decorator_type, bool text_position);              // Print Main Menu
    virtual void print_manage_databases_menu(char decorator_type, bool text_position);  // Print Manage Databases Menu
    virtual void print_create_database_menu(char decorator_type, bool text_position);
    virtual void print_delete_database_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, std::vector<Database> database_vector); // Print Delete Database Menu
    virtual void print_current_database_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, Database current_database); // Print Current Database Menu
    virtual void print_settings_menu(char decorator_type, bool text_position);

    // Manage Database Sub-Menus
    virtual void print_load_database(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, std::vector<Database> database_vector); // Print Load Database Menu
    virtual void print_choose_database(bool text_position);
    virtual void print_found_databases(bool text_position, int number_of_entries, int page_number, std::vector<Database> found_databases);
    virtual void print_find_menu(bool text_position, int number_of_entries, bool is_database);
    virtual void print_search_word(const std::string& search_word);
    virtual void print_load_database_options(bool is_empty, bool is_paged, bool show_clear_button);
    virtual void print_edit_database_menu(bool text_position);
    virtual void print_edit_database_name();

    // Create Database Sub-Menus
    virtual void print_database_already_exists(bool text_position);
    virtual void print_database_created_successfully(bool text_position);

    // Delete Database Sub-Menus
    virtual void print_delete_database_menu_options(bool is_empty, bool is_paged, bool show_clear_button);
    virtual void print_delete_database_choose_menu(bool text_position);
    virtual void print_delete_database_confirm_deletion_menu();
    virtual void print_delete_database_confirm_deletion_prompt(bool is_confirmed);

    // Available Databases Sub-Menus
    virtual void print_available_databases_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, std::vector<Database> database_vector);
    virtual void print_available_databases_options(bool is_empty, bool is_paged);
    
    // Current Database Sub-Menus
    virtual void print_current_database_menu_options(bool is_empty, bool is_paged, bool show_clear_button);
    virtual void print_current_database_menu_delete_object_prompt(int number_of_elements_deleted);
    virtual void print_add_object_menu(char decorator_type);
    virtual void print_add_object_result(bool result);
    virtual void print_delete_object_menu();
    virtual void print_delete_by_id();
    virtual void print_delete_by_name();
    virtual void print_delete_by_type();
    virtual void print_edit_object_menu();
    virtual void print_edit_object(char decorator_type, int object_id, Database current_database);
    virtual void print_found_objects(int number_of_entries, int page_number, const std::vector<Object>& found_objects);

    // Settings Sub-Menus
    virtual void print_decorator_settings(char decorator_type);
    virtual void print_text_position_settings(bool text_position);
    virtual void print_sound_settings(bool sound);
    virtual void print_number_of_entries_settings(int number_of_entries);
    virtual void print_continuous_mode_settings(bool continuous_mode);

    // Special Functions
    virtual void print_program_tittle();  // print program tittle in top right corner
    virtual void print_invalid_input(bool text_position);   // print Invalid Input in User Input Area
    virtual void clear_user_input_zone(); // clears User Input Area
    virtual void clear_search_results(int number_of_entries);  // clears search results when using find
private:

};

#endif // MENU UI