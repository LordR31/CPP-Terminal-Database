// Menu.cpp
#include "Menu.hpp" // Include the corresponding header file

using namespace std;

Menu::Menu(){
    string input_settings = "program_settings.txt"; // path to program_settings
    ifstream input;

    // check if program settings can be opened
    // first time launching the program, program_settings and index_manager DO NOT exist!
    input.open(input_settings);
    if (!input) {                                               
        input.clear(); 
        ofstream output(input_settings);                            // if it can't be opened, create it 
        
        ifstream get_database_index;                                // try to open index_manager to get database index to avoid loss of data
        get_database_index.open("index_manager.txt");
        int index = 0;
        if(get_database_index.is_open()){                           // if it was opened, get database index
            string line;
            while(getline(get_database_index, line))
            index++;
        }else{                                                      // otherwise, create index_manager
            ofstream create_index_manager;
            create_index_manager.open("index_manager.txt");
            if(!create_index_manager.is_open()){                    // if it cannot be created, cry error and exit.
                cout << "ERROR: Could not initialize settings!!";
                exit(0);
            }
        }

        // write the program_settings to file
        output << "Current_database_index=" << index << '\n';
        output << "Decorator_type=1\n";
        output << "Text_position=1\n";
        output << "Sound=0\n";
        output << "Number_of_entries=5\n";
        output << "Continuous_mode=1\n";

        // close the file as output and open as input
        output.close();
        input.open(input_settings);
        }

    // get program settings parameters
    int settings_parameters[6] = {0};
    if(input.is_open()){
        int i = 0;
        string line;
        while(getline(input, line)){
            size_t position = line.find("=");
            settings_parameters[i] = line[position + 1] - '0';
            i++;
        }
    } else{                                                      // if program_settings could not be opened again, cry error and exit
        cout << "program_settings.txt could not be opened!\n";
        exit(0);
    }
    input.close();                                               // close the file after reading

    // assign the settings parameters
    current_database_index=settings_parameters[0];
    decorator_type = settings_parameters[1];
    switch (settings_parameters[1]){
    case 1:
        decorator_type = '*';
        break;
    case 2:
        decorator_type = '~';
        break;
    case 3:
        decorator_type = '+';
        break;
    case 4:
        decorator_type = '/';
        break;
    }
    text_position = settings_parameters[2];
    sound = settings_parameters[3];
    number_of_entries = settings_parameters[4];
    continuous_mode = settings_parameters[5];

    // read the index_manager to get the existing databases
    ifstream available_databases;
    available_databases.open("index_manager.txt"); 
    if(available_databases.is_open()){                          // check again if the file exists... it SHOULD exist!
        string line;
        while(getline(available_databases, line)){
            Database temp_database;
            temp_database.load_database(line);
            database_vector.emplace_back(temp_database);
        }
    } else{                                                     // if it doesn't exist somehow, create it, but risk LOSING data
        available_databases.clear();
        ofstream output;
        output.open("index_manager.txt");
        output.close();
        available_databases.open("index_manager.txt");
    }

    if(!available_databases.is_open()){                         // if it cannot be opened after it was created, cry error and exit!
        cout << "index_manager.txt could not be loaded!\n";
        exit(0);
    }
    available_databases.close();

    string files_path = "files";
    filesystem::path folderPath(files_path);

    if(!filesystem::exists(folderPath))                      // check if files folder exists
        if(!filesystem::create_directory(folderPath)){        // if not, try to create it
            cout << "ERROR: Could not create files folder!"; // if not possible, cry error and exit
            exit(0);
        }
}

void Menu::print_program_tittle(){
    move(1,PROGRAM_TITTLE_COLUMN_POSTION);
    printw("Terminal Database Manager");
}

void Menu::print_main_menu(){
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

int Menu::main_menu(){
    print_main_menu();
    while(true){
        if(check_resize())     // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 0;

        int choice = getch();  // get user input
        if(sound)              // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
        case 49: // 1 in ASCII    Enter Manager Databases Menu
            return 1;
            break;
        case 50: // 2 in ASCII    Enter Settings Menu
            return 7;
            break;
        case 51: // 3 in ASCII    Exit program!
            return 20;
        default:
            break;
        }
    }
}

void Menu::print_manage_databases_menu(){
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

int Menu::manage_databases_menu(){
    page_number = 0; // reset all_database_page_number
    print_manage_databases_menu();
    while(true){    
        if(check_resize())                             // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 1;

        int choice = getch(); // get user input
        if(sound)             // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
        case 10: // Enter in ASCII         Go back
            return 0;
        case 49: // 1 in ASCII             Create Database Menu
            return 2;
            break;
        case 50: // 2 in ASCII             Load Database Menu
            return 3;
            break;
        case 51: // 3 in ASCII             Delete Database Menu
            return 4;
            break;
        case 52: // 4 in ASCII             Print all available databases
            return 5;
            break;
        default:
            continue;
        }
    }
}

void Menu::print_load_database(bool is_empty, bool is_paged){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(MENU_TITTLE_LINE_POSITION, LINE_TEXT_START_POSITION);
    printw("Manage Databases Menu");
    print_program_tittle();

    draw_line(decorator_type);                                                                     // draw the bottom line
    // write out the menu options
    string temp_string = "Press Enter to go back";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

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

void Menu::print_load_database_options(bool is_paged, bool show_clear_button){
    string temp_string = "1 - Previous Page";
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

    temp_string = "4 - Edit";
    move(USER_INPUT_LINE, current_position);
    printw("%s", temp_string.c_str());
    current_position += temp_string.length() + offset;

    if(is_paged & (!show_clear_button)){
        temp_string = "5 - Next Page";
        move(USER_INPUT_LINE, current_position);
        printw("%s", temp_string.c_str());
    }
}

int Menu::load_database_menu(){
    bool is_paged = false;
    bool is_empty = database_vector.empty();
    bool is_in_find_interface = false;

    if(!is_empty)
        if(static_cast<int>(database_vector.size()) > number_of_entries) // if there IS something to display, check if you need pages
            is_paged = true;

    print_load_database(is_empty, is_paged);
    print_load_database_options(is_paged, is_in_find_interface);
        
    if(is_empty){
        getch();
        return 1; 
    }

    while(true){
        int choice = getch(); // get user input
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 3;

        if(sound)             // make ANNOYING sound if sound turned on 
            beep();

        switch (choice){
            case 49:
                if(is_paged & ~is_in_find_interface)                                                                                                        // if the button is shown (there are pages)
                    decrement_page(true);
                return 3;
            case 50:
                if(choose_database())
                    return 6;
                else
                    return 3;
            case 51:
                if(is_in_find_interface){
                    return 3;
                }else{
                    find_database();
                    clear_user_input_zone();
                    is_in_find_interface = true;
                    print_load_database_options(is_paged, is_in_find_interface);
                }
                continue;
            case 52:
                if(edit_database()){
                    update_index_manager();
                    reload_database_vector();
                }
                return 3;
            case 53:
                if(is_paged & ~is_in_find_interface)                                                                                                     // if the button is shown (there are pages)            
                    increment_page(true);                                                                                 // otherwise just ignore this input
                return 3; 
            case 10:
                return 1;
            default:
                continue;
        }
    }
    return 6;                                                                              // go to Print Current Database Menu
}

int Menu::create_database_menu(){
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
    if(text_position == 1)                                        // check if text should be left side aligned or centered and write accordingly
            move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        else
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    echo();                                                                                                                            // enable echo 
    nocbreak();                                                                                                                        // enable buffering
    char database_name [256];
    getnstr(database_name, sizeof(database_name) - 1);                                                                                 // get user input
    if(sound)                                                                                                                          // make ANNOYING sound if sound turned on
        beep();
    noecho();                                                                                                                          // no echo
    cbreak();                                                                                                                          // no buffering

    if(check_resize())                                                                                                                 // check if the window was resized by the user and re-enter the menu to re-draw everything
        return 2;

    if(database_name[0] == '\0')                                                                                                       // if the user pressed Enter, go back 
        return 1;

    string database_path = "files/" + (string)database_name + ".txt";                                                                  // otherwise, create the path to the database 

    if(filesystem::exists(database_path)){  
        temp_string = "This database already exists!";
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 5, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());

        temp_string = "WARNING: This can lead to loss of important data!!!";   
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 4, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);                                                                                       // check if the database already exists
        printw("%s", temp_string.c_str());

        temp_string = "Do you want to overwrite? (Y / N)"; 
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        printw("%s", temp_string.c_str()); // and if it does, warn the user
        while(true){                                                                                                                   // and determine the next course of action
            int choice = getch();
            if(choice == 121 || choice == 89){      // lower and upper case y in ASCII                                                 // if they want to overwrite
                ofstream output;                                                          
                output.open(database_path);                                                                                            // overwrite file
                output << "Database_id=" << current_database_index << endl;                                                            // write the database index 
                output << "Next_object_id=0\n";                                                                                        // write the next object's id (0 for new databases)
                output.close();   
                reload_database_vector();                                                                                              // close the file
                return 1;
            }else if(choice == 110 || choice == 78) // lower and upper case n in ASCII                                                   
                return 1;                                                                                                              // otherwise, return to Manage Databases Menu
        }
    }else{                                                                                                                             // but if the database does not already exist
        ofstream output;                                                          
        output.open(database_path);                                                                                                    // create the file
        output << "Database_id=" << current_database_index << endl;                                                                    // write the database index 
        output << "Next_object_id=0\n";                                                                                                // write the next object's id (0 for new databases)
        output.close();                                                                                                                // close the file
    }

    ofstream index_manager;
    index_manager.open("index_manager.txt", ios::app);                        // open index_manager to add the new database to "memory"
    index_manager << current_database_index << "." << database_name << endl;  // add the database index and name
    index_manager.close();                                                    // close the file

    reload_database_vector();                                                 // reload the database vector to reflect the changes
    get_new_database_index();                                                 // get the new current database index
    save_settings();                                                          // save the settings

    temp_string = "Database created successfully!";
    if(text_position == 1)                    // check if text should be left side aligned or centered and write accordingly
            move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        else
            move(USER_INPUT_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());        // tell the user that the database was created successfully!
    return 1;                                 // go back to the Database Manager Menu
}

void Menu::print_delete_database_menu(bool is_empty, bool is_paged){
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

void Menu::print_delete_database_menu_options(bool is_empty, bool is_paged, bool show_clear_button){
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

void Menu::print_delete_database_choose_menu(){
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

void Menu::print_delete_database_confirm_deletion_menu(){
    // write the new info message to teach the user how to confirm deleting the database
    string temp_string = "Type DELETE to confirm deleting the database!";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE - 1, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());

    temp_string = "Leave blank and press Enter to cancel and go back";
    move(CENTERED_AVAILABLE_OPTIONS_START_LINE, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
    printw("%s", temp_string.c_str());
}

bool Menu::delete_database(){
    bool is_confirmed = false;
    int i = 0;
    for(i; i < static_cast<int>(database_vector.size()); i++)
        if(database_vector[i].get_database_id() == current_database.get_database_id()){
            is_confirmed = current_database.delete_database();                          // if the user DID successfully type "DELETE", delete the database file, BUT get confirmation
            break;
        }

    if(is_confirmed){
        database_vector.erase(database_vector.begin() + i);              // remove the database from the vector
    }
    return is_confirmed;
}

void Menu::print_delete_database_confirm_deletion_prompt(bool is_confirmed){
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    if(is_confirmed)
        printw("Database deleted! Press any key to continue...");
    else
        printw("Database could not be deleted! Press any key to continue...");
}

int Menu::delete_database_menu(){
    bool is_paged = false;
    bool is_empty = database_vector.empty();
    bool is_in_find_interface = false;

    if (!is_empty){                          // check if there are any databases to print
        if(static_cast<int>(database_vector.size()) > number_of_entries)
            is_paged = true;
        }
    print_delete_database_menu(is_empty, is_paged);
    print_delete_database_menu_options(is_empty, is_paged, is_in_find_interface);
        
    if(is_empty){
        getch();
        return 1;
    }

    while(true){
        int choice = getch(); // get user input
        
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
           return 3;

        if(sound)             // make ANNOYING sound if sound turned on 
           beep();

        switch (choice){
                case 49:
                    if(is_paged)
                       decrement_page(true);
                    return 4;
                case 50:{
                    print_delete_database_choose_menu();
                    echo();
                    nocbreak();
                    char database_choice[3];
                    getnstr(database_choice, sizeof(database_choice) - 1);
                    noecho();
                    cbreak();
                    if(sound)                                                             // make ANNOYING sound if sound turned on
                        beep();
                    echo();                                      // echo on
                    nocbreak();                                  // buffering on

                    if(database_choice[0] == '\0')                                                // if the user pressed Enter, go back
                        return 4;
                    else {
                        for(int i = 0; i < static_cast<int>(database_vector.size()); i++)
                            if(stoi(database_choice) == database_vector[i].get_database_id()){
                                current_database = database_vector[i];
                                break;
                            }else if(i == static_cast<int>(database_vector.size())){
                                print_invalid_input();
                                return 4;
                            }
                    }
                    
                    print_delete_database_confirm_deletion_menu();
                    while(true){
                        clear_user_input_zone();
                        
                        //TODO: ADD RESIZE!!!

                        char confirmation[7];                        
                        getnstr(confirmation, sizeof(confirmation) - 1); // get user input
                        if(check_resize()){                              // check if the window was resized by the user and re-enter the menu to re-draw everything
                            noecho();                                 // disable echo
                            cbreak();                                 // disable buffering
                            return 4;
                        }
                        if(sound)                                    // make ANNOYING sound if sound turned on
                            beep(); 
                        noecho();                                    // no echo
                        cbreak();                                    // no buffering
                                
                        if(confirmation[0] == '\0')                  // if the user left blank and pressed Enter, go back
                            return 1;
                        else if(strcmp(confirmation, "DELETE") != 0){                                    // if the user unsuccessfully typed "DELETE"
                                print_invalid_input();
                                getch();
                                if(sound)
                                    beep();
                                return 4;
                        }else{
                            bool is_confirmed = delete_database();                                                            
                            update_index_manager();
                            reload_database_vector();                                                        // reload the database_vector to reflect the changes
                            get_new_database_index();
                            print_delete_database_confirm_deletion_prompt(is_confirmed);
                            getch();
                            return 1;
                        }
                    }
                    return 4;
                }
                case 51:
                    if(is_in_find_interface){
                        return 4;
                    }else{
                        find_database();
                        clear_user_input_zone();
                        is_in_find_interface = true;
                        print_delete_database_menu_options(is_empty, is_paged, is_in_find_interface);
                    }
                    continue;
                case 52:
                    increment_page(true);                                                                                                  // otherwise just ignore this input
                    return 4;
                case 10:
                    return 1;
                 default:
                     continue;
        }
    }
    return 1;                                                                                // return to Manage Databases Menu
}

void Menu::print_available_databases_menu(bool is_empty, bool is_paged){
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

void Menu::print_databases_options(bool is_empty, bool is_paged){
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

int Menu::available_databases_menu(){
    bool is_empty = database_vector.empty();
    bool is_paged = false;

    if (!is_empty)                    // check if there are any databases to print
        if(static_cast<int>(database_vector.size()) > number_of_entries) // if there IS something to display, check if you need pages
            is_paged = true;
            
    print_available_databases_menu(is_empty, is_paged);
    print_databases_options(is_empty, is_paged);
    
    while(true){
        int input = getch();                    // wait user input
        
        if(check_resize())                      // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 5;
        if(sound)                               // make ANNOYING sound if sound turned on
            beep();
        switch (input){
        case 49:{
            if(is_paged)                                                                                                       // if the button is shown (there are pages)
                decrement_page(true);
            return 5;
        }
        case 51:{
            if(is_paged)   
                increment_page(true);                                                                                                   // if the button is shown (there are pages)                                                                                                            // otherwise just ignore this input
            return 5;
        }  
        case 10:
            return 1;
        default:
            continue;
        }
    }
    return 1;                               // return to Manage Databases Menu
}

void Menu::print_current_database_menu(bool is_empty, bool is_paged){
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

        temp_string = "Page " + to_string(page_number + 1);
        move(CENTERED_AVAILABLE_OPTIONS_START_LINE + 2, CENTERED_LINE_TEXT_START_POSITION - temp_string.length() / 2);
        printw("%s", temp_string.c_str());

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

void Menu::print_current_database_menu_options(bool is_empty, bool is_paged, bool show_clear_button){
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

void Menu::print_current_database_menu_delete_object_prompt(int number_of_elements_deleted){
    if(number_of_elements_deleted > 0){
        int final_size = current_database.get_number_of_objects();                                                       // get final number of elements
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        printw("%d element(s) were deleted from the database! Press any key to continue...", number_of_elements_deleted); // and print the number of deleted elements
        getch();                                                                                                         // wait user input before moving on
    }else{
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
        printw("No element was deleted from the database! Press any key to continue...");                        // otherwise, inform the user about the problem
        getch();                                                                                                         // and wait for user input
    }
}

int Menu::current_database_menu(){
    vector<Object> current_database_objects = current_database.get_database_objects();

    bool is_paged = false;
    bool is_empty = current_database_objects.empty();
    bool is_in_find_interface = false;

    if (!is_empty)                                                                // check if the database is empty
        if(current_database.get_number_of_objects() > number_of_entries) // and if there are objects to display, check if you need pages
            is_paged = true;                                                     

    print_current_database_menu(is_empty, is_paged);
    print_current_database_menu_options(is_empty, is_paged, is_in_find_interface);

    // determine the next step based on user input
    while(true){
        int choice = getch();
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 6;
        if(sound)             // make ANNOYING sound if sound turned on
            beep();
        switch (choice){
            case 49:{                                                                                                               // 1 -> Previous page
                if(is_paged)                                                                                                        // if the button is shown (there are pages)
                    increment_page(false);                                                                                                // otherwise just ignore the input
                return 6;
            }
            case 50:{                                                                                                                // 2 -> Add item to database
                int status = add_object_menu();                                                                                      // call the function to add an object to the database
                while(status == 0)
                    status = add_object_menu();
                current_database.save_database();
                reload_database_vector();
                return 6;
            }
            case 51:{                                                                                                                // 3 -> Delete item from database
                int initial_size = current_database.get_number_of_objects();                                                         // get initial number of elements
                bool status = delete_object_menu();                                                                                   // try to delete element(s) and get confirmation
                
                if(status == 0)
                    return 6;
                else if(status == 1){
                    int final_size = current_database.get_number_of_objects();
                    print_current_database_menu_delete_object_prompt(initial_size - final_size);
                    current_database.save_database();                                                                                    // save database changes
                    reload_database_vector();                                                                                            // and reload the vector
                }
                return 6;
            }
            case 52:{     
                if(is_in_find_interface){
                    return 6;
                }else{
                    find_object_menu();  
                    clear_user_input_zone();
                    is_in_find_interface = true;
                    print_current_database_menu_options(is_empty, is_paged, is_in_find_interface);  
                }
                continue;                                                                                                          // exit loop to return to Load Database Menu
            }
                                                                                                                                    // TODO: Add number of elements counter to screen
                                                                                                                                    // TODO: Add number of pages to screen:    Page X/XX
            case 53:{
                edit_object_menu();
                current_database.save_database();
                reload_database_vector();
                return 6;
            }
            case 10:
                page_number = 0;
                return 3;
            case 54:{                                                                                                               // 5 -> Next page
                if(is_paged)                                                                                                       // if the button is shown (there are pages)            
                    decrement_page(false);
                return 6;
            }
            default:                                                                                                               // ignore other inputs
                continue;
               
        }
    }

    current_database.save_database(); // save database changes
    reload_database_vector();         // reload the database vector
    return 3;                         // return to load database menu
}

void Menu::print_add_object_menu(){
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

void Menu::print_add_object_result(bool result){
    if(result)
        printw("Object added to the database!");
    else
        printw("Invalid object! Please make sure to follow the correct format!"); 
}

int Menu::add_object_menu(){
    print_add_object_menu();
    while(true){
        echo();                              // enable echo 
        nocbreak();                          // enable buffering
        char object[256];
        getnstr(object, sizeof(object) - 1); // get user input
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 0;
        if(sound)                            // make ANNOYING sound if sound turned on
            beep();
        noecho();                            // no echo
        cbreak();                            // no buffering
            
        string object_string = object;       // make the input a string  
        if(object[0] == '\0')                // if the user left blank and pressed Enter, go back
            break;

        stringstream new_object(object_string);
        vector<string> temp_object;
        string temp_object_feature;

        while(getline(new_object, temp_object_feature, ' '))                          // so long as there are features to remove from that input line
            temp_object.push_back(temp_object_feature);                               // extract them into a vector
 
        bool result = false;
        try{                                                                          // then try to create an object with them
            string temp_name = temp_object[0];
            string temp_type = temp_object[1];
            int temp_quantity = stoi(temp_object[2]);

            result = current_database.add_object(temp_name, temp_type, temp_quantity);         // and add it to the database object vector

            
                                              // finally tell the user that the object was added to the database (never really shown cuz no getch() to pause)
        }catch(int errorCode){                                                        // if not possible, tell the user to carefully follow the format
            print_add_object_result(result);
            break;
        }
        print_add_object_result(result);
        break;
    }
    return 1;
}

void Menu::print_edit_object_menu(){
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
    temp_string = "Choose item to edit:";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
}

void Menu::print_edit_object(int object_id){
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

void Menu::edit_object_menu(){
    print_edit_object_menu();

    echo();                                                                                                                            // enable echo 
    nocbreak();                                                                                                                        // enable buffering
    char object_id [256];
    getnstr(object_id, sizeof(object_id) - 1);                                                                                 // get user input
    if(sound)                                                                                                                          // make ANNOYING sound if sound turned on
        beep();
    noecho();                                                                                                                          // no echo
    cbreak();                                                                                                                          // no buffering

    if(check_resize())                                                                                                                 // check if the window was resized by the user and re-enter the menu to re-draw everything
        return;

    if(object_id[0] == '\0')                                                                                                       // if the user pressed Enter, go back 
        return;
    
    print_edit_object(stoi(object_id));
    echo();                                                                                                                            // enable echo 
    nocbreak();                                                                                                                        // enable buffering
    char new_features [256];
    getnstr(new_features, sizeof(new_features) - 1);                                                                                 // get user input
    if(sound)                                                                                                                          // make ANNOYING sound if sound turned on
        beep();
    noecho();                                                                                                                          // no echo
    cbreak();

    if(new_features[0] == '\0')                                                                                                       // if the user pressed Enter, go back 
        return;

    bool is_name = true;
    bool is_type = true;
    bool is_quantity = true;

    if(new_features[0] == ' ')
        is_name = false;
    
    if(new_features[1] == ' ')
        is_type = false;

    if(new_features[2] == ' ')
        is_quantity = false;

    stringstream new_object(new_features);
    vector<string> temp_object;
    string temp_object_feature;

    string new_name;
    string new_type;
    int new_quantity;

    while(getline(new_object, temp_object_feature, ' ')){                          // so long as there are features to remove from that input line
        temp_object.push_back(temp_object_feature);                                // extract them into a vector
    }

    if(is_name){
        new_name = temp_object[0];
        if(is_type){
            new_type = temp_object[1];
            if(is_quantity)
                new_quantity = stoi(temp_object[2]);
        }else if(is_quantity)
            new_quantity = stoi(temp_object[1]);
    }else if(is_type){
        new_type = temp_object[0];
        if(is_quantity)
            new_quantity = stoi(temp_object[1]);
    }else
        new_quantity = stoi(temp_object[0]);

    if(is_name)
        if(is_type)
            if(is_quantity)
                current_database.edit_object(stoi(object_id), new_name, new_type, new_quantity);
            else
                current_database.edit_object(stoi(object_id), new_name, new_type);
        else
            current_database.edit_object(stoi(object_id), new_name);
    else
        return;

}

void Menu::find_object_menu(){
    print_find_menu(false);
    
    string input_word = "";
    int counter = 0;

    vector<Object> found_objects;

    while (true) {
        int input_char = getch();

        if (input_char == KEY_BACKSPACE){
            if (counter > 0) {
                input_word.pop_back();
                counter--;
            }
        }else if (input_char == 10 || input_char == KEY_ENTER){
            noecho();
            cbreak();
            break;
        }else if (isprint(input_char)){                         // check if it's a printable character and not junk
                input_word += static_cast<char>(input_char);
                counter++;
        }
        print_search_word(input_word);
        clear_search_results();

        vector<Object> id_results = current_database.find_object_by_id(input_word);
        vector<Object> name_results = current_database.find_object_by_name(input_word);
        vector<Object> type_results = current_database.find_object_by_type(input_word);
        vector<Object> quantity_results = current_database.find_object_by_quantity(input_word);

        found_objects.insert(found_objects.end(), id_results.begin(), id_results.end());
        found_objects.insert(found_objects.end(), name_results.begin(), name_results.end());
        found_objects.insert(found_objects.end(), type_results.begin(), type_results.end());
        found_objects.insert(found_objects.end(), quantity_results.begin(), quantity_results.end());

        sort(found_objects.begin(), found_objects.end());
        auto last_unique_element = unique(found_objects.begin(), found_objects.end());
        found_objects.erase(last_unique_element, found_objects.end());

        print_found_objects(found_objects);
        found_objects.clear();
    }                            
    noecho();
    cbreak();
}

void Menu::print_found_objects(const vector<Object>& found_objects){
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

void Menu::print_delete_object_menu(){
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

void Menu::print_delete_by_id(){
    string temp_string = "Delete by ID";
    move(MENU_TITTLE_LINE_POSITION + 2, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "ID of the item to delete: ";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
    refresh();
}

void Menu::print_delete_by_name(){
    string temp_string = "Delete by Name";
    move(MENU_TITTLE_LINE_POSITION + 2, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "Name of the item to delete: ";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
    refresh();
}

void Menu::print_delete_by_type(){
    string temp_string = "Delete by Type";
    move(MENU_TITTLE_LINE_POSITION + 2, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());

    temp_string = "Type of the item to delete: ";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
    refresh();
}

bool Menu::delete_object_menu(){
    print_delete_object_menu();
    
    int choice = getch(); // get user input
    if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
        return 1;
    if(sound)             // make ANNOYING sound if sound turned on 
        beep();

    if(choice == 10)      // if the user pressed Enter, go back
        return 0;

    switch(choice){
    case 49:{                                                                   // 1 -> delete objects by id
        print_delete_by_id();

        echo();                                                                 // echo on
        nocbreak();                                                             // buffering on
        char id_to_delete[10];
        getnstr(id_to_delete, sizeof(id_to_delete));                            // get user input
        if(check_resize()){                                                     // check if the window was resized by the user and re-enter the menu to re-draw everything
            noecho();                                                           // no echo
            cbreak();                                                           // no buffering
            return 1;
        }
        if(sound)                                                               // make ANNOYING sound if sound turned on   
            beep();
        noecho();                                                               // no echo
        cbreak();                                                               // no buffering

        if(id_to_delete[0] == '\0')                                             // if the user left blank and pressed Enter, go back
            return 0;

        bool is_confirmed = current_database.delete_object(stoi(id_to_delete)); // try to delete element and get confirmation
        return is_confirmed;                                                    // return the answser
    }
    case 50:{                                                                   // 2 -> delete objects by name
        print_delete_by_name();

        echo();                                                                 // echo on
        nocbreak();                                                             // buffering on
        char name_to_delete[255];
        getnstr(name_to_delete, sizeof(name_to_delete));                        // get user input
        if(check_resize()){                                                     // check if the window was resized by the user and re-enter the menu to re-draw everything
            noecho();                                                           // no echo
            cbreak();                                                           // no buffering
            return 1;
        }
        if(sound)                                                               // make ANNOYING sound if sound turned on   
            beep();          
        noecho();                                                               // no echo
        cbreak();                                                               // no buffering


        string string_to_find = "0.";
        string_to_find += name_to_delete;

        bool is_confirmed = current_database.delete_object(string_to_find);
        return is_confirmed;
    }
    case 51:{                                                                   // 3 -> delete objects by type
        print_delete_by_type();

        echo();                                                                 // echo on
        nocbreak();                                                             // buffering on
        char type_to_delete[255];
        getnstr(type_to_delete, sizeof(type_to_delete));                        // get user input
        if(check_resize()){                                                     // check if the window was resized by the user and re-enter the menu to re-draw everything
            noecho();                                                           // no echo
            cbreak();                                                           // no buffering
            return 1;
        }
        if(sound)                                                               // make ANNOYING sound if sound turned on   
            beep();
        noecho();                                                               // no echo
        cbreak();                                                               // no buffering

        string string_to_find = "1.";
        string_to_find += type_to_delete;

        bool is_confirmed = current_database.delete_object(string_to_find);     // try to delete and get confirmation
        if(is_confirmed)
            return 1;                                                           // return confirmation
    }}

    return 0;
}

void Menu::print_settings_menu(){
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

void Menu::print_decorator_settings(){
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

void Menu::print_text_position_settings(){
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

void Menu::print_sound_settings(){
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

void Menu::print_number_of_entries_settings(){
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

void Menu::print_continuous_mode_settings(){
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

int Menu::settings_menu(){
    print_settings_menu();
    while(true){
        int choice = getch(); // wait user input
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 7;
        if(sound)             // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
        case 49:{                              // Change decorator type
            print_decorator_settings();

            int decorator_choice = 0;
            while(decorator_choice  != 49 && decorator_choice != 50 && decorator_choice != 51 && decorator_choice != 52){ // until the user pressed a valid key, keep them trapped in an infinite loop
                decorator_choice = getch();                                                                               // wait user input
                if(check_resize())                                                                                        // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return 7;
                if(sound)                                                                                                 // make ANNOYING sound if sound turned on
                    beep();

                if(decorator_choice == 10)                                                                                // if the user pressed Enter, go back and free them
                    break;
            }
            switch (decorator_choice){                                                                                    // based on user choice, change the decorator type
            case 49:
                decorator_type = '*';
                break;
            case 50:
                decorator_type = '~';
                break;
            case 51:
                decorator_type = '+';
                break;
            case 52:
                decorator_type = '/';
                break;
            }
            return 7;
        }
        case 50:{                              // Change text alignment 
            print_text_position_settings();

            int text_choice = getch();
            if(check_resize())                                // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 7;
            if(sound)                                         // make ANNOYING sound if sound turned on
                beep();
            
            if(text_choice == 10)
                return 7;
            else
                while(text_choice  != 49 && text_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                    text_choice = getch();                      // wait user input
                    if(check_resize())                          // check if the window was resized by the user and re-enter the menu to re-draw everything
                        return 7;
                    if(sound)                                   // make ANNOYING sound if sound turned on
                        beep();

                    if(text_choice == 10)                       // if the user pressed Enter, go back and free them
                        return 7;                                  
                }

            // determine the next step based on user input
            if(text_choice == 50)      // if 1, align the text left side
                text_position = false;
            else                       // otherwise, center the text
                text_position = true;
            return 7;                  
        }
        case 51:{                              // Turn *annoying* Sound ON/OFF
            print_sound_settings();
            
            int sound_choice = getch();                       // get user input
            if(check_resize())                                // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 7;
            if(sound)                                         // make ANNOYING sound if sound turned on
                beep();

            if(sound_choice == 10)                            // if the user pressed Enter, go back
                return 7;
            else
                while(sound_choice  != 49 && sound_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                    sound_choice = getch();                       // wait user input
                    if(check_resize())                            // check if the window was resized by the user and re-enter the menu to re-draw everything
                        return 7;
                    if(sound)                                     // make ANNOYING sound if sound turned on
                        beep();

                    if(sound_choice == 10)                        // if the user pressed Enter, go back and free them
                        return 7;
                }

            // determine the next step based on user input
            if(sound_choice == 49) // if 1, turn on *annoying* sound
                sound = true;
            else                   // otherwise, turn it off *blessed silence*
                sound = false;
            return 7;
        }
        case 52:{                              // Change number of entries per page
            print_number_of_entries_settings();
            
            int entries_choice = getch(); // get user input
            if(check_resize())            // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 7;
            if(sound)                     // make ANNOYING sound if sound turned on 
                beep();

            if(entries_choice == 10)      // if the user pressed Enter, go back
                return 7;

            while(entries_choice  != 49 && entries_choice != 50 && entries_choice != 51){ // until the user pressed a valid key, keep them trapped in an infinite loop
                entries_choice = getch();                                                 // wait user input
                if(check_resize())                                                        // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return 7;
                if(sound)                                                                 // make ANNOYING sound if sound turned on
                    beep();

                if(entries_choice == 10)                                                  // if the user pressed Enter, go back and free them
                    return 7;
            }

            // determine the next step based on user input
            switch (entries_choice){
            case 49:                   // if 1, change number of entries to 3                 
                number_of_entries = 3;
                break;
            case 50:                   // if 2, change number of entries to 5
                number_of_entries = 5;
                break;
            case 51:                   // if 3, change number of entries to 7
                number_of_entries = 7;
            }
            return 7;
        }

        // TODO: ADD COMPACT MODE!
        // case 53:{
        //     move(LINES - 6, 3);
        //     for(int i = 0; i < 18; i++)
        //         printw(" ");
        //     move(LINES / 2 - 4, COLS / 2 - 6);
        //     printw("Compact mode:");
        //     move(LINES / 2 - 4, COLS - 38);
        //     printw("1. ON");
        //     move(LINES / 2 - 3, COLS - 38);
        //     printw("2. OFF");
            
        //     if(compact_mode){
        //         move(LINES / 2 - 4, COLS - 38 + 8);
        //         printw("(Current)");
        //     }else{
        //         move(LINES / 2 - 3, COLS - 38 + 7);
        //         printw("(Current)");
        //     }

        //     move(LINES / 2 - 5, COLS / 2 - 8);
        //     for(int i = 0; i < 17; i++)
        //         printw("*");

        //     move(LINES / 2 - 3, COLS / 2 - 8);
        //     for(int i = 0; i < 17; i++)
        //         printw("*");

        //     move(LINES / 2 - 5, COLS - 40);
        //     for(int i = 0; i < 20; i++)
        //         printw("*");
            
        //     move (LINES / 2 - 2, COLS - 40);
        //     for(int i = 0; i < 20; i++)
        //         printw("*");

        //     int compact_choice = getch();
        //     if(sound)
        //         beep();

        //     if(compact_choice == 10)
        //         break;

        //     while(compact_choice  != 49 && compact_choice != 50){
        //         compact_choice = getch();
        //         if(sound)
        //             beep();

        //         if(compact_choice == 10)
        //             break;
        //     }

        //     if(compact_choice == 49)
        //         compact_mode = true;
        //     else
        //         compact_mode = false;
        //     break;
        // }
        case 53:{                              // Turn Continuous Mode ON/OFF
            print_continuous_mode_settings();
            
            int continuous_choice = getch(); // get user input 
            if(check_resize())               // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 7;
            if(sound)                        // make ANNOYING sound if sound turned on   
                beep();

            if(continuous_choice == 10)      // if the user pressed Enter, go back
                return 7;

            while(continuous_choice  != 49 && continuous_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                continuous_choice = getch();                            // wait user input
                if(check_resize())                                      // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return 7;
                if(sound)                                               // make ANNOYING sound if sound turned on 
                    beep();

                if(continuous_choice == 10)                             // if the user pressed Enter, go back and free them
                    return 7;
            }

            // determine the next step based on user input
            if(continuous_choice == 49)  // if 1, turn continuous mode on
                continuous_mode = true;
            else                         // otherwise, tun it off
                continuous_mode = false; 
            return 7;
        }
        case 54:{            // Save settings (to file)
            save_settings();
            return 0;
        }
        case 10:{            // return to main menu
            return 0;
        }
        }
    }
}

void Menu::print_invalid_input(){
    string temp_string = "Invalid input! Press any key to continue...";
    if(text_position == 1)                                                             // check if text should be left side aligned or centered and write accordingly
        move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    else                                                       
        move(USER_INPUT_LINE, COLS / 2 - 35);
    printw("%s", temp_string.c_str());  // and warn him about it
}

void Menu::print_choose_database(){
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

void Menu::clear_user_input_zone(){
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    for(int i = 3; i < LAST_COLUMN_OF_TEXT - LINE_TEXT_START_POSITION; i++)
        printw(" ");
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
}

bool Menu::choose_database(){
    print_choose_database();
    echo();
    nocbreak();
    char database_choice[3];
    getnstr(database_choice, sizeof(database_choice) - 1);
    
    if(database_choice[0] == '\0'){
        noecho();
        cbreak();
        return false;
    }
    else{
        for(int i = 0; i < static_cast<int>(database_vector.size()); i++)
            if(stoi(database_choice) == database_vector[i].get_database_id()){
                current_database = database_vector[i];                         // make the selected database active
                break;
            }
        print_invalid_input();
    }
    noecho();
    cbreak();
    return true;
}

void Menu::print_find_menu(bool is_database){
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

void Menu::print_search_word(const string& search_word){
    move(USER_INPUT_LINE, 10);
    for (int i = 0; i < COLS - 11; i++) 
        printw(" ");
    move(USER_INPUT_LINE, 10);
    printw("%s", search_word.c_str());
}

void Menu::clear_search_results(){
    for (int i = 0; i < number_of_entries; i++){
        move(AVAILABLE_OPTIONS_START_LINE - i, LINE_TEXT_START_POSITION);
        for (int j = LINE_TEXT_START_POSITION; j < LAST_COLUMN_OF_TEXT - LINE_TEXT_START_POSITION; j++)
            printw(" ");
    }
    refresh();
}

void Menu::print_edit_database_menu(){
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

void Menu::print_edit_database_name(){
    clear_user_input_zone();
    string temp_string = "Set new database name: ";
    move(USER_INPUT_LINE, LINE_TEXT_START_POSITION);
    printw("%s", temp_string.c_str());
}

bool Menu::edit_database(){
    print_edit_database_menu();
    if(choose_database()){
        print_edit_database_name();
        echo();
        nocbreak();
        char new_database_name[256];
        getnstr(new_database_name, sizeof(new_database_name) - 1);
        noecho();
        cbreak();

        if(new_database_name[0] == '\0')
            return false;
        else{
            string file_path = "files/" + current_database.get_database_name() + ".txt";
            current_database.set_database_name(new_database_name);
            string new_file_path = "files/" + current_database.get_database_name() + ".txt";

            for(int i = 0; i < static_cast<int>(database_vector.size()); i++)
                if(database_vector[i].get_database_id() == current_database.get_database_id()){
                    if(filesystem::exists(file_path))
                        filesystem::rename(file_path, new_file_path);
                    else 
                        return false;
                    database_vector[i] = current_database;
                }
            return true;
        }
    }
    return false;

}

void Menu::find_database(){
    print_find_menu(true);
    string input_word = "";
    int counter = 0;

    while (true) {
        int input_char = getch();

        if (input_char == KEY_BACKSPACE){
            if (counter > 0) {
                input_word.pop_back();
                counter--;
            }
        }else if (input_char == 10 || input_char == KEY_ENTER){
            noecho();
            cbreak();
            break;
        }else if (isprint(input_char)){                         // check if it's a printable character and not junk
                input_word += static_cast<char>(input_char);
                counter++;
        }
        print_search_word(input_word);
        clear_search_results();

        vector<Database> found_databases = match_word(input_word);
        print_found_databases(found_databases);
        found_databases.clear();
    }                            
    noecho();
    cbreak();
}

void Menu::decrement_page(bool is_database){
    if(is_database){
        if(continuous_mode){                                                                                 // if Continuous Mode on
                if(page_number == 0)                                                                             // and if on page 0
                    page_number = (static_cast<int>(database_vector.size()) - 1) / number_of_entries;                  // go to last page
                else                                                                                             // but if not on page 0
                    page_number--;                                                                               // just decrement the page number
            }else if(page_number > 0)                                                                            // otherwise, if Continuous Mode off and not on the first page
                page_number --;                                                                                  // decrement the page number
    }else{    
        if(continuous_mode){                                                                                 // if Continuous Mode on
                if(page_number == 0)                                                                             // and if on page 0
                    page_number = current_database.get_number_of_objects() / number_of_entries;                  // go to last page
                else                                                                                             // but if not on page 0
                    page_number--;                                                                               // just decrement the page number
            }else if(page_number > 0)                                                                            // otherwise, if Continuous Mode off and not on the first page
                page_number --;                                                                                  // decrement the page number     
    }
}

void Menu::increment_page(bool is_database){
    if(is_database){
        if(continuous_mode){                                                                                     // if Continuous Mode on                       
            if(page_number == (static_cast<int>(database_vector.size()) - 1) / number_of_entries)                      // and if on the last page
                page_number = 0;                                                                                 // go to page 0
            else                                                                                                 // but if not on the last page
                page_number++;                                                                                   // just increment the page number
        }else if(page_number < (static_cast<int>(database_vector.size()) - 1) / number_of_entries)                     // oterwise, if Continuous Mode off and not on the last page
            page_number ++;                                                                                      // go to next page
    }else{
        if(continuous_mode){                                                                                     // if Continuous Mode on                       
            if(page_number == current_database.get_number_of_objects() / number_of_entries)                      // and if on the last page
                page_number = 0;                                                                                 // go to page 0
            else                                                                                                 // but if not on the last page
                page_number++;                                                                                   // just increment the page number
        }else if(page_number < current_database.get_number_of_objects() / number_of_entries)                     // oterwise, if Continuous Mode off and not on the last page
            page_number ++;                                                                                      // go to next page
    }
}

void Menu::update_index_manager(){
    ofstream index_manager("index_manager.txt");                                     // rewrite index_manager
    for(int j = 0; j < static_cast<int>(database_vector.size()); j++) 
        index_manager << database_vector[j].get_database_id() << '.' << database_vector[j].get_database_name() << '\n';
    index_manager.close();                                                           // close the file
}

void Menu::get_new_database_index(){
    current_database_index = 0;    

    // get the new current_database_index 
    for(int j = 0; j < static_cast<int>(database_vector.size()); j++)
        if(database_vector[j].get_database_id() == current_database_index)
            current_database_index++; 
        else if(database_vector[j].get_database_id() != current_database_index)
            break;

}

void Menu::save_settings(){
    ofstream save_settings("program_settings.txt");                        // open program_settings

    // write the settings as chosen by user
    save_settings << "Current_database_index=" << current_database_index << endl;
    switch (decorator_type){                                                      // determine which decorator type is being used
    case '*':
        save_settings << "Decorator_type=" << 1 << endl;
        break;
    case '~':
        save_settings << "Decorator_type=" << 2 << endl;
        break;
    case '+':
        save_settings << "Decorator_type=" << 3 << endl;
        break;
    case '/':
        save_settings << "Decorator_type=" << 4 << endl;
        break;
    }
    save_settings << "Text_position="          << text_position          << endl;
    save_settings << "Sound="                  << sound                  << endl;
    save_settings << "Number_of_entries="      << number_of_entries      << endl;
 // save_settings << "Compact_mode="           << compact_mode           << endl;
    save_settings << "Continuous_mode="        << continuous_mode        << endl;
    move(LINES - 2, 3);
    printw("Settings saved!");
    save_settings.close();                                                        // close the file
}

void Menu::reload_database_vector(){
    ifstream available_databases("index_manager.txt"); // open index_manager
    string line;
    database_vector.clear();                                  // clear the database_vector
    while(getline(available_databases, line)){                // so long as there are lines to read
        Database temp_database;                               // init a temp database object
        temp_database.load_database(line);                    // and load it using the line extracted from the file
        database_vector.emplace_back(temp_database);          // then add the database object to the database_vector
    }
    available_databases.close();                              // and finally close the file
    sort(database_vector.begin(), database_vector.end());
}

vector<Database> Menu::match_word(const string& input_word) {
    std::vector<Database> temp_vector;

    for (auto& database : database_vector) {
        string database_name = database.get_database_name();
        if (database_name.rfind(input_word, 0) == 0)
            temp_vector.push_back(database);
    }

    return temp_vector;
}

void Menu::print_found_databases(vector<Database> found_databases){
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