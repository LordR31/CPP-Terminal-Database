// Menu.cpp
#include "Menu.h" // Include the corresponding header file

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

int Menu::print_menu(){
    while(true){
        // create the outline box using the preferred decorator, write the menu name and program title
        clear();
        draw_main_box(decorator_type);
        move(1,3);
        printw("Main Menu");
        move(1,COLS - 28);
        printw("Terminal Database Manager");

        // write out the menu options
        if(text_position == 1)               // check if text should be left side aligned or centered and write accordingly
            move(LINES - 6,3);
        else
            move(LINES - 6, COLS / 2 - 9);
        printw("1. Manage Databases");
        if(text_position == 1)               // check if text should be left side aligned or centered and write accordingly
            move(LINES - 5,3);
        else
            move(LINES - 5, COLS / 2 - 5);
        printw("2. Settings");
        if(text_position == 1)               // check if text should be left side aligned or centered and write accordingly
            move(LINES - 4,3);
        else
            move(LINES - 4, COLS / 2 - 7);
        printw("3. Quit Program");
        draw_line(decorator_type);           // draw the bottom line
        refresh();
        if(check_resize())                   // check if the window was resized by the user and re-enter the menu to re-draw everything
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
            endwin();
            exit(0);
            break;
        default:
            break;
        }
    }
}

int Menu::manage_database(){
    all_database_page_number = 0; // reset all_database_page_number
    while(true){
        // create the outline box using the preferred decorator, write the menu name and program title
        clear(); 
        draw_main_box(decorator_type); 
        move(1,3);
        printw("Manage Databases");
        move(1, COLS - 28);
        printw("Terminal Database Manager");

        // write out the menu options
        if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
            move(LINES - 7, 3);
        else
            move(LINES - 7, COLS / 2 - 9);
        printw("1. Create database");
        if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
            move(LINES - 6, 3);
        else
            move(LINES - 6, COLS / 2 - 8);
        printw("2. Load database");
        if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
            move(LINES - 5, 3);
        else
            move(LINES - 5, COLS / 2 - 9);
        printw("3. Delete database");
        if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
            move(LINES - 4, 3);
        else
            move(LINES - 4, COLS / 2 - 14);
        printw("4. Print available databases");
        draw_line(decorator_type);                     // draw the bottom line
        if(text_position == 1)                         // check if text should be left side aligned or centered and write accordingly
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 17);
        printw("Press Enter to go back to main menu");
        refresh();
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
            printw("Invalid Choice!");  // Warn user they pressed an invalid key
            break;
        }
    }
}

int Menu::load_database(){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(1,3);
    printw("Load Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    draw_line(decorator_type);                                                                     // draw the bottom line
    // write out the menu options
    move(LINES / 2 - 4, COLS / 2 - 11);
    printw("Press Enter to go back.");
    refresh();

    bool is_paged = false;
    if (database_vector.empty()){                    // check if there are any databases to print
        move(8, COLS / 2 - 16);
        printw("There are no available databases."); // and tell the user there's nothing to display
        if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 14);
        printw("Press any key to continue...");
        getch();
        return 1;
    }else{
        if(static_cast<int>(database_vector.size()) > number_of_entries) // if there IS something to display, check if you need pages
            is_paged = true;

       if(text_position == 1)                                                                      // check if text should be left side aligned or centered and write accordingly
            move(LINES - 4 - number_of_entries, 3);
        else
            move(LINES - 4 - number_of_entries, COLS / 2 - 9);
        printw("Available Databases");

        for(int i = 0; i < number_of_entries; i++){
            if(i + all_database_page_number * number_of_entries < static_cast<int>(database_vector.size())){
                if(text_position == 1)                                                                     // check if text should be left side aligned or centered and write accordingly
                    move(LINES - 4 - i, 3);
                else
                    move(LINES - 4 - i, COLS / 2 - (database_vector[i + all_database_page_number * number_of_entries].get_database_name().length() / 2));
                printw("%d.%s", database_vector[i + all_database_page_number * number_of_entries].get_database_id(), database_vector[i + all_database_page_number * number_of_entries].get_database_name().c_str());
            }
        }

        while(true){
            if(is_paged){                                 // if there ARE pages, show the page buttons
                move(LINES - 2, 3);
                printw("1 - Previous Page");
                move(LINES - 2, 17 + 3 + 10 + 19 + 8 + 10 + 10 + 12);
                printw("5 - Next Page");
            }

            move(LINES - 2, 17 + 3 + 10);
            printw("2 - Choose Database");

            move(LINES - 2, 17 + 3 + 10 + 19 + 5);
            printw("3 - Find");

            move(LINES - 2, 17 + 3 + 10 + 19 + 8 + 5 + 5);
            printw("4 - Edit");   

            int choice = getch(); // get user input
            if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 3;

            if(sound)             // make ANNOYING sound if sound turned on 
                beep();

            switch (choice){
                    case 49:{
                        if(is_paged){                                                                                                        // if the button is shown (there are pages)
                            if(continuous_mode){                                                                                                // if Continuous Mode on
                                if(all_database_page_number == 0){                                                                             // if on page 0
                                    all_database_page_number = static_cast<int>(database_vector.size()) / number_of_entries;                      // go to last page
                                }else{all_database_page_number--;}                                                                             // otherwise just decrement the page number
                            }else if(all_database_page_number > 0){                                                                         // othetwise, if NOT on page 0
                                    all_database_page_number --;                                                                               // go to the previous page
                            }}else                                                                                                          // otherwise just ignore the input
                                return 3;
                        return 3;
                    }
                    case 50:{
                        while(true){
                            move(LINES - 2, 3);
                            for(int i = 3; i < COLS - 4; i++)
                                printw(" ");
                            if(text_position == 1)
                                move(LINES - 2, 3);
                            else
                                move(LINES - 2, COLS / 2 - 8);
                            printw("Choose database: ");
                            echo();
                            nocbreak();
                            char database_choice[3];
                            getnstr(database_choice, sizeof(database_choice) - 1);
                            noecho();
                            cbreak();

                            if(database_choice[0] == '\0')
                                break;

                            if((choice - '0') > static_cast<int>(database_vector.size())){
                                if(text_position == 1)                                                             // check if text should be left side aligned or centered and write accordingly
                                    move(LINES - 2, 3);
                                else                                                       
                                    move(LINES - 2, COLS / 2 - 35);
                                printw("Invalid choice! Try entering a valid index. Press Enter to continue...");  // and warn him about it
                                while(true)
                                    if(getch() != 10)
                                        continue;
                                continue;
                            }else{
                                for(int i = 0; i < static_cast<int>(database_vector.size()); i++)
                                    if(stoi(database_choice) == database_vector[i].get_database_id())
                                        current_database = database_vector[i];                                   // make the selected database active
                                return 6;
                            }
                        }
                        continue;
                    }
                    case 51:{
                        move(LINES - 2, 3);
                        for(int i = 3; i < COLS - 4; i++)
                            printw(" ");

                        if(text_position == 1)                                                                      // check if text should be left side aligned or centered and write accordingly
                            move(LINES - 4 - number_of_entries, 3);
                        else
                            move(LINES - 4 - number_of_entries, COLS / 2 - 9);
                        printw("Matching Databases");

                        move(LINES - 2, 3);
                        printw("Search:");

                        string input_word = "";
                        int counter = 0;

                        for (int i = 0; i < number_of_entries; i++){
                                move(LINES - 4 - i, 3);
                                for (int j = 0; j < COLS / 2; j++)
                                    printw(" ");
                        }

                        while (true) {
                            int input_char = getch();

                            if (input_char == KEY_BACKSPACE){
                                if (counter > 0) {
                                    input_word.pop_back();
                                    counter--;
                                }
                            }else if (input_char == 10 || input_char == KEY_ENTER){
                                break;
                            }else if (isprint(input_char)){                         // check if it's a printable character and not junk
                                    input_word += static_cast<char>(input_char);
                                    counter++;
                            }

                            move(LINES - 2, 10);
                            for (int i = 0; i < COLS - 11; i++) 
                                printw(" ");
                            move(LINES - 2, 10);
                            printw("%s", input_word.c_str());

                            for (int i = 0; i < number_of_entries; i++){
                                move(LINES - 4 - i, 3);
                                for (int j = 0; j < COLS / 2; j++)
                                    printw(" ");
                            }

                            refresh();

                            vector<Database> found_databases = match_word(input_word);
                            print_found_databases(found_databases);
                            found_databases.clear();
                        }                            
                        noecho();
                        cbreak();
                        continue;
                    }
                    case 10:
                        return 1;
                    case 53:{
                        if(is_paged){                                                                                                       // if the button is shown (there are pages)            
                            if(continuous_mode){                                                                                               // if Continuous Mode on                       
                                if(all_database_page_number == static_cast<int>(database_vector.size()) / number_of_entries){                 // if on the last page
                                    all_database_page_number = 0;                                                                                // go to page 0
                                }else{all_database_page_number++;}                                                                            // otherwise just increment the page number
                            }else if(all_database_page_number < static_cast<int>(database_vector.size()) / number_of_entries){             // oterwise, if NOT on the last page
                                all_database_page_number ++;                                                                                  // go to next page
                            }}else         
                                return 3;                                                                                                   // otherwise just ignore this input
                        return 3;
                    }  
                    default:
                        continue;
            }
        }
    }
    return 6;                                                                              // go to Print Current Database Menu
}

int Menu::create_database(){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(1,3);
    printw("Create Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    move(LINES / 2 - 4, COLS / 2 - 25);

    // write out the menu options
    printw("Leave blank and press Enter to cancel and go back!");
    draw_line(decorator_type);                                    // draw the bottom line
    if(text_position == 1)                                        // check if text should be left side aligned or centered and write accordingly
            move(LINES - 2,3);
        else
            move(LINES - 2, COLS / 2 - 7);
    printw("Database name: ");

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

    if(filesystem::exists(database_path)){                                                                                            // check if the database already exists
        move(LINES - 2, 3);
        printw("This database already exists! Do you want to overwrite? (Y / N) WARNING: This can lead to loss of important data!!!"); // and if it does, warn the user
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

    current_database_index = database_vector.size() + 1;                      // get the new current database index
    save_settings();                                                          // save the settings
    reload_database_vector();                                                 // reload the database vector to reflect the changes

    if(text_position == 1)                    // check if text should be left side aligned or centered and write accordingly
            move(LINES - 2,3);
        else
            move(LINES - 2, COLS / 2 - 15);
    printw("Database created successfully!"); // tell the user that the database was created successfully!
    return 1;                                 // go back to the Database Manager Menu
}

int Menu::delete_database(){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(1, 3);
    printw("Delete Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    draw_line(decorator_type);           // draw the bottom line
    refresh();

    // write out the menu options, infos and warnings
    move(LINES / 2 - 4, COLS / 2 - 5);
    printw("WARNING!!!");
    move(LINES / 2 - 3, COLS / 2 - 17);
    printw("Deleting a database is PERMANENT!!!");
    move(LINES / 2 - 1, COLS / 2 - 11);
    printw("Press Enter to go back");
    
    if(text_position == 1)                                                                          // check if text should be left side aligned or centered and write accordingly
        move(LINES - 4 - number_of_entries, 3);
    else
        move(LINES - 4 - number_of_entries, COLS / 2 - 9);
    printw("Available databases");

    bool is_paged = false;
    if (database_vector.empty()){                    // check if there are any databases to print
        move(8, COLS / 2 - 16);
        printw("There are no available databases."); // and tell the user there's nothing to display
        if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 14);
        printw("Press any key to continue...");
        getch();
        return 1;
    }else{
        if(static_cast<int>(database_vector.size()) > number_of_entries)
            is_paged = true;

        string page_number_string = "Page " + to_string(all_database_page_number); // print the page number
        move(LINES / 2 + 1, COLS / 2 - page_number_string.length() / 2);
        printw("%s", page_number_string.c_str());

        for(int i = 0; i < number_of_entries; i++){                                                                                                                                          
            if(i + all_database_page_number * number_of_entries < static_cast<int>(database_vector.size())){                                                                        // starting with the LAST database in the vector
                if(text_position == 1)                                                                      // check if text should be left side aligned or centered and write accordingly
                    move(LINES - 4 - i, 3);
                else
                    move(LINES - 4 - i, COLS / 2 - (database_vector[i + all_database_page_number * number_of_entries].get_database_name().length() / 2));    
                printw("%d.%s", database_vector[i + all_database_page_number * number_of_entries].get_database_id(), database_vector[i + all_database_page_number * number_of_entries].get_database_name().c_str());                         // print ID.Name
            }                                                                                               // TODO: THIS NEEDS TO HAVE PAGES!!!
        }

        while(true){
            if(is_paged){                                 // if there ARE pages, show the page buttons
                move(LINES - 2, 3);
                printw("1 - Previous Page");
                move(LINES - 2, 17 + 3 + 10 + 19 + 8 + 10 + 10 + 12);
                printw("3 - Next Page");
            }

            move(LINES - 2, 17 + 3 + 12);
            printw("2 - Choose Database");

            move(LINES - 2, 17 + 3 + 10 + 19 + 18);
            printw("3 - Find");

            int choice = getch(); // get user input
            
            if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 3;

            if(sound)             // make ANNOYING sound if sound turned on 
                beep();

            switch (choice){
                    case 49:{
                        if(is_paged){                                                                                                        // if the button is shown (there are pages)
                            if(continuous_mode){                                                                                                // if Continuous Mode on
                                if(all_database_page_number == 0){                                                                             // if on page 0
                                    all_database_page_number = static_cast<int>(database_vector.size()) / number_of_entries;                      // go to last page
                                }else{all_database_page_number--;}                                                                             // otherwise just decrement the page number
                            }else if(all_database_page_number > 0){                                                                         // othetwise, if NOT on page 0
                                    all_database_page_number --;                                                                               // go to the previous page
                            }}else                                                                                                          // otherwise just ignore the input
                                return 4;
                        return 4;
                    }
                    case 50:{
                        while(true){
                            move(LINES - 2, 3);
                            for(int i = 3; i < COLS - 4; i++)
                                printw(" ");
                            if(text_position == 1)
                                move(LINES - 2, 3);
                            else
                                move(LINES - 2, COLS / 2 - 8);
                            printw("Choose database to delete: ");
                            echo();
                            nocbreak();
                            char database_choice[3];
                            getnstr(database_choice, sizeof(database_choice) - 1);
                            noecho();
                            cbreak();

                            if(database_choice[0] == '\0')                                                // if the user pressed Enter, go back
                                break;

                            if(stoi(database_choice) > static_cast<int>(database_vector.size())){     // if the user chose a number, check if it was NOT a valid database id
                                if(text_position == 1)                                                // check if text should be left side aligned or centered and write accordingly
                                    move(LINES - 2, 3);
                                else
                                    move(LINES - 2, COLS / 2 - 29);
                                printw("Invalid choice! Try entering a valid index. Select Index: "); // and warn him about it
                                echo();                                                               // enable echo 
                                nocbreak();                                                           // enable buffering
                                char choice_char[2];                                                  
                                getnstr(choice_char, sizeof(choice_char));                            // get user input again
                                if(check_resize()){                                                   // check if the window was resized by the user and re-enter the menu to re-draw everything
                                    noecho();                                                         // disable echo
                                    cbreak();                                                         // disable buffering
                                    return 4;
                                }
                                noecho();                                                             // disable echo
                                cbreak();                                                             // disable buffering
                                if(sound)                                                             // make ANNOYING sound if sound turned on
                                    beep();

                                if(choice > static_cast<int>(database_vector.size())){                // if the user pressed a number key, check if it was NOT a valid index AGAIN
                                    if(text_position == 1)                                            // check if text should be left side aligned or centered and write accordingly
                                        move(LINES - 2, 3);
                                    else
                                        move(LINES - 2, COLS / 2 - 20);
                                    printw("There have been too many invalid choices! Press any key to continue and go back..."); // tell him he/she had his/her chance and go back, because deleting a database is serious bussiness
                                    for(int j = 85; j < COLS; j++)
                                        printw(" ");
                                    getch();
                                    return 1;
                                }
                            }else{
                                // write the new info message to teach the user how to confirm deleting the database
                                move(LINES / 2 - 1, COLS / 2 - 22);
                                printw("Type DELETE to confirm deleting the database!");
                                move(LINES / 2, COLS / 2 - 24);
                                printw("Leave blank and press Enter to cancel and go back");
                                while(true){
                                    move(LINES - 2, 3);
                                    for(int i = 0; i < COLS - 4; i++)
                                        printw(" ");
                                    move(LINES - 2, 3);

                                    //TODO: ADD RESIZE!!!

                                    echo();                                      // echo on
                                    nocbreak();                                  // buffering on
                                    char confirmation[7];                        
                                    getnstr(confirmation, sizeof(confirmation) - 1); // get user input
                                    if(check_resize()){                          // check if the window was resized by the user and re-enter the menu to re-draw everything
                                            noecho();                            // disable echo
                                            cbreak();                            // disable buffering
                                            return 4;
                                    }
                                    if(sound)                                    // make ANNOYING sound if sound turned on
                                        beep(); 
                                    noecho();                                    // no echo
                                    cbreak();                                    // no buffering

                                    if(confirmation[0] == '\0')                  // if the user left blank and pressed Enter, go back
                                        return 0;

                                    int status = 1;
                                    while(status == 1)
                                        if(strcmp(confirmation, "DELETE") != 0){                                    // if the user unsuccessfully typed "DELETE"
                                            move(LINES - 2, 3);
                                            printw("Invalid Input! Do you want to try again? (Y / N): ");           // warn them and ask if they want to try again
                                            int input = getch();
                                            if(check_resize())                                                      // check if the window was resized by the user and re-enter the menu to re-draw everything
                                                return 4;
                                            if(sound)
                                                beep();

                                            if(confirmation[0] == '\0')
                                                status = 0;

                                            if(input == 121 || input == 89)       // lower and upper case y in ASCII
                                                break;                            // break this loop and try again

                                            else if(input == 110 || input == 78){ // lower and upper case n in ASCII
                                                status = 0;
                                            }
                                        }else
                                            status = 2;

                                    if(status == 2){
                                        bool is_confirmed = false;
                                        int i = 0;
                                        for(i; i < static_cast<int>(database_vector.size()); i++)
                                            if(database_vector[i].get_database_id() == stoi(database_choice)){
                                                is_confirmed = database_vector[i].delete_database();                         // if the user DID successfully type "DELETE", delete the database file, BUT get confirmation
                                                break;
                                            }

                                        if(is_confirmed){
                                            database_vector.erase(database_vector.begin() + i);                      // remove the database from the vector

                                            current_database_index = 0;                                         // get the new current_database_index 
                                            for(int j = 0; j < static_cast<int>(database_vector.size()); j++)
                                                if(database_vector[j].get_database_id() == current_database_index)
                                                    current_database_index++;                                                             

                                            ofstream index_manager("index_manager.txt");                                     // rewrite index_manager
                                            for(int j = 0; j < static_cast<int>(database_vector.size()); j++) 
                                                index_manager << j << '.' << database_vector[j].get_database_name() << '\n';
                                            index_manager.close();                                                           // close the file
                                            reload_database_vector();                                                        // reload the database_vector to reflect the changes
                                            move(LINES - 2, 3);
                                            printw("Database deleted! Press any key to continue...");
                                            getch();
                                            return 1;
                                        }
                                    }else{
                                        printw("Operation canceled! Press any key to continue...");                      // tell the user there was an error and the database was NOT deleted
                                        getch();
                                        return 1;
                                    }
                                }
                            }
                        }
                        continue;
                    }
                    case 10:
                        return 1;
                    case 51:{
                        if(is_paged){                                                                                                       // if the button is shown (there are pages)            
                            if(continuous_mode){                                                                                               // if Continuous Mode on                       
                                if(all_database_page_number == static_cast<int>(database_vector.size()) / number_of_entries){                 // if on the last page
                                    all_database_page_number = 0;                                                                                // go to page 0
                                }else{all_database_page_number++;}                                                                            // otherwise just increment the page number
                            }else if(all_database_page_number < static_cast<int>(database_vector.size()) / number_of_entries){             // oterwise, if NOT on the last page
                                all_database_page_number ++;                                                                                  // go to next page
                            }}else         
                                return 4;                                                                                                   // otherwise just ignore this input
                        return 4;
                    }  
                    default:
                        continue;
            }
        }
    }
    return 1;                                                                                // return to Manage Databases Menu
}

int Menu::print_databases(){
    // create the outline box using the preferred decorator, write the menu name and program title
    clear();
    draw_main_box(decorator_type);
    move(1, 3);
    printw("Available Databases");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    draw_line(decorator_type);           // draw the bottom line
    refresh();

    bool is_paged = false;
    if (database_vector.empty()){                    // check if there are any databases to print
        move(8, COLS / 2 - 16);
        printw("There are no available databases."); // and tell the user there's nothing to display
        if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 14);
        printw("Press any key to continue...");
        getch();
        return 1;
    }else{
        if(static_cast<int>(database_vector.size()) > number_of_entries) // if there IS something to display, check if you need pages
            is_paged = true;
                                                              // and write out the structure

        if(text_position == 1)                                // check if text should be left side aligned or centered and write accordingly
            move(LINES - 4 - number_of_entries, 3);
        else
            move(LINES - 4 - number_of_entries, COLS / 2 - 3 - 11 - 5);
        printw("Database ID");

        if(text_position == 1)                                // check if text should be left side aligned or centered and write accordingly
            move(LINES - 4 - number_of_entries, 20);
        else
            move(LINES - 4 - number_of_entries, COLS / 2 + 6 + 5);
        printw("Database Name");

        string page_number_string = "Page " + to_string(all_database_page_number); // print the page number
        move(LINES / 2, COLS / 2 - page_number_string.length() / 2);
        printw("%s", page_number_string.c_str());

        for(int i = 0; i < number_of_entries; i++){                                                                                                                                          
            if(i + all_database_page_number * number_of_entries < static_cast<int>(database_vector.size())){
                if(text_position == 1){ // check if text should be left side aligned or centered and write accordingly
                    move(LINES - 4 - i, 3);    
                    printw("%d", database_vector[i + all_database_page_number * number_of_entries].get_database_id());
                    move(LINES - 4 - i, 20);
                    printw("%s", database_vector[i + all_database_page_number * number_of_entries].get_database_name().c_str());                                 // ID         Name
                }else{
                    move(LINES - 4 - i, COLS / 2 - 3 - 5 - to_string(i).length() - 5);
                    printw("%d", database_vector[i + all_database_page_number * number_of_entries].get_database_id());
                    move(LINES - 4 - i, COLS / 2 + 13 - database_vector[i + all_database_page_number * number_of_entries].get_database_name().length() / 2 + 5);
                    printw("%s", database_vector[i].get_database_name().c_str());                                 // ID         Name
                }
            }
        }
    
    while(true){
        if(is_paged){                                 // if there ARE pages, show the page buttons
            move(LINES - 2, 2);
            printw("1 - Previous Page");
            move(LINES - 2, COLS - 15);
            printw("3 - Next Page");
        }
        move(LINES - 2, COLS / 2 - 6);
        printw("Enter - Back");

        int input = getch();                    // wait user input
        if(check_resize())                      // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 5;
        if(sound)                               // make ANNOYING sound if sound turned on
            beep();
        switch (input){
        case 49:{
            if(is_paged){                                                                                                        // if the button is shown (there are pages)
                if(continuous_mode){                                                                                                // if Continuous Mode on
                    if(all_database_page_number == 0){                                                                             // if on page 0
                        all_database_page_number = static_cast<int>(database_vector.size()) / number_of_entries;                      // go to last page
                    }else{all_database_page_number--;}                                                                             // otherwise just decrement the page number
                }else if(all_database_page_number > 0){                                                                         // othetwise, if NOT on page 0
                        all_database_page_number --;                                                                               // go to the previous page
                }}else                                                                                                          // otherwise just ignore the input
                    return 5;
            return 5;
        }
            
        case 10:
            return 1;
        case 51:{
            if(is_paged){                                                                                                       // if the button is shown (there are pages)            
                if(continuous_mode){                                                                                               // if Continuous Mode on                       
                    if(all_database_page_number == static_cast<int>(database_vector.size()) / number_of_entries){                 // if on the last page
                        all_database_page_number = 0;                                                                                // go to page 0
                    }else{all_database_page_number++;}                                                                            // otherwise just increment the page number
                }else if(all_database_page_number < static_cast<int>(database_vector.size()) / number_of_entries){             // oterwise, if NOT on the last page
                    all_database_page_number ++;                                                                                  // go to next page
                }}else         
                    return 5;                                                                                                   // otherwise just ignore this input
            return 5;
        }  
        default:
            continue;
        }
        }
    }
    return 1;                               // return to Manage Databases Menu
}

int Menu::print_current_database(){
    while(true){
        // create the outline box using the preferred decorator, write the menu name and program title
        clear();
        draw_main_box(decorator_type);
        move(1, 3);
        printw("%s", current_database.get_database_name().c_str());
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        draw_line(decorator_type);
        refresh();

        vector<Object> current_database_objects = current_database.get_database_objects();
        
        bool is_empty = false;
        bool is_paged = false;
        if (current_database_objects.empty()){                                        // check if the database is empty
            move(8, COLS / 2 - 25);
            printw("Database is empty. No objects to display.");                      // and tell the user there's nothing to display
            is_empty = true;
        }else{
            if(static_cast<int>(current_database_objects.size()) > number_of_entries) // if there IS something to display, check if you need pages
                is_paged = true;

                                                                              // and write out the database structure
            move(LINES - 4 - number_of_entries - 1, 3);
            printw("ID");                                                     // ID
            move(LINES - 4 - number_of_entries - 1, 8);
            printw("Name");                                                   //     Name
            move(LINES - 4 - number_of_entries - 1, COLS - 10 - 50);
            printw("Type");                                                   //                                   Type
            move(LINES - 4 - number_of_entries - 1, COLS - 10);
            printw("Quantity");                                               //                                                               Quantity

            string page_number_string = "Page " + to_string(current_database_page_number); // Print the page number
            move(LINES / 2, COLS / 2 - page_number_string.length() / 2);
            printw("%s", page_number_string.c_str());

            for(int i = 0; i < number_of_entries; i++){                                    // and then print the objects, as many on a page as selected by the user (number_of_entries)
                if(i + current_database_page_number * number_of_entries < static_cast<int>(current_database_objects.size())){
                    move(LINES - 4 - i, 3);
                    printw("%d",current_database_objects[i + current_database_page_number * number_of_entries].get_id());
                    move(LINES - 4 - i, 8);
                    printw("%s",current_database_objects[i + current_database_page_number * number_of_entries].get_name().c_str());
                    move(LINES - 4 - i, COLS - 10 - 50);
                    printw("%s", current_database_objects[i + current_database_page_number * number_of_entries].get_type().c_str());
                    move(LINES - 4 - i, COLS - 5 - to_string(current_database_objects[i + current_database_page_number * number_of_entries].get_quantity()).length() / 2 - 1);
                    printw("%d", current_database_objects[i + current_database_page_number * number_of_entries].get_quantity());
                    refresh();
                }
            }
        }

        // write out the menu options
        if(is_paged){                                 // if there ARE pages, show the page buttons
            move(LINES - 2, 2);
            printw("1 - Previous Page");
            move(LINES - 2, COLS - 15);
            printw("5 - Next Page");
        }
        if(!is_empty){                                // if it's NOT empty, show the delete button
            move(LINES - 2, COLS - 15 - 14 - 8 - 15);
            printw("3 - Delete item");
        }

        move(LINES - 2, COLS - 15 - 21 - 8 - 15 - 15);
        printw("2 - Add item");                      
        
        move(LINES - 2, COLS - 15 - 7 - 8);
        printw("4 - Back");
        
        int choice = getch(); // get user input
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 6;
        if(sound)             // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
            case 49: {                                                                                                               // 1 -> Previous page
                if(is_paged){                                                                                                        // if the button is shown (there are pages)
                    if(continuous_mode){                                                                                                // if Continuous Mode on
                        if(current_database_page_number == 0){                                                                             // if on page 0
                            current_database_page_number = current_database.get_number_of_objects() / number_of_entries;                      // go to last page
                        }else{current_database_page_number--;}                                                                             // otherwise just decrement the page number
                    }else if(current_database_page_number > 0){                                                                         // othetwise, if NOT on page 0
                            current_database_page_number --;                                                                               // go to the previous page
                    }}else                                                                                                          // otherwise just ignore the input
                        return 6;
                break;
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
                int status = delete_object_menu();                                                                                   // try to delete element(s) and get confirmation
                if(status == 1)
                    return 6;
                if(status == 2){                                                                                                     // if confirmed
                    int final_size = current_database.get_number_of_objects();                                                       // get final number of elements
                    move(LINES - 2, 3);
                    printw("%d element(s) were deleted from the database! Press any key to continue...", initial_size - final_size); // and print the number of deleted elements
                    getch();                                                                                                         // wait user input before moving on
                }else if(status == 0){
                    move(LINES - 2, 3);
                    printw("No element was deleted from the database! Press any key to continue...");                        // otherwise, inform the user about the problem
                    getch();                                                                                                         // and wait for user input
                }
                current_database.save_database();                                                                                    // save database changes
                reload_database_vector();                                                                                            // and reload the vector
                return 6;
            }
            case 52:{                                                                                                                // 4 -> Back
                return 3;                                                                                                            // exit loop to return to Load Database Menu
            }

                                                                                                                                    // TODO: Add Find & Edit button
                                                                                                                                    // TODO: Add number of elements counter to screen
                                                                                                                                    // TODO: Add number of pages to screen:    Page X/XX

            case 53:{                                                                                                               // 5 -> Next page
                if(is_paged){                                                                                                       // if the button is shown (there are pages)            
                    if(continuous_mode){                                                                                               // if Continuous Mode on                       
                        if(current_database_page_number == current_database.get_number_of_objects() / number_of_entries){                 // if on the last page
                            current_database_page_number = 0;                                                                                // go to page 0
                        }else{current_database_page_number++;}                                                                            // otherwise just increment the page number
                    }else if(current_database_page_number < current_database.get_number_of_objects() / number_of_entries){             // oterwise, if NOT on the last page
                        current_database_page_number ++;                                                                                  // go to next page
                    }}else         
                        return 6;                                                                                                   // otherwise just ignore this input
                break;
            }
            default:{                                                                                                               // ignore other inputs
                return 6;
            }    
        }
    }

    current_database.save_database(); // save database changes
    reload_database_vector();         // reload the database vector
    return 3;                         // return to load database menu
}

int Menu::add_object_menu(){
    while(true){
        // create the outline box using the preferred decorator, write the menu name and program title
        clear();
        draw_main_box(decorator_type);
        move(1,3);
        printw("Add Object");
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        move(LINES / 2 - 4, COLS / 2 - 23);

        // inform the user as to how to create new objects
        printw( "Objects are created using the following format:");
        move(LINES / 2 - 5, COLS - 30);
        printw( "**********************");
        move(LINES / 2 - 4, COLS - 28);
        printw( "name type quantity");
        move(LINES / 2 - 3, COLS - 30);
        printw( "**********************");
        move(LINES - 4, 3);
        printw("Leave blank and press Enter to cancel and go back");
        draw_line(decorator_type);                                        // draw the bottom line
        move(LINES - 2, 3);

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
 
        try{                                                                          // then try to create an object with them
            string temp_name = temp_object[0];
            string temp_type = temp_object[1];
            int temp_quantity = std::stoi(temp_object[2]);

            current_database.add_object(temp_name, temp_type, temp_quantity);         // and add it to the database object vector

            printw("Object added to the database!");                                  // finally tell the user that the object was added to the database (never really shown cuz no getch() to pause)
            break;
        }catch(int errorCode){                                                        // if not possible, tell the user to carefully follow the format
            printw("Invalid object! Please make sure to follow the correct format!"); 
        }
    }
    return 1;
}

int Menu::delete_object_menu(){
    // print on top of the database menu
    move(3,3);
    printw("Delete by...");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    move(5, 3);
    printw("1. ID");
    move(6, 3);
    printw("2. Name");
    move(7, 3);
    printw("3. Type");
    move(10, 3);
    printw("WARNING!!!");
    move(11, 3);
    printw("All matching objects will be deleted!!!");
    move(13, 3);        
    printw("Press Enter to cancel and go back.");
    refresh();
    
    move(LINES - 2, 2);
    for(int i = 2; i < COLS - 1; i++)
        printw(" ");
    
    int choice = getch(); // get user input
    if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
        return 1;
    if(sound)             // make ANNOYING sound if sound turned on 
        beep();

    if(choice == 10)      // if the user pressed Enter, go back
        return 0;

    switch(choice){
    case 49:{                                                                   // 1 -> delete objects by id
        move(3,3);
        printw("Delete by ID");
        move(LINES - 2, 3);
        printw("ID of the item to delete: ");
        refresh();

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
        move(1,3);
        printw("Delete by Name");
        move(LINES - 2, 3);
        printw("Name of the item to delete: ");

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
        move(1,3);
        printw("Delete by Type");
        move(LINES - 2, 3);
        printw("Type of the item to delete: ");

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
            return 2;                                                           // return confirmation
    }}

    return 0;
}

int Menu::settings(){
    while(true){
        // create the outline box using the preferred decorator, write the menu name and program title
        clear();
        draw_main_box(decorator_type);
        move(1,3);
        printw("Settings");
        move(1, COLS - 28);
        printw("Terminal Database Manager");

        // write out the menu options, infos and warnings
        if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
            move(LINES - 9, 3);
        else
            move(LINES - 9, COLS / 2 - 8);
        printw("1. Decorator type");
        if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
            move(LINES - 8, 3);
        else
            move(LINES - 8, COLS / 2 - 8);
        printw("2. Text position");
        if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
            move(LINES - 7, 3);
        else
            move(LINES - 7, COLS / 2 - 4);
        printw("3. Sound");
        if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
            move(LINES - 6, 3);
        else
            move(LINES - 6, COLS / 2 - 10);
        printw("4. Number of entries");
        if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
            move(LINES - 5, 3);
        else
            move(LINES - 5, COLS / 2 - 9);
        printw("5. Continuous mode");
        if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
            move(LINES - 4, 3);
        else
            move(LINES - 4, COLS / 2 - 8);
        printw("6. Save settings");

        draw_line(decorator_type);                      // draw the bottom line
        if(text_position == 1)                          // check if text should be left side aligned or centered and write accordingly
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 18);
        printw("Press Enter to go back to main menu.");
        refresh();
        if(check_resize())                              // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 7;

        int choice = getch(); // wait user input
        if(check_resize())    // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 7;
        if(sound)             // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
        case 49:{                              // Change decorator type
            move(LINES - 9, 3);
            for(int i = 0; i < 17; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 8);
            printw("Decorator Type:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. *");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. ~");
            move(LINES / 2 - 2, COLS - 38);
            printw("3. +");
            move(LINES / 2 - 1, COLS - 38);
            printw("4. /");

            switch (decorator_type){                // show which is the currently selected option
            case '*':
                move(LINES / 2 - 4, COLS - 38 + 6);
                printw("(Current)");
                break;
            case '~':
                move(LINES / 2 - 3, COLS - 38 + 6);
                printw("(Current)");
                break;
            case '+':
                move(LINES / 2 - 2, COLS - 38 + 6);
                printw("(Current)");
                break;
            case '/':
                move(LINES / 2 - 1, COLS - 38 + 6);
                printw("(Current)");
                break;
            }

            move(LINES / 2 - 5, COLS / 2 - 10);
            for(int i = 0; i < 19; i++)
                printw("*");

            move(LINES / 2 - 3, COLS / 2 - 10);
            for(int i = 0; i < 19; i++)
                printw("*");

            move(LINES / 2 - 5, COLS - 40);
            for(int i = 0; i < 18; i++)
                printw("*");
            
            move (LINES / 2, COLS - 40);
            for(int i = 0; i < 18; i++)
                printw("*");
            

            int decorator_choice = getch(); // get user input
            if(check_resize())              // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 7;
            if(sound)                       // make ANNOYING sound if sound turned on
                beep();

            if(decorator_choice == 10)      // if the user pressed Enter, go back
                break;

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
            break;
        }
        case 50:{                              // Change text alignment 
            move(LINES - 8, 3);
            for(int i = 0; i < 16; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 7);
            printw("Text position:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. Left side");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. Centered");
            
            if(text_position){                       // show which is the currently selected option
                move(LINES / 2 - 4, COLS - 38 + 14);                
                printw("(Current)");
            }else{
                move(LINES / 2 - 3, COLS - 38 + 13);
                printw("(Current)");
            }
            
            move(LINES / 2 - 5, COLS / 2 - 9);
            for(int i = 0; i < 18; i++)
                printw("*");

            move(LINES / 2 - 3, COLS / 2 - 9);
            for(int i = 0; i < 18; i++)
                printw("*");

            move(LINES / 2 - 5, COLS - 40);
            for(int i = 0; i < 26; i++)
                printw("*");
            
            move (LINES / 2 - 2, COLS - 40);
            for(int i = 0; i < 26; i++)
                printw("*");

            int text_choice = getch();                      // get user input
            if(check_resize())                              // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 7;
            if(sound)                                       // make ANNOYING sound if sound turned on 
                beep();

            if(text_choice == 10)                           // if the user pressed Enter, go back
                break;

            while(text_choice  != 49 && text_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                text_choice = getch();                      // wait user input
                if(check_resize())                          // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return 7;
                if(sound)                                   // make ANNOYING sound if sound turned on
                    beep();

                if(text_choice == 10)                       // if the user pressed Enter, go back and free them
                    break;                                  
            }

            // determine the next step based on user input
            if(text_choice == 49)      // if 1, align the text left side
                text_position = true;
            else                       // otherwise, center the text
                text_position = false;
            break;                  
        }
        case 51:{                              // Turn *annoying* Sound ON/OFF
            move(LINES - 7, 3);
            for(int i = 0; i < 8; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 2);
            printw("Sound:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. ON");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. OFF");

            if(sound){                              // show which is the currently selected option
                move(LINES / 2 - 4, COLS - 38 + 7);
                printw("(Current)");
            }else{
                move(LINES / 2 - 3, COLS - 38 + 8);
                printw("(Current)");
            }

            move(LINES / 2 - 5, COLS / 2 - 4);
            for(int i = 0; i < 10; i++)
                printw("*");

            move(LINES / 2 - 3, COLS / 2 - 4);
            for(int i = 0; i < 10; i++)
                printw("*");

            move(LINES / 2 - 5, COLS - 40);
            for(int i = 0; i < 20; i++)
                printw("*");
            
            move (LINES / 2 - 2, COLS - 40);
            for(int i = 0; i < 20; i++)
                printw("*");

            int sound_choice = getch();                       // get user input
            if(check_resize())                                // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 7;
            if(sound)                                         // make ANNOYING sound if sound turned on
                beep();

            if(sound_choice == 10)                            // if the user pressed Enter, go back
                break;

            while(sound_choice  != 49 && sound_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                sound_choice = getch();                       // wait user input
                if(check_resize())                            // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return 7;
                if(sound)                                     // make ANNOYING sound if sound turned on
                    beep();

                if(sound_choice == 10)                        // if the user pressed Enter, go back and free them
                    break;
            }

            // determine the next step based on user input
            if(sound_choice == 49) // if 1, turn on *annoying* sound
                sound = true;
            else                   // otherwise, turn it off *blessed silence*
                sound = false;
            break;
        }
        case 52:{                              // Change number of entries per page
            move(LINES - 6, 3);
            for(int i = 0; i < 20; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 9);
            printw("Number of entries:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. 3 Entries per page");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. 5 Entries per page");
            move(LINES / 2 - 2, COLS - 38);
            printw("3. 7 Entries per page");

            switch (number_of_entries){              // show which is the currently selected option
            case 3:
                move(LINES / 2 - 4, COLS - 38 + 23);
                printw("(Current)");
                break;
            case 5:
                move(LINES / 2 - 3, COLS - 38 + 23);
                printw("(Current)");
                break;
            case 7:
                move(LINES / 2 - 2, COLS - 38 + 23);
                printw("(Current)");
                break;
            }

            move(LINES / 2 - 5, COLS / 2 - 11);
            for(int i = 0; i < 22; i++)
                printw("*");

            move(LINES / 2 - 3, COLS / 2 - 11);
            for(int i = 0; i < 22; i++)
                printw("*");

            move(LINES / 2 - 5, COLS - 40);
            for(int i = 0; i < 35; i++)
                printw("*");
            
            move (LINES / 2 - 1, COLS - 40);
            for(int i = 0; i < 35; i++)
                printw("*");

            int entries_choice = getch(); // get user input
            if(check_resize())            // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 7;
            if(sound)                     // make ANNOYING sound if sound turned on 
                beep();

            if(entries_choice == 10)      // if the user pressed Enter, go back
                break;

            while(entries_choice  != 49 && entries_choice != 50 && entries_choice != 51){ // until the user pressed a valid key, keep them trapped in an infinite loop
                entries_choice = getch();                                                 // wait user input
                if(check_resize())                                                        // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return 7;
                if(sound)                                                                 // make ANNOYING sound if sound turned on
                    beep();

                if(entries_choice == 10)                                                  // if the user pressed Enter, go back and free them
                    break;
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
            break;
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
            move(LINES - 5, 3);
            for(int i = 0; i < 18; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 8);
            printw("Continuous mode:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. ON");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. OFF");
            
            if(continuous_mode){                    // show which is the currently selected option
                move(LINES / 2 - 4, COLS - 38 + 8);
                printw("(Current)");
            }else{
                move(LINES / 2 - 3, COLS - 38 + 7);
                printw("(Current)");
            }

            move(LINES / 2 - 5, COLS / 2 - 10);
            for(int i = 0; i < 20; i++)
                printw("*");

            move(LINES / 2 - 3, COLS / 2 - 10);
            for(int i = 0; i < 20; i++)
                printw("*");

            move(LINES / 2 - 5, COLS - 40);
            for(int i = 0; i < 20; i++)
                printw("*");
            
            move (LINES / 2 - 2, COLS - 40);
            for(int i = 0; i < 20; i++)
                printw("*");

            int continuous_choice = getch(); // get user input 
            if(check_resize())               // check if the window was resized by the user and re-enter the menu to re-draw everything
                return 7;
            if(sound)                        // make ANNOYING sound if sound turned on   
                beep();

            if(continuous_choice == 10)      // if the user pressed Enter, go back
                break;

            while(continuous_choice  != 49 && continuous_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                continuous_choice = getch();                            // wait user input
                if(check_resize())                                      // check if the window was resized by the user and re-enter the menu to re-draw everything
                    return 7;
                if(sound)                                               // make ANNOYING sound if sound turned on 
                    beep();

                if(continuous_choice == 10)                             // if the user pressed Enter, go back and free them
                    break;
            }

            // determine the next step based on user input
            if(continuous_choice == 49)  // if 1, turn continuous mode on
                continuous_mode = true;
            else                         // otherwise, tun it off
                continuous_mode = false; 
            break;
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
            if(i + all_database_page_number * number_of_entries < static_cast<int>(found_databases.size())){
                if(text_position == 1)                                                                       // check if text should be left side aligned or centered and write accordingly
                    move(LINES - 4 - i, 3);
                else
                    move(LINES - 4 - i, COLS / 2 - (found_databases[i + all_database_page_number * number_of_entries].get_database_name().length() / 2));
                printw("%d.%s", found_databases[i + all_database_page_number * number_of_entries].get_database_id(), found_databases[i + all_database_page_number * number_of_entries].get_database_name().c_str());
            }
    }
}