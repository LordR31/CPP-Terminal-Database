// Menu.cpp
#include "MenuLogic.hpp" // Include the corresponding header file

using namespace std;

MenuLogic::MenuLogic(){
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

// MAIN FUNCTIONS
int MenuLogic::main_menu(){
    menu_ui.print_main_menu(decorator_type, text_position);
    while(true){
        if(check_resize())     // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 0;

        int choice = getch();  // get user input
        if(sound)              // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
        case ASCII_1: //    Enter Manager Databases Menu
            return MANAGE_DATABASES_MENU_ID;
            break;
        case ASCII_2: //    Enter Settings Menu
            return SETTINGS_MENU_ID;
            break;
        case ASCII_3: //    Exit program!
            return EXIT_PROGRAM;
        default:
            break;
        }
    }
}

int MenuLogic::manage_databases_menu(){
    page_number = 0; // reset all_database_page_number
    menu_ui.print_manage_databases_menu(decorator_type, text_position);
    while(true){    
        if(check_resize())                             // check if the window was resized by the user and re-enter the menu to re-draw everything
            return MANAGE_DATABASES_MENU_ID;

        int choice = getch(); // get user input
        if(sound)             // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
        case ASCII_ENTER: //         Go back
            return 0;
        case ASCII_1: //             Create Database Menu
            return CREATE_DATABASE_MENU_ID;
            break;
        case ASCII_2: //             Load Database Menu
            return MANAGE_DATABASE_SUBMENU_ID;
            break;
        case ASCII_3: //             Delete Database Menu
            return DELETE_DATABASE_MENU_ID;
            break;
        case ASCII_4: //             Print all available databases
            return PRINT_AVAILABLE_DATABASES_MENU_ID;
            break;
        default:
            continue;
        }
    }
}

int MenuLogic::create_database_menu(){
    menu_ui.print_create_database_menu(decorator_type, text_position);

    echo();                                                                                                                            // enable echo 
    nocbreak();                                                                                                                        // enable buffering
    char database_name [256];
    getnstr(database_name, sizeof(database_name) - 1);                                                                                 // get user input
    if(sound)                                                                                                                          // make ANNOYING sound if sound turned on
        beep();
    noecho();                                                                                                                          // no echo
    cbreak();                                                                                                                          // no buffering

    if(check_resize())                                                                                                                 // check if the window was resized by the user and re-enter the menu to re-draw everything
        return CREATE_DATABASE_MENU_ID;

    if(database_name[0] == '\0')                                                                                                       // if the user pressed Enter, go back 
        return MANAGE_DATABASES_MENU_ID;

    string database_path = "files/" + (string)database_name + ".txt";                                                                  // otherwise, create the path to the database 

    if(filesystem::exists(database_path)){  
        menu_ui.print_database_already_exists(text_position);
        while(true){                                                                                                                   // and determine the next course of action
            int choice = getch();
            if(choice == ASCII_y || choice == ASCII_Y){      // lower and upper case y in ASCII                                                 // if they want to overwrite
                ofstream output;                                                          
                output.open(database_path);                                                                                            // overwrite file
                output << "Database_id=" << current_database_index << endl;                                                            // write the database index 
                output << "Next_object_id=0\n";                                                                                        // write the next object's id (0 for new databases)
                output.close();   
                reload_database_vector();                                                                                              // close the file
                menu_ui.print_database_created_successfully(text_position);
                getch();
                return MANAGE_DATABASES_MENU_ID;
            }else if(choice == ASCII_n || choice == ASCII_N) // lower and upper case n in ASCII                                                   
                return MANAGE_DATABASES_MENU_ID;                                                                                                              // otherwise, return to Manage Databases Menu
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

    menu_ui.print_database_created_successfully(text_position);
    getch();
    return MANAGE_DATABASES_MENU_ID;          // go back to the Database Manager Menu
}

int MenuLogic::load_database_menu(){
    bool is_paged = false;
    bool is_empty = database_vector.empty();
    bool is_in_find_interface = false;

    if(!is_empty)
        if(static_cast<int>(database_vector.size()) > number_of_entries) // if there IS something to display, check if you need pages
            is_paged = true;

    menu_ui.print_load_database(decorator_type, text_position, number_of_entries, is_empty, is_paged, page_number, database_vector);
    menu_ui.print_load_database_options(is_empty, is_paged, is_in_find_interface);
        
    if(is_empty){
        getch();
        return MANAGE_DATABASES_MENU_ID; 
    }

    while(true){
        int choice = getch(); // get user input
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return MANAGE_DATABASE_SUBMENU_ID;

        if(sound)             // make ANNOYING sound if sound turned on 
            beep();

        switch (choice){
            case ASCII_1:
                if(is_paged & ~is_in_find_interface)                                                                                                        // if the button is shown (there are pages)
                    decrement_page(true);
                return MANAGE_DATABASE_SUBMENU_ID;
            case ASCII_2:
                if(choose_database())
                    return PRINT_CURRENT_DATABASE_MENU_ID;
                else
                    return MANAGE_DATABASE_SUBMENU_ID;
            case ASCII_3:
                if(is_in_find_interface){
                    return MANAGE_DATABASE_SUBMENU_ID;
                }else{
                    find_database();
                    menu_ui.clear_user_input_zone();
                    is_in_find_interface = true;
                    menu_ui.print_load_database_options(is_empty, is_paged, is_in_find_interface);
                }
                continue;
            case ASCII_4:
                if(edit_database()){
                    update_index_manager();
                    reload_database_vector();
                }
                return MANAGE_DATABASE_SUBMENU_ID;
            case ASCII_5:
                if(is_paged & (!is_in_find_interface))                                                                                                     // if the button is shown (there are pages)            
                    increment_page(true);                                                                                 // otherwise just ignore this input
                return MANAGE_DATABASE_SUBMENU_ID; 
            case ASCII_ENTER:
                return MANAGE_DATABASES_MENU_ID;
            default:
                continue;
        }
    }
    return PRINT_CURRENT_DATABASE_MENU_ID;                                                                              // go to Print Current Database Menu
}

int MenuLogic::delete_database_menu(){
    bool is_paged = false;
    bool is_empty = database_vector.empty();
    bool is_in_find_interface = false;

    if (!is_empty){                          // check if there are any databases to print
        if(static_cast<int>(database_vector.size()) > number_of_entries)
            is_paged = true;
        }
    menu_ui.print_delete_database_menu(decorator_type, text_position, number_of_entries, is_empty, is_paged, page_number, database_vector);
    menu_ui.print_delete_database_menu_options(is_empty, is_paged, is_in_find_interface);
        
    if(is_empty){
        getch();
        return MANAGE_DATABASES_MENU_ID;
    }

    while(true){
        int choice = getch(); // get user input
        
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
           return MANAGE_DATABASE_SUBMENU_ID;

        if(sound)             // make ANNOYING sound if sound turned on 
           beep();

        switch (choice){
                case ASCII_1:
                    if(is_paged)
                       decrement_page(true);
                    return DELETE_DATABASE_MENU_ID;
                case ASCII_2:{
                    menu_ui.print_delete_database_choose_menu(text_position);
                    echo();
                    nocbreak();
                    char database_choice[3];
                    getnstr(database_choice, sizeof(database_choice) - 1);
                    noecho();
                    cbreak();
                    if(sound)                                                             // make ANNOYING sound if sound turned on
                        beep();

                    if(database_choice[0] == '\0')                                                // if the user pressed Enter, go back
                        return DELETE_DATABASE_MENU_ID;
                    else {
                        for(int i = 0; i < static_cast<int>(database_vector.size()); i++)
                            if(stoi(database_choice) == database_vector[i].get_database_id()){
                                current_database = database_vector[i];
                                break;
                            }else if(i == static_cast<int>(database_vector.size())){
                                menu_ui.print_invalid_input(text_position);
                                return DELETE_DATABASE_MENU_ID;
                            }
                    }
                    
                    menu_ui.print_delete_database_confirm_deletion_menu();
                    while(true){
                        menu_ui.clear_user_input_zone();
                        
                        //TODO: ADD RESIZE!!!
                        echo();
                        nocbreak();
                        char confirmation[7];                        
                        getnstr(confirmation, sizeof(confirmation) - 1); // get user input
                        if(check_resize()){                              // check if the window was resized by the user and re-enter the menu to re-draw everything
                            noecho();                                 // disable echo
                            cbreak();                                 // disable buffering
                            return DELETE_DATABASE_MENU_ID;
                        }
                        if(sound)                                    // make ANNOYING sound if sound turned on
                            beep(); 
                        noecho();                                    // no echo
                        cbreak();                                    // no buffering
                                
                        if(confirmation[0] == '\0')                  // if the user left blank and pressed Enter, go back
                            return MANAGE_DATABASES_MENU_ID;
                        else if(strcmp(confirmation, "DELETE") != 0){                                    // if the user unsuccessfully typed "DELETE"
                                menu_ui.print_invalid_input(text_position);
                                getch();
                                if(sound)
                                    beep();
                                return DELETE_DATABASE_MENU_ID;
                        }else{
                            bool is_confirmed = delete_database();                                                            
                            update_index_manager();
                            reload_database_vector();                                                        // reload the database_vector to reflect the changes
                            get_new_database_index();
                            menu_ui.print_delete_database_confirm_deletion_prompt(is_confirmed);
                            getch();
                            return MANAGE_DATABASES_MENU_ID;
                        }
                    }
                    return DELETE_DATABASE_MENU_ID;
                }
                case ASCII_3:
                    if(is_in_find_interface){
                        return DELETE_DATABASE_MENU_ID;
                    }else{
                        find_database();
                        menu_ui.clear_user_input_zone();
                        is_in_find_interface = true;
                        menu_ui.print_delete_database_menu_options(is_empty, is_paged, is_in_find_interface);
                    }
                    continue;
                case ASCII_4:
                    increment_page(true);                                                                                                  // otherwise just ignore this input
                    return DELETE_DATABASE_MENU_ID;
                case ASCII_ENTER:
                    return MANAGE_DATABASES_MENU_ID;
                 default:
                     continue;
        }
    }
    return MANAGE_DATABASES_MENU_ID;                                                                                // return to Manage Databases Menu
}

int MenuLogic::available_databases_menu(){
    bool is_empty = database_vector.empty();
    bool is_paged = false;

    if (!is_empty)                    // check if there are any databases to print
        if(static_cast<int>(database_vector.size()) > number_of_entries) // if there IS something to display, check if you need pages
            is_paged = true;
            
    menu_ui.print_available_databases_menu(decorator_type, text_position, number_of_entries, is_empty, is_paged, page_number, database_vector);
    menu_ui.print_available_databases_options(is_empty, is_paged);
    
    while(true){
        int input = getch();                    // wait user input
        
        if(check_resize())                      // check if the window was resized by the user and re-enter the menu to re-draw everything
            return PRINT_AVAILABLE_DATABASES_MENU_ID;
        if(sound)                               // make ANNOYING sound if sound turned on
            beep();
        switch (input){
        case ASCII_1:{
            if(is_paged)                                                                                                       // if the button is shown (there are pages)
                decrement_page(true);
            return PRINT_AVAILABLE_DATABASES_MENU_ID;
        }
        case ASCII_3:{
            if(is_paged)   
                increment_page(true);                                                                                                   // if the button is shown (there are pages)                                                                                                            // otherwise just ignore this input
            return PRINT_AVAILABLE_DATABASES_MENU_ID;
        }  
        case ASCII_ENTER:
            return MANAGE_DATABASES_MENU_ID;
        default:
            continue;
        }
    }
    return MANAGE_DATABASES_MENU_ID;                               // return to Manage Databases Menu
}

int MenuLogic::current_database_menu(){
    vector<Object> current_database_objects = current_database.get_database_objects();

    bool is_paged = false;
    bool is_empty = current_database_objects.empty();
    bool is_in_find_interface = false;

    if (!is_empty)                                                                // check if the database is empty
        if(current_database.get_number_of_objects() > number_of_entries) // and if there are objects to display, check if you need pages
            is_paged = true;                                                     

    menu_ui.print_current_database_menu(decorator_type, text_position, number_of_entries, is_empty, is_paged, page_number, current_database);
    menu_ui.print_current_database_menu_options(is_empty, is_paged, is_in_find_interface);

    // determine the next step based on user input
    while(true){
        int choice = getch();
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return PRINT_CURRENT_DATABASE_MENU_ID;
        if(sound)             // make ANNOYING sound if sound turned on
            beep();
        switch (choice){
            case ASCII_1:{                                                                                                               // 1 -> Previous page
                if(is_paged)                                                                                                        // if the button is shown (there are pages)
                    increment_page(false);                                                                                                // otherwise just ignore the input
                return PRINT_CURRENT_DATABASE_MENU_ID;
            }
            case ASCII_2:{                                                                                                                // 2 -> Add item to database
                int status = add_object_menu();                                                                                      // call the function to add an object to the database
                while(status == 0)
                    status = add_object_menu();
                current_database.save_database();
                reload_database_vector();
                return PRINT_CURRENT_DATABASE_MENU_ID;
            }
            case ASCII_3:{                                                                                                                // 3 -> Delete item from database
                int initial_size = current_database.get_number_of_objects();                                                         // get initial number of elements
                bool status = delete_object_menu();                                                                                   // try to delete element(s) and get confirmation
                
                if(status == 0)
                    return PRINT_CURRENT_DATABASE_MENU_ID;
                else if(status == 1){
                    int final_size = current_database.get_number_of_objects();
                    menu_ui.print_current_database_menu_delete_object_prompt(initial_size - final_size);
                    current_database.save_database();                                                                                    // save database changes
                    reload_database_vector();                                                                                            // and reload the vector
                }
                return PRINT_CURRENT_DATABASE_MENU_ID;
            }
            case ASCII_4:{     
                if(is_in_find_interface){
                    return PRINT_CURRENT_DATABASE_MENU_ID;
                }else{
                    find_object_menu();  
                    menu_ui.clear_user_input_zone();
                    is_in_find_interface = true;
                    menu_ui.print_current_database_menu_options(is_empty, is_paged, is_in_find_interface);  
                }
                continue;                                                                                                          // exit loop to return to Load Database Menu
            }
                                                                                                                                    // TODO: Add number of elements counter to screen
                                                                                                                                    // TODO: Add number of pages to screen:    Page X/XX
            case ASCII_5:{
                edit_object_menu();
                current_database.save_database();
                reload_database_vector();
                return PRINT_CURRENT_DATABASE_MENU_ID;
            }
            case ASCII_ENTER:
                page_number = 0;
                return MANAGE_DATABASE_SUBMENU_ID;
            case ASCII_7:{                                                                                                               // 5 -> Next page
                if(is_paged)                                                                                                       // if the button is shown (there are pages)            
                    decrement_page(false);
                return PRINT_CURRENT_DATABASE_MENU_ID;
            }
            default:                                                                                                               // ignore other inputs
                continue;
               
        }
    }

    current_database.save_database();  // save database changes
    reload_database_vector();          // reload the database vector
    return MANAGE_DATABASE_SUBMENU_ID; // return to load database menu
}

int MenuLogic::settings_menu(){
   menu_ui.print_settings_menu(decorator_type, text_position);
    while(true){
        int choice = getch(); // wait user input
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return SETTINGS_MENU_ID;
        if(sound)             // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
        case ASCII_1:{                              // Change decorator type
            menu_ui.print_decorator_settings(decorator_type);

            int decorator_choice = 0;
            while(decorator_choice  != ASCII_1 && decorator_choice != ASCII_2 && decorator_choice != ASCII_3 && decorator_choice != ASCII_4){ // until the user pressed a valid key, keep them trapped in an infinite loop
                decorator_choice = getch();                                                                               // wait user input
                if(check_resize())                                                                                        // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return SETTINGS_MENU_ID;
                if(sound)                                                                                                 // make ANNOYING sound if sound turned on
                    beep();

                if(decorator_choice == ASCII_ENTER)                                                                                // if the user pressed Enter, go back and free them
                    break;
            }
            switch (decorator_choice){                                                                                    // based on user choice, change the decorator type
            case ASCII_1:
                decorator_type = '*';
                break;
            case ASCII_2:
                decorator_type = '~';
                break;
            case ASCII_3:
                decorator_type = '+';
                break;
            case ASCII_4:
                decorator_type = '/';
                break;
            }
            return SETTINGS_MENU_ID;
        }
        case ASCII_2:{                              // Change text alignment 
            menu_ui.print_text_position_settings(text_position);

            int text_choice = getch();
            if(check_resize())                                // check if the window was resized by the user and re-enter the menu to re-draw everything
                return SETTINGS_MENU_ID;
            if(sound)                                         // make ANNOYING sound if sound turned on
                beep();
            
            if(text_choice == ASCII_ENTER)
                return SETTINGS_MENU_ID;
            else
                while(text_choice  != ASCII_1 && text_choice != ASCII_2){ // until the user pressed a valid key, keep them trapped in an infinite loop
                    text_choice = getch();                      // wait user input
                    if(check_resize())                          // check if the window was resized by the user and re-enter the menu to re-draw everything
                        return SETTINGS_MENU_ID;
                    if(sound)                                   // make ANNOYING sound if sound turned on
                        beep();

                    if(text_choice == ASCII_ENTER)                       // if the user pressed Enter, go back and free them
                        return SETTINGS_MENU_ID;                                  
                }

            // determine the next step based on user input
            if(text_choice == ASCII_2)      // if 1, align the text left side
                text_position = false;
            else                       // otherwise, center the text
                text_position = true;
            return SETTINGS_MENU_ID;                  
        }
        case ASCII_3:{                              // Turn *annoying* Sound ON/OFF
            menu_ui.print_sound_settings(sound);
            
            int sound_choice = getch();                       // get user input
            if(check_resize())                                // check if the window was resized by the user and re-enter the menu to re-draw everything
                return SETTINGS_MENU_ID;
            if(sound)                                         // make ANNOYING sound if sound turned on
                beep();

            if(sound_choice == ASCII_ENTER)                            // if the user pressed Enter, go back
                return SETTINGS_MENU_ID;
            else
                while(sound_choice  != ASCII_1 && sound_choice != ASCII_2){ // until the user pressed a valid key, keep them trapped in an infinite loop
                    sound_choice = getch();                       // wait user input
                    if(check_resize())                            // check if the window was resized by the user and re-enter the menu to re-draw everything
                        return SETTINGS_MENU_ID;
                    if(sound)                                     // make ANNOYING sound if sound turned on
                        beep();

                    if(sound_choice == ASCII_ENTER)                        // if the user pressed Enter, go back and free them
                        return SETTINGS_MENU_ID;
                }

            // determine the next step based on user input
            if(sound_choice == ASCII_1) // if 1, turn on *annoying* sound
                sound = true;
            else                   // otherwise, turn it off *blessed silence*
                sound = false;
            return SETTINGS_MENU_ID;
        }
        case ASCII_4:{                              // Change number of entries per page
            menu_ui.print_number_of_entries_settings(number_of_entries);
            
            int entries_choice = getch(); // get user input
            if(check_resize())            // check if the window was resized by the user and re-enter the menu to re-draw everything
                return SETTINGS_MENU_ID;
            if(sound)                     // make ANNOYING sound if sound turned on 
                beep();

            if(entries_choice == ASCII_ENTER)      // if the user pressed Enter, go back
                return SETTINGS_MENU_ID;

            while(entries_choice  != ASCII_1 && entries_choice != ASCII_2 && entries_choice != ASCII_3){ // until the user pressed a valid key, keep them trapped in an infinite loop
                entries_choice = getch();                                                 // wait user input
                if(check_resize())                                                        // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return SETTINGS_MENU_ID;
                if(sound)                                                                 // make ANNOYING sound if sound turned on
                    beep();

                if(entries_choice == ASCII_ENTER)                                                  // if the user pressed Enter, go back and free them
                    return SETTINGS_MENU_ID;
            }

            // determine the next step based on user input
            switch (entries_choice){
            case ASCII_1:                   // if 1, change number of entries to 3                 
                number_of_entries = 3;
                break;
            case ASCII_2:                   // if 2, change number of entries to 5
                number_of_entries = 5;
                break;
            case ASCII_3:                   // if 3, change number of entries to 7
                number_of_entries = 7;
            }
            return SETTINGS_MENU_ID;
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
        case ASCII_5:{                              // Turn Continuous Mode ON/OFF
            menu_ui.print_continuous_mode_settings(continuous_mode);
            
            int continuous_choice = getch(); // get user input 
            if(check_resize())               // check if the window was resized by the user and re-enter the menu to re-draw everything
                return SETTINGS_MENU_ID;
            if(sound)                        // make ANNOYING sound if sound turned on   
                beep();

            if(continuous_choice == ASCII_ENTER)      // if the user pressed Enter, go back
                return SETTINGS_MENU_ID;

            while(continuous_choice  != ASCII_1 && continuous_choice != ASCII_2){ // until the user pressed a valid key, keep them trapped in an infinite loop
                continuous_choice = getch();                            // wait user input
                if(check_resize())                                      // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return SETTINGS_MENU_ID;
                if(sound)                                               // make ANNOYING sound if sound turned on 
                    beep();

                if(continuous_choice == ASCII_ENTER)                             // if the user pressed Enter, go back and free them
                    return SETTINGS_MENU_ID;
            }

            // determine the next step based on user input
            if(continuous_choice == ASCII_1)  // if 1, turn continuous mode on
                continuous_mode = true;
            else                         // otherwise, tun it off
                continuous_mode = false; 
            return SETTINGS_MENU_ID;
        }
        case ASCII_6:{            // Save settings (to file)
            save_settings();
            return MAIN_MENU_ID;
        }
        case ASCII_ENTER:{            // return to main menu
            return MAIN_MENU_ID;
        }
        }
    }
}

// SUB-MENU FUNCTIONS

// Manage Databases Functions
bool MenuLogic::edit_database(){
    menu_ui.print_edit_database_menu(text_position);
    if(choose_database()){
        menu_ui.print_edit_database_name();
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

void MenuLogic::find_database(){
    menu_ui.print_find_menu(text_position, number_of_entries, true);
    string input_word = "";
    int counter = 0;

    while (true) {
        int input_char = getch();

        if (input_char == KEY_BACKSPACE){
            if (counter > 0) {
                input_word.pop_back();
                counter--;
            }
        }else if (input_char == ASCII_ENTER || input_char == KEY_ENTER){
            noecho();
            cbreak();
            break;
        }else if (isprint(input_char)){                         // check if it's a printable character and not junk
                input_word += static_cast<char>(input_char);
                counter++;
        }
        menu_ui.print_search_word(input_word);
        menu_ui.clear_search_results(number_of_entries);

        vector<Database> found_databases = match_word(input_word);
        menu_ui.print_found_databases(text_position, number_of_entries, page_number, found_databases);
        found_databases.clear();
    }                            
    noecho();
    cbreak();
}

bool MenuLogic::choose_database(){
    menu_ui.print_choose_database(text_position);
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
        menu_ui.print_invalid_input(text_position);
    }
    noecho();
    cbreak();
    return true;
}

// Delete Database Functions
bool MenuLogic::delete_database(){
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

// Current Database Functions
int MenuLogic::add_object_menu(){
    menu_ui.print_add_object_menu(decorator_type);
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
            menu_ui.print_add_object_result(result);
            break;
        }
        menu_ui.print_add_object_result(result);
        break;
    }
    return 1;
}

bool MenuLogic::delete_object_menu(){
    menu_ui.print_delete_object_menu();
    
    int choice = getch(); // get user input
    if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
        return 1;
    if(sound)             // make ANNOYING sound if sound turned on 
        beep();

    if(choice == ASCII_ENTER)      // if the user pressed Enter, go back
        return 0;

    switch(choice){
    case ASCII_1:{                                                                   // 1 -> delete objects by id
        menu_ui.print_delete_by_id();

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
    case ASCII_2:{                                                                   // 2 -> delete objects by name
        menu_ui.print_delete_by_name();

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
    case ASCII_3:{                                                                   // 3 -> delete objects by type
        menu_ui.print_delete_by_type();

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

void MenuLogic::edit_object_menu(){
    menu_ui.print_edit_object_menu();

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
    
    menu_ui.print_edit_object(decorator_type, stoi(object_id), current_database);
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

void MenuLogic::find_object_menu(){
    menu_ui.print_find_menu(text_position, number_of_entries, false);
    
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
        }else if (input_char == ASCII_ENTER || input_char == KEY_ENTER){
            noecho();
            cbreak();
            break;
        }else if (isprint(input_char)){                         // check if it's a printable character and not junk
                input_word += static_cast<char>(input_char);
                counter++;
        }
        menu_ui.print_search_word(input_word);
        menu_ui.clear_search_results(number_of_entries);

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

        menu_ui.print_found_objects(number_of_entries, page_number, found_objects);
        found_objects.clear();
    }                            
    noecho();
    cbreak();
}

// SPECIAL FUNCTIONS
void MenuLogic::decrement_page(bool is_database){
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

void MenuLogic::increment_page(bool is_database){
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

vector<Database> MenuLogic::match_word(const string& input_word) {
    std::vector<Database> temp_vector;

    for (auto& database : database_vector) {
        string database_name = database.get_database_name();
        if (database_name.rfind(input_word, 0) == 0)
            temp_vector.push_back(database);
    }

    return temp_vector;
}

void MenuLogic::update_index_manager(){
    ofstream index_manager("index_manager.txt");                                     // rewrite index_manager
    for(int j = 0; j < static_cast<int>(database_vector.size()); j++) 
        index_manager << database_vector[j].get_database_id() << '.' << database_vector[j].get_database_name() << '\n';
    index_manager.close();                                                           // close the file
}

void MenuLogic::get_new_database_index(){
    current_database_index = 0;    

    // get the new current_database_index 
    for(int j = 0; j < static_cast<int>(database_vector.size()); j++)
        if(database_vector[j].get_database_id() == current_database_index)
            current_database_index++; 
        else if(database_vector[j].get_database_id() != current_database_index)
            break;

}

void MenuLogic::save_settings(){
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
    save_settings.close();                                                        // close the file
}

void MenuLogic::reload_database_vector(){
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
