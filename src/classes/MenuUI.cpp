// MenuUI.cpp

#include "MenuUI.hpp"

using namespace std;

MenuUI::MenuUI(){}

// PRINT MENUS
void MenuUI::print_main_menu(char decorator_type, bool text_position){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Main Menu");
    print_program_tittle();
    
    string temp_string = "1. Manage Databases";
    // write out the menu options
    if(text_position == 1)               // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - 2, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE - 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "2. Settings";
    if(text_position == 1)               // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - 1, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE - 1, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "3. Quit Program";
    if(text_position == 1)               // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    draw_line(decorator_type);           // draw the bottom line
    refresh();
}

void MenuUI::print_manage_databases_menu(char decorator_type, bool text_position){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear(); 
    draw_main_box(decorator_type); 
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Manage Databases Menu");
    print_program_tittle();
    
    // write out the menu options
    string temp_string = "1. Create database";
    if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
       move(AVAILABLE_OPTIONS_START_LINE - 3, LINE_TEXT_START_POSITION);
    else
       move(AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "2. Manage databases";
    if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
       move(AVAILABLE_OPTIONS_START_LINE - 2, LINE_TEXT_START_POSITION);
    else
       move(AVAILABLE_OPTIONS_START_LINE - 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "3. Delete database";
    if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - 1, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE - 1, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "4. Print available databases";
    if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "Press Enter to go back to main menu";
    draw_line(decorator_type);                     // draw the bottom line
    if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
        move(LINES - 2, LINE_TEXT_START_POSITION);
    else
        move(LINES - 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
    refresh();
}

void MenuUI::print_create_database_menu(char decorator_type, bool text_position){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Create Database Menu");
    print_program_tittle();
    draw_line(decorator_type);                                    // draw the bottom line
    
    // write out the menu options
    string temp_string = "Leave blank and press Enter to cancel and go back!";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "Database name: ";
    if(text_position == LEFT_ALIGNED)                                  // check if text should be left side aligned or centered and write accordingly
            move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        else
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
}

void MenuUI::print_delete_database_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, vector<Database> database_vector){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(1, 3);
    printw("Delete Database Menu");
    print_program_tittle();
    draw_line(decorator_type);           // draw the bottom line
    refresh();

    string temp_string;
    if(is_empty){
        temp_string = "There are no available databases."; 
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 6, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str()); // and tell the user there's nothing to display
        
        temp_string = "Press any key to continue...";
        if(text_position == 1)
            move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        else
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
        return;
    }else{
        // write out the menu options, infos and warnings
        string temp_string = "WARNING!!!";
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());

        temp_string = "Deleting a database is PERMANENT!!!";
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());

        temp_string = "Press Enter to go back";
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 1, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
        
        temp_string = "Available databases";
        if(text_position == 1)                                                                          // check if text should be left side aligned or centered and write accordingly
            move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, 3);
        else
            move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());

        if(is_paged){
            temp_string = "Page " + to_string(page_number + 1);
            move(CENTERED_AVAILABLE_OPTIONS_START_LINE + 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
            printw("%s", temp_string.c_str());
        }

        for(int i = 0; i < number_of_entries; i++){                                                                                                                                          
            if(i + page_number * number_of_entries < static_cast<int>(database_vector.size())){                                                                        // starting with the LAST database in the vector
                if(text_position == 1)                                                                      // check if text should be left side aligned or centered and write accordingly
                    move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);
                else
                    move(AVAILABLE_OPTIONS_START_LINE - i, CENTERED_LINE_TEXT_START_POSITION - (database_vector[i + page_number * number_of_entries].get_database_name().length() / 2));    
                printw("%d.%s", database_vector[i + page_number * number_of_entries].get_database_id(), database_vector[i + page_number * number_of_entries].get_database_name().c_str());                         // print ID.Name
            }                                                                                               // TODO: THIS NEEDS TO HAVE PAGES!!!
        } 
    }
}

void MenuUI::print_current_database_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, Database current_database){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("%s", current_database.get_database_name().c_str());
    print_program_tittle();
    draw_line(decorator_type);
    refresh();

    string temp_string = "Press Enter to go back";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "Database is empty. No objects to display.";
    if(is_empty){                                                         // if the database is empty
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 6, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());              // tell the user there's nothing to display
    }else{                                                                // otherwise
                                                                          // write out the database structure
        temp_string = "ID";
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION);
        printw("%s", temp_string.c_str());

        temp_string = "Name";
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION + 5); // 5 -> offset
        printw("%s", temp_string.c_str());
                                 
        temp_string = "Type";
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION + 50); // 50 -> offset for name
        printw("%s", temp_string.c_str());
                                 
        temp_string = "Quantity";
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION + 80); // 80 -> offset for type
        printw("%s", temp_string.c_str());                                             

        if(is_paged){
            temp_string = "Page " + to_string(page_number + 1);
            move(CENTERED_AVAILABLE_OPTIONS_START_LINE + 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
            printw("%s", temp_string.c_str());
        }

        vector<Object> current_database_objects = current_database.get_database_objects();
        for(int i = 0; i < number_of_entries; i++){                                    // and then print the objects, as many on a page as selected by the user (number_of_entries)
            if(i + page_number * number_of_entries < static_cast<int>(current_database_objects.size())){
                move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);
                printw("%d",current_database_objects[i + page_number * number_of_entries].get_id());
                move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION + 5);
                printw("%s",current_database_objects[i + page_number * number_of_entries].get_name().c_str());
                move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION + 50);
                printw("%s", current_database_objects[i + page_number * number_of_entries].get_type().c_str());
                move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION + 80);
                printw("%d", current_database_objects[i + page_number * number_of_entries].get_quantity());
                refresh();
            }
        }
    }
}

void MenuUI::print_settings_menu(char decorator_type, bool text_position){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Settings Menu");
    print_program_tittle();
    draw_line(decorator_type);                      // draw the bottom line

    // write out the menu options, infos and warnings
    string temp_string = "1. Decorator type";
    if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - 5, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE - 5, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "2. Text position";
    if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - 4, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "3. Sound";
    if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - 3, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "4. Number of entries";
    if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - 2, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE - 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "5. Continuous mode";
    if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - 1, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE - 1, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "6. Save settings";
    if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE, LINE_TEXT_START_POSITION);
    else
        move(AVAILABLE_OPTIONS_START_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "Press Enter to go back.";
    if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    else
        move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
    refresh();
}

// Manage Database Sub-Menus
void MenuUI::print_load_database(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, vector<Database> database_vector){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Manage Databases Menu");
    print_program_tittle();

    draw_line(decorator_type);                                                                     // draw the bottom line
    // write out the menu options
    string temp_string = "Press Enter to go back";
    if(!is_empty){
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
    }

    if (is_empty){                    // check if there are any databases to print
        temp_string = "There are no available databases.";
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 6, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str()); // and tell the user there's nothing to display

        temp_string = "Press any key to continue...";
        if(text_position == 1)
            move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        else
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
        return;
    }else{
        temp_string = "Available Databases";
        if(text_position == 1)                                                                      // check if text should be left side aligned or centered and write accordingly
            move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, LINE_TEXT_START_POSITION);
        else
            move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());

        for(int i = 0; i < number_of_entries; i++){
            if(i + page_number * number_of_entries < static_cast<int>(database_vector.size())){
                if(text_position == 1)                                                                       // check if text should be left side aligned or centered and write accordingly
                    move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);
                else
                    move(AVAILABLE_OPTIONS_START_LINE - i, CENTERED_LINE_TEXT_START_POSITION - (database_vector[i + page_number * number_of_entries].get_database_name().length() / 2));
                printw("%d.%s", database_vector[i + page_number * number_of_entries].get_database_id(), database_vector[i + page_number * number_of_entries].get_database_name().c_str());
            }
        }
        
        if(is_paged){
            temp_string = "Page " + to_string(page_number + 1);
            move(CENTERED_AVAILABLE_OPTIONS_START_LINE + 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
            printw("%s", temp_string.c_str());
        }
    }
    refresh();
}

void MenuUI::print_choose_database(bool text_position){
    move(USER_INPUT_LINE, 3);
    for(int i = 3; i < COLS - 4; i++)
        printw(" ");
    
    string temp_string = "Choose database: ";
    if(text_position == 1)
        move(USER_INPUT_LINE, 3);
    else
        move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
}

void MenuUI::print_found_databases(bool text_position, int number_of_entries, int page_number, vector<Database> found_databases){
    for(int i = 0; i < number_of_entries; i++){
            if(i + page_number * number_of_entries < static_cast<int>(found_databases.size())){
                if(text_position == 1)                                                                       // check if text should be left side aligned or centered and write accordingly
                    move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);
                else
                    move(AVAILABLE_OPTIONS_START_LINE - i, CENTERED_LINE_TEXT_START_POSITION - (found_databases[i + page_number * number_of_entries].get_database_name().length() / 2));
                printw("%d.%s", found_databases[i + page_number * number_of_entries].get_database_id(), found_databases[i + page_number * number_of_entries].get_database_name().c_str());
            }
    }
}

void MenuUI::print_find_menu(bool text_position, int number_of_entries, bool is_database){
    for (int i = 0; i < number_of_entries; i++){
            move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);
            for (int j = LINE_TEXT_START_POSITION; j < LAST_COLUMN_OF_TEXT - LINE_TEXT_START_POSITION; j++)
                printw(" ");
    }
    
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE + 2, LINE_TEXT_START_POSITION);
    for(int i = LINE_TEXT_START_POSITION; i < LAST_COLUMN_OF_TEXT - LINE_TEXT_START_POSITION; i++)
        printw(" ");

    clear_user_input_zone();
    
    string temp_string;
    if(is_database){
        temp_string = "Matching Databases";
        if(text_position == 1)                                  // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, LINE_TEXT_START_POSITION);
        else
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
    }else{
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION);
        for(int i = LINE_TEXT_START_POSITION; i < LAST_COLUMN_OF_TEXT - LINE_TEXT_START_POSITION; i++)
            printw(" ");

        temp_string = "Matching Objects";
        if(text_position == 1)                                  // check if text should be left side aligned or centered and write accordingly
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 3, LINE_TEXT_START_POSITION);
        else
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 3, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
    }

    temp_string = "Search:";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
}

void MenuUI::print_search_word(const string& search_word){
    move(USER_INPUT_LINE, 10);
    for (int i = 0; i < COLS - 11; i++) 
        printw(" ");
    move(USER_INPUT_LINE, 10);
    printw("%s", search_word.c_str());
}

void MenuUI::print_load_database_options(bool is_empty, bool is_paged, bool show_clear_button){
    clear_user_input_zone();
    string temp_string = "1 - Previous Page";
    if(!is_empty){
        int offset = 5;
        int current_position = LINE_TEXT_START_POSITION + offset;
        if(is_paged & (!show_clear_button)){                                 // if there ARE pages, show the page buttons
            move(USER_INPUT_LINE, current_position);
            printw("%s", temp_string.c_str());
        }
        current_position += temp_string.length() + offset;

        temp_string = "2 - Choose Database";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
        current_position += temp_string.length() + offset;

        temp_string = "3 - Find";
        if(!show_clear_button){
            move(USER_INPUT_LINE, current_position);
            printw("%s", temp_string.c_str());
        }else{
            temp_string = "3 - Clear";
            move(USER_INPUT_LINE, current_position);
            printw("%s", temp_string.c_str());
        }
        current_position += temp_string.length() + offset;

        temp_string = "4 - Edit";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
        current_position += temp_string.length() + offset;

        if(is_paged & (!show_clear_button)){
            temp_string = "5 - Next Page";
            move(USER_INPUT_LINE, current_position);
            printw("%s", temp_string.c_str());
        }
    }else{
        temp_string = "Press any key to continue...";
        move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
    }
}

void MenuUI::print_edit_database_menu(bool text_position){
    move(USER_INPUT_LINE, 3);
    for(int i = 3; i < COLS - 4; i++)
        printw(" ");
    
    clear_user_input_zone();
    string temp_string = "Choose database to edit: ";
    if(text_position == 1)
        move(USER_INPUT_LINE, 3);
    else
        move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
}

void MenuUI::print_edit_database_name(){
    clear_user_input_zone();
    string temp_string = "Set new database name: ";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
}

// Create Database Sub-Menus
void MenuUI::print_database_already_exists(bool text_position){
    clear_user_input_zone();    
    string temp_string = "This database already exists!";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "WARNING: This can lead to loss of important data!!!";   
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);                                                                                       // check if the database already exists
    printw("%s", temp_string.c_str());

    temp_string = "Do you want to overwrite? (Y / N)"; 
    if(text_position == LEFT_ALIGNED)
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    else
        move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str()); // and if it does, warn the user
}

void MenuUI::print_database_created_successfully(bool text_position){
    clear_user_input_zone();
    string temp_string = "Database created successfully! Press any key to continue...";
    if(text_position == LEFT_ALIGNED)                    // check if text should be left side aligned or centered and write accordingly
            move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        else
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());        // tell the user that the database was created successfully!
}

// Delete Database Sub-Menus
void MenuUI::print_delete_database_menu_options(bool is_empty, bool is_paged, bool show_clear_button){
    if(!is_empty){
        int initial_offset = 8;
        int offset = 5;
        int current_position = LINE_TEXT_START_POSITION + initial_offset + offset;

        string temp_string;
        if(is_paged & (!show_clear_button)){
            temp_string = "1 - Previous Page";
            move(USER_INPUT_LINE, current_position);
            printw("%s", temp_string.c_str());
            current_position += temp_string.length() + offset;
        }

        temp_string = "2 - Choose Database";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
        current_position += temp_string.length() + offset;
        
        if(show_clear_button){
            temp_string = "3 - Clear";
            move(USER_INPUT_LINE, current_position);
            printw("%s", temp_string.c_str());
            current_position += temp_string.length() + offset;
        }else{
            temp_string = "3 - Find";
            move(USER_INPUT_LINE, current_position);
            printw("%s", temp_string.c_str());
            current_position += temp_string.length() + offset;
        }

        if(is_paged & (!show_clear_button)){
            temp_string = "3 - Next Page";
            move(USER_INPUT_LINE, current_position);
            printw("%s", temp_string.c_str());
        }
    }
}

void MenuUI::print_delete_database_choose_menu(bool text_position){
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    for(int i = LINE_TEXT_START_POSITION; i < LAST_COLUMN_OF_TEXT - LINE_TEXT_START_POSITION; i++)
        printw(" ");

    string temp_string = "Choose database to delete: ";
    if(text_position == 1)
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    else
        move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
}

void MenuUI::print_delete_database_confirm_deletion_menu(){
    // write the new info message to teach the user how to confirm deleting the database
    string temp_string = "Type DELETE to confirm deleting the database!";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 1, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "Leave blank and press Enter to cancel and go back";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
}

void MenuUI::print_delete_database_confirm_deletion_prompt(bool is_confirmed){
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    if(is_confirmed)
        printw("Database deleted! Press any key to continue...");
    else
        printw("Database could not be deleted! Press any key to continue...");
}

// Available Databases Sub-Menus
void MenuUI::print_available_databases_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, vector<Database> database_vector){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("All Available Databases");
    print_program_tittle();
    draw_line(decorator_type);           // draw the bottom line
    refresh();

    string temp_string;
    if(is_empty){
        temp_string = "There are no available databases.";
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 6, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str()); // and tell the user there's nothing to display
        
        temp_string = "Press any key to continue...";
        if(text_position == 1)
            move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        else
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
    }else{
        if(is_paged){
            temp_string = "Page " + to_string(page_number + 1);
            move(CENTERED_AVAILABLE_OPTIONS_START_LINE + 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
            printw("%s", temp_string.c_str());
        }

        temp_string = "Database Name";
        int database_name_offset = 5 + temp_string.length();
        if(text_position == 1)                                // check if text should be left side aligned or centered and write accordingly
            move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, database_name_offset);
        else
            move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, CENTERED_LINE_TEXT_START_POSITION + database_name_offset);
        printw("%s", temp_string.c_str());

        temp_string = "Database ID";                                               
        int database_id_offset = 5 + temp_string.length();
        if(text_position == 1)                                // check if text should be left side aligned or centered and write accordingly
            move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, LINE_TEXT_START_POSITION);
        else
            move(AVAILABLE_OPTIONS_START_LINE - number_of_entries, CENTERED_LINE_TEXT_START_POSITION - database_id_offset);
        printw("%s", temp_string.c_str());

        for(int i = 0; i < number_of_entries; i++){                                                                                                                                          
            if(i + page_number * number_of_entries < static_cast<int>(database_vector.size())){
                if(text_position == 1){ // check if text should be left side aligned or centered and write accordingly
                    move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);    
                    printw("%d", database_vector[i + page_number * number_of_entries].get_database_id());
                    move(AVAILABLE_OPTIONS_START_LINE - i, database_name_offset);
                    printw("%s", database_vector[i + page_number * number_of_entries].get_database_name().c_str());                                 // ID         Name
                }else{
                    move(AVAILABLE_OPTIONS_START_LINE - i, CENTERED_LINE_TEXT_START_POSITION - database_id_offset + temp_string.length() / 2);
                    printw("%d", database_vector[i + page_number * number_of_entries].get_database_id());
                    move(AVAILABLE_OPTIONS_START_LINE - i, CENTERED_LINE_TEXT_START_POSITION + database_name_offset - database_vector[i + page_number * number_of_entries].get_database_name().length() / 2 + 5);
                    printw("%s", database_vector[i + page_number * number_of_entries].get_database_name().c_str());                                 // ID         Name
                }
            }
        }
    }
}

void MenuUI::print_available_databases_options(bool is_empty, bool is_paged){
    if(!is_empty){
        string temp_string;
        temp_string = "Enter - Back";
        move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());
        int offset = 5 + temp_string.length();
        
        if(is_paged){
            temp_string = "1 - Previous Page";                          
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - offset - temp_string.length());
            printw("%s", temp_string.c_str());

            temp_string = "3 - Next Page";
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION + offset);
            printw("%s", temp_string.c_str());
        }
    }
}

// Current Database Sub-Menus
void MenuUI::print_current_database_menu_options(bool is_empty, bool is_paged, bool show_clear_button){
    int offset = 5;
    int current_position = LINE_TEXT_START_POSITION + offset;
    
    string temp_string = "1 - Previous Page";
    if(is_paged & (!show_clear_button)){                                 // if there ARE pages, show the page buttons
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
    }
    current_position += offset + temp_string.length();

    temp_string = "2 - Add";
    move(USER_INPUT_LINE, current_position);
    printw("%s", temp_string.c_str()); 
    current_position += offset + temp_string.length();  

    if(!is_empty){                                // if it's NOT empty, show the delete button
        temp_string = "3 - Delete";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
    }
    current_position += offset + temp_string.length();

    if(!is_empty & (!show_clear_button)){
        temp_string = "4 - Find";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
    }else{
        temp_string = "4 - Clear";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
    }
    current_position += offset + temp_string.length();

    if(!is_empty){
        temp_string = "5 - Edit";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
    }
    current_position += offset + temp_string.length();
    
    if(is_paged & (!show_clear_button)){ 
        temp_string = "6 - Next Page";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
    }
}

void MenuUI::print_current_database_menu_delete_object_prompt(int number_of_elements_deleted){
    if(number_of_elements_deleted > 0){                                                     
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        printw("%d element(s) were deleted from the database! Press any key to continue...", number_of_elements_deleted); // and print the number of deleted elements
        getch();                                                                                                         // wait user input before moving on
    }else{
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        printw("No element was deleted from the database! Press any key to continue...");                        // otherwise, inform the user about the problem
        getch();                                                                                                         // and wait for user input
    }
}

void MenuUI::print_add_object_menu(char decorator_type){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Add Object Menu");
    print_program_tittle();
    draw_line(decorator_type);                                        // draw the bottom line
    
    string temp_string;
    // inform the user as to how to create new objects
    move(LINES / 2 - 4, COLS / 2 - 23);
    printw( "Objects are created using the following format:");

    temp_string = "**********************";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, LAST_COLUMN_OF_TEXT - temp_string.length() - 2); // 2 -> little space between * and border
    printw("%s", temp_string.c_str());

    temp_string = "name type quantity";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - temp_string.length() - 4); // 4 -> little space between text and border
    printw("%s", temp_string.c_str());

    temp_string = "**********************";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - temp_string.length() - 2); // 2 -> little space between * and border
    printw("%s", temp_string.c_str());

    temp_string = "Leave blank and press Enter to cancel and go back";
    move(AVAILABLE_OPTIONS_START_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
    
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
}

void MenuUI::print_add_object_result(bool result){
    if(result)
        printw("Object added to the database!");
    else
        printw("Invalid object! Please make sure to follow the correct format!"); 
}

void MenuUI::print_delete_object_menu(){
    // print on top of the database menu
    string temp_string = "Delete by...";
    move(MENU_TITTLE_LINE_POSITION + 2, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "1. ID";
    move(MENU_TITTLE_LINE_POSITION + 4, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "2. Name";
    move(MENU_TITTLE_LINE_POSITION + 5, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "3. Type";
    move(MENU_TITTLE_LINE_POSITION + 6, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "WARNING!!!";
    move(MENU_TITTLE_LINE_POSITION + 9, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "All matching objects will be deleted!!!";
    move(MENU_TITTLE_LINE_POSITION + 10, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "Press Enter to cancel and go back.";
    move(MENU_TITTLE_LINE_POSITION + 12, LINE_TEXT_START_POSITION);        
    printw("%s", temp_string.c_str());

    refresh();
    
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    for(int i = LINE_TEXT_START_POSITION; i < COLS - LINE_TEXT_START_POSITION; i++)
        printw(" ");
}

void MenuUI::print_delete_by_id(){
    string temp_string = "Delete by ID";
    move(MENU_TITTLE_LINE_POSITION + 2, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "ID of the item to delete: ";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
    refresh();
}

void MenuUI::print_delete_by_name(){
    string temp_string = "Delete by Name";
    move(MENU_TITTLE_LINE_POSITION + 2, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "Name of the item to delete: ";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
    refresh();
}

void MenuUI::print_delete_by_type(){
    string temp_string = "Delete by Type";
    move(MENU_TITTLE_LINE_POSITION + 2, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "Type of the item to delete: ";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
    refresh();
}

void MenuUI::print_edit_object_menu(){
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Edit Object Menu");

    string temp_string = "Objects are edited using the following format:";
    // inform the user as to how to create new objects
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "All the features are optional, but there must be at least one of them";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "To skip one feature leave blank (press space)";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 0, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "**********************";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, LAST_COLUMN_OF_TEXT - temp_string.length() - 2); // 2 -> little space between * and border
    printw("%s", temp_string.c_str());

    temp_string = "name type quantity";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - temp_string.length() - 4); // 4 -> little space between text and border
    printw("%s", temp_string.c_str());

    temp_string = "**********************";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - temp_string.length() - 2); // 2 -> little space between * and border
    printw("%s", temp_string.c_str());

    temp_string = "Leave blank and press Enter to cancel and go back";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
    
    clear_user_input_zone();
    temp_string = "Choose item to edit (by ID):";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
}

void MenuUI::print_edit_object(char decorator_type, int object_id, Database current_database){
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Edit Object Menu");
    print_program_tittle();
    draw_line(decorator_type);

    string temp_string = "Object chosen for edit";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = current_database.get_object_by_id(object_id);
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "Enter new object details:";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
}   

void MenuUI::print_found_objects(int number_of_entries, int page_number, const std::vector<Object>& found_objects){
        string temp_string = "ID";
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION);
        printw("%s", temp_string.c_str());

        temp_string = "Name";
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION + 5); // 5 -> offset
        printw("%s", temp_string.c_str());
                                 
        temp_string = "Type";
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION + 50); // 50 -> offset for name
        printw("%s", temp_string.c_str());
                                 
        temp_string = "Quantity";
        move(AVAILABLE_OPTIONS_START_LINE - number_of_entries - 1, LINE_TEXT_START_POSITION + 80); // 80 -> offset for type
        printw("%s", temp_string.c_str());                                             

        // temp_string = "Page " + to_string(page_number + 1);
        // move(CENTERED_AVAILABLE_OPTIONS_START_LINE + 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        // printw("%s", temp_string.c_str());

        for(int i = 0; i < number_of_entries; i++){                                    // and then print the objects, as many on a page as selected by the user (number_of_entries)
            if(i + page_number * number_of_entries < static_cast<int>(found_objects.size())){
                move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);
                printw("%d",found_objects[i + page_number * number_of_entries].get_id());
                move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION + 5);
                printw("%s",found_objects[i + page_number * number_of_entries].get_name().c_str());
                move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION + 50);
                printw("%s", found_objects[i + page_number * number_of_entries].get_type().c_str());
                move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION + 80);
                printw("%d", found_objects[i + page_number * number_of_entries].get_quantity());
                refresh();
            }
        }
}

// Settings Sub-Menus
void MenuUI::print_decorator_settings(char decorator_type){
    move(AVAILABLE_OPTIONS_START_LINE - 5, LINE_TEXT_START_POSITION);
    for(int i = 0; i < 17; i++)
        printw(" ");
    
    string temp_string = "Decorator Type:";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "1. *";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "2. ~";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "3. +";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 2, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "4. /";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 1, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "(Current)";
    switch (decorator_type){                // show which is the currently selected option
    case '*':
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 36 + temp_string.length() / 2);
        break;
    case '~':
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 36 + temp_string.length() / 2);
        break;
    case '+':
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 2, LAST_COLUMN_OF_TEXT - 36 + temp_string.length() / 2);
        break;
    case '/':
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 1, LAST_COLUMN_OF_TEXT - 36 + temp_string.length() / 2);
        break;
    }
    printw("%s", temp_string.c_str());

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, CENTERED_LINE_TEXT_START_POSITION - 10);
    for(int i = 0; i < 19; i++)
        printw("*");

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - 10);
    for(int i = 0; i < 19; i++)
        printw("*");

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 18; i++)
        printw("*");
    
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 18; i++)
        printw("*");
}

void MenuUI::print_text_position_settings(bool text_position){
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LINE_TEXT_START_POSITION);
    for(int i = 0; i < 16; i++)
        printw(" ");
    
    string temp_string = "Text position:";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - 7);
    printw("%s", temp_string.c_str());

    temp_string = "1. Left side";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "2. Centered";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());
    
    temp_string = "(Current)";
    if(text_position)                       // show which is the currently selected option
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 24);                
    else
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 24);
    printw("%s", temp_string.c_str());
    
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, CENTERED_LINE_TEXT_START_POSITION - 9);
    for(int i = 0; i < 18; i++)
        printw("*");

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - 9);
    for(int i = 0; i < 18; i++)
        printw("*");

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 26; i++)
        printw("*");
    
    move (CENTERED_AVAILABLE_OPTIONS_START_LINE - 2, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 26; i++)
        printw("*");
}

void MenuUI::print_sound_settings(bool sound){
    move(AVAILABLE_OPTIONS_START_LINE - 3, LINE_TEXT_START_POSITION);
    for(int i = 0; i < 8; i++)
        printw(" ");

    string temp_string = "Sound:";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - 2);
    printw("%s", temp_string.c_str());

    temp_string = "1. ON";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "2. OFF";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "(Current)";
    if(sound){                              // show which is the currently selected option
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 30);
    }else{
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 30);
    }
    printw("%s", temp_string.c_str());

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, CENTERED_LINE_TEXT_START_POSITION - 4);
    for(int i = 0; i < 10; i++)
        printw("*");

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - 4);
    for(int i = 0; i < 10; i++)
        printw("*");

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 20; i++)
        printw("*");
    
    move (CENTERED_AVAILABLE_OPTIONS_START_LINE - 2, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 20; i++)
        printw("*");
}

void MenuUI::print_number_of_entries_settings(int number_of_entries){
    move(AVAILABLE_OPTIONS_START_LINE - 2, LINE_TEXT_START_POSITION);
    for(int i = 0; i < 20; i++)
        printw(" ");
    
    string temp_string = "Number of entries:";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - 9);
    printw("%s", temp_string.c_str());

    temp_string = "1. 3 Entries per page";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "2. 5 Entries per page";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "3. 7 Entries per page";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 2, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "(Current)";
    switch (number_of_entries){              // show which is the currently selected option
    case 3:
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 15);
        break;
    case 5:
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 15);
        break;
    case 7:
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 2, LAST_COLUMN_OF_TEXT - 15);
        break;
    }
    printw("%s", temp_string.c_str());

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, CENTERED_LINE_TEXT_START_POSITION - 11);
    for(int i = 0; i < 22; i++)
        printw("*");

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - 11);
    for(int i = 0; i < 22; i++)
        printw("*");

    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 35; i++)
        printw("*");
    
    move (CENTERED_AVAILABLE_OPTIONS_START_LINE - 1, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 35; i++)
        printw("*");
}

void MenuUI::print_continuous_mode_settings(bool continuous_mode){
    move(AVAILABLE_OPTIONS_START_LINE - 1, LINE_TEXT_START_POSITION);
    for(int i = 0; i < 18; i++)
        printw(" ");
    
    string temp_string = "Continuous mode:";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - 8);
    printw("%s", temp_string.c_str());

    temp_string = "1. ON";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());

    temp_string = "2. OFF";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 37);
    printw("%s", temp_string.c_str());
    
    temp_string = "(Current)";
    if(continuous_mode)                    // show which is the currently selected option
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, LAST_COLUMN_OF_TEXT - 30);
    else
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, LAST_COLUMN_OF_TEXT - 30);
    printw("%s", temp_string.c_str());
 
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, CENTERED_LINE_TEXT_START_POSITION - 10);
    for(int i = 0; i < 20; i++)
        printw("*");
 
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 3, CENTERED_LINE_TEXT_START_POSITION - 10);
    for(int i = 0; i < 20; i++)
        printw("*");
 
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 20; i++)
        printw("*");
    
    move (CENTERED_AVAILABLE_OPTIONS_START_LINE - 2, LAST_COLUMN_OF_TEXT - 39);
    for(int i = 0; i < 20; i++)
        printw("*");

}

// Special Functions
void MenuUI::print_program_tittle(){
    move(1,PROGRAM_TITTLE_COLUMN_POSTION);
    printw("Terminal Database Manager");
}

void MenuUI::print_invalid_input(bool text_position){
    string temp_string = "Invalid input! Press any key to continue...";
    if(text_position == 1)                                                             // check if text should be left side aligned or centered and write accordingly
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    else                                                       
        move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());  // and warn him about it
}

void MenuUI::clear_user_input_zone(){
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    for(int i = 3; i < LAST_COLUMN_OF_TEXT - LINE_TEXT_START_POSITION; i++)
        printw(" ");
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
}

void MenuUI::clear_search_results(int number_of_entries){
    for (int i = 0; i < number_of_entries; i++){
        move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);
        for (int j = LINE_TEXT_START_POSITION; j < LAST_COLUMN_OF_TEXT - LINE_TEXT_START_POSITION; j++)
            printw(" ");
    }
    refresh();
}
