// Menu.cpp
#include "Menu.h" // Include the corresponding header file

using namespace std;

Menu::Menu(){
    string input_settings = "../src/program_settings.txt"; // path to program_settings
    ifstream input;

    // check if program settings can be opened
    // first time launching the program, program_settings and index_manager DO NOT exist!
    input.open(input_settings);
    if (!input) {                                               
        input.clear(); 
        ofstream output(input_settings);                            // if it can't be opened, create it 
        
        ifstream get_database_index;                                // try to open index_manager to get database index to avoid loss of data
        get_database_index.open("../src/index_manager.txt");
        int index = 0;
        if(get_database_index.is_open()){                           // if it was opened, get database index
            string line;
            while(getline(get_database_index, line))
            index++;
        }else{                                                      // otherwise, create index_manager
            ofstream create_index_manager;
            create_index_manager.open("../src/index_manager.txt");
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
    available_databases.open("../src/index_manager.txt"); 
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
        output.open("../src/index_manager.txt");
        output.close();
        available_databases.open("../src/index_manager.txt");
    }

    if(!available_databases.is_open()){                         // if it cannot be opened after it was created, cry error and exit!
        cout << "index_manager.txt could not be loaded!\n";
        exit(0);
    }
    available_databases.close();
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
    
    // write out the menu options
    move(LINES / 2 - 4, COLS / 2 - 11);
    printw("Press Enter to go back.");
    refresh();
    if(check_resize())                   // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 3;

    int i = static_cast<int>(database_vector.size()) - 1;
    if(text_position == 1)                                                                         // check if text should be left side aligned or centered and write accordingly
            move(LINES - 6 - i, 3);
        else
            move(LINES - 6 - i, COLS / 2 - 9);
    printw("Available databases");

    for(i; i >= 0; i--){
        if(text_position == 1)                                                                     // check if text should be left side aligned or centered and write accordingly
            move(LINES - 4 - i, 3);
        else
            move(LINES - 4 - i, COLS / 2 - (database_vector[i].get_database_name().length() / 2));
        printw("%d.%s", i, database_vector[i].get_database_name().c_str());
    }
    
    draw_line(decorator_type);                                                                     // draw the bottom line
    if(text_position == 1)                                                                         // check if text should be left side aligned or centered and write accordingly
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 8);
    printw("Select database:");

    int choice = getch(); // get user input
    if(sound)             // make ANNOYING sound if sound turned on 
        beep();

    // determine the next step based on user input
    if(choice == 10)                                                                       // if the user pressed Enter, go back
        return 1;

    if((choice - '0') > static_cast<int>(database_vector.size())){                         // if the user pressed a number key, check if it was NOT a valid index
        if(text_position == 1)                                                             // check if text should be left side aligned or centered and write accordingly
            move(LINES - 2, 3);
        else                                                       
            move(LINES - 2, COLS / 2 - 35);
        printw("Invalid choice! Try entering a valid index. Press Enter to continue...");  // and warn him about it
        choice = getch();
        return 3;
    }
    current_database = database_vector.at(choice - '0');                                   // make the selected database active
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
    if(check_resize())                                            // check if the window was resized by the user and re-enter the menu to re-draw everything
        return 2;

    echo();                                            // enable echo 
    nocbreak();                                        // enable buffering
    char database_name [256];
    getnstr(database_name, sizeof(database_name) - 1); // get user input
    if(sound)                                          // make ANNOYING sound if sound turned on
        beep();
    noecho();                                          // no echo
    cbreak();                                          // no buffering

    if(database_name[0] == '\0')                                              // if the user pressed Enter, go back 
        return 1;

    string database_path = "../src/files/" + (string)database_name + ".txt";  // otherwise, create the path to the database 

    ofstream output;                                                          
    output.open(database_path, ios::app);                                     // create the file OR open in append to prevent loss of data
    output << "Database_id=" << current_database_index << endl;               // write the database index 
    output << "Next_object_id=0\n";                                           // write the next object's id (0 for new databases)
    output.close();                                                           // close the file
                                                                              // TODO: CHECK IF FILE ALREADY EXISTS AND WARN USER!!!

    ofstream index_manager;
    index_manager.open("../src/index_manager.txt", ios::app);                 // open index_manager to add the new database to "memory"
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
    if(check_resize())                   // check if the window was resized by the user and re-enter the menu to re-draw everything
        return 4;

    // write out the menu options, infos and warnings
    int i = static_cast<int>(database_vector.size()) - 1;
    move(LINES / 2 - 4, COLS / 2 - 5);
    printw("WARNING!!!");
    move(LINES / 2 - 3, COLS / 2 - 17);
    printw("Deleting a database is PERMANENT!!!");
    move(LINES / 2, COLS / 2 - 19);
    printw("Leave blank and press Enter to go back");
    if(text_position == 1)                                                                          // check if text should be left side aligned or centered and write accordingly
        move(LINES - 5 - i, 3);
    else
        move(LINES - 5 - i, COLS / 2 - 9);
    printw("Available databases");

    for(i; i >= 0; i--){                                                                            // starting with the LAST database in the vector
        if(text_position == 1)                                                                      // check if text should be left side aligned or centered and write accordingly
            move(LINES - 4 - i, 3);
        else
            move(LINES - 4 - i, COLS / 2 - (database_vector[i].get_database_name().length() / 2));    
        printw("%d.%s", i, database_vector[i].get_database_name().c_str());                         // print ID.Name
                                                                                                    // TODO: THIS NEEDS TO HAVE PAGES!!!
    }

    if(text_position == 1)                                                                          // check if text should be left side aligned or centered and write accordingly
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 8);
    printw("Select database: ");


    echo();                                    // enable echo 
    nocbreak();                                // enable buffering
    char choice_char[2];
    getnstr(choice_char, sizeof(choice_char)); // get user input
    if(sound)                                  // make ANNOYING sound if sound turned on
        beep();
    noecho();                                  // disable echo
    cbreak();                                  // disable buffering

    // determine the next step based on user input
    if(choice_char[0] == '\0')                               // if the user pressed Enter, go back
        return 1;

    int choice = stoi(choice_char);

    if((choice) > static_cast<int>(database_vector.size())){                  // if the user pressed a number key, check if it was NOT a valid index
        if(text_position == 1)                                                // check if text should be left side aligned or centered and write accordingly
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 29);
        printw("Invalid choice! Try entering a valid index. Select Index: "); // and warn him about it
        choice = getch(); // get user input AGAIN
        if(sound)         // make ANNOYING sound if sound turned on
            beep();

        if(choice > static_cast<int>(database_vector.size())){                // if the user pressed a number key, check if it was NOT a valid index AGAIN
            if(text_position == 1)                                            // check if text should be left side aligned or centered and write accordingly
                move(LINES - 2, 3);
            else
                move(LINES - 2, COLS / 2 - 20);
            printw("There have been too many invalid choices!");              // tell him he/she had his/her chance and go back, because deleting a database is serious bussiness
            for(int j = 44; j < COLS; j++)
                printw(" ");
            return 1;
        }
    }

    int status = database_vector[choice].delete_database(decorator_type, sound);             // if the user DID input a valid index, load the Delete Database Menu for that specific database  
    while(status != 0){                                                                      // until the deletion was confirmed or cancelled, keep re-entering that menu
        if(status == 2){                                                                     // status = 2 -> Confirmation; status = 0 -> Cancelled
            database_vector.erase(database_vector.begin() + (choice ));                      // when the deletion was confirmed, remove the database from the vector
            current_database_index = database_vector.size();                                 // get the new current_database_index
            ofstream index_manager("../src/index_manager.txt");                              // rewrite index_manager
            for(int j = 0; j < static_cast<int>(database_vector.size()); j++) 
                index_manager << j << '.' << database_vector[j].get_database_name() << '\n';
            index_manager.close();                                                           // close the file
            save_settings();                                                                 // save the settings
            reload_database_vector();                                                        // reload the database_vector to reflect the changes
            break;                                                                           // exit the loop
            }
        status = database_vector[choice].delete_database(decorator_type, sound);             // re-enter the deletion menu (IF STATUS != 0 or 2)
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
    if(check_resize())                   // check if the window was resized by the user and re-enter the menu to re-draw everything
        return 1;

    if(static_cast<int>(database_vector.size() > 0)){         // check if there are any entries to print                                                      
        int i = static_cast<int>(database_vector.size() - 1); // get the number of entries to know how to arange the menu
                                                              // TODO: THIS MENU NEEDS TO HAVE PAGES!!!

        if(text_position == 1)                                // check if text should be left side aligned or centered and write accordingly
            move(LINES - i - 6, 3);
        else
            move(LINES - i - 6, COLS / 2 - 3 - 11 - 5);
        printw("Database ID");

        if(text_position == 1)                                // check if text should be left side aligned or centered and write accordingly
            move(LINES - i - 6, 20);
        else
            move(LINES - i - 6, COLS / 2 + 6 + 5);
        printw("Database Name");  

        for(i; i >= 0; i--){                                                                                  // starting with the last entry
                                                                                                              // check if text should be left side aligned or centered and write accordingly
            if(text_position == 1){
                move(LINES - 4 - i, 3);    
                printw("%d", i);
                move(LINES - 4 - i, 20);
                printw("%s", database_vector[i].get_database_name().c_str());                                 // ID         Name
            }else{
                move(LINES - 4 - i, COLS / 2 - 3 - 5 - to_string(i).length() - 5);    
                printw("%d", i);
                move(LINES - 4 - i, COLS / 2 + 13 - database_vector[i].get_database_name().length() / 2 + 5);
                printw("%s", database_vector[i].get_database_name().c_str());                                 // ID         Name
            }
    }
    } else{
        move(LINES / 2 - 4, COLS / 2 - 11);
        printw("No databases to display");                                                                    // Or print that there's nothing to print
    }

    if(text_position == 1)                  // check if text should be left side aligned or centered and write accordingly
        move(LINES - 2, 3);
    else
        move(LINES - 2, COLS / 2 - 14);
    printw("Press any key to go back...");
    getch();                                // wait user input
    if(sound)                               // make ANNOYING sound if sound turned on
        beep();
    return 1;                               // return to Manage Databases Menu
}

int Menu::print_current_database(){
    // status: 0 - finished; 1 - previous page; 2 - next page;
    int status = current_database.print_database(decorator_type, number_of_entries, current_database_page_number, sound); // get status
    while(true){                                                                                                          // and until explicit exit
        switch (status){                                                                                                  // keep deciding the next step 
        case 0:                                
            current_database.save_database(); // save database changes
            reload_database_vector();         // reload the database_vector;
            return 3;                         // return to Load Database Menu
            break;                            // exit the loop
        case 1:                                                                                                               // Manage previous page depending on Continuous Mode
            if(continuous_mode)                                                                                               // if on
                if(current_database_page_number == 0)                                                                         // if on page 0
                    current_database_page_number = current_database.get_number_of_objects() / number_of_entries;              // go to last page
                else
                    current_database_page_number--;                                                                           // otherwise just decrement the page number
            else          
                if(current_database_page_number > 0)                                                                          // if NOT on page 0
                    current_database_page_number --;                                                                          // go to the previous page

            status = current_database.print_database(decorator_type, number_of_entries, current_database_page_number, sound); // re-enter the menu get status
            continue;
        case 2:                                                                                                               // Manage next page depending on Continuous Mode
            if(continuous_mode)                                                                                               // if on                       
                if(current_database_page_number == current_database.get_number_of_objects() / number_of_entries)              // if on the last page
                    current_database_page_number = 0;                                                                         // go to page 0
                else
                    current_database_page_number++;                                                                           // otherwise just increment the page number
            else                                                                                      
                if(current_database_page_number < current_database.get_number_of_objects() / number_of_entries)               // if NOT on the last page
                    current_database_page_number ++;                                                                          // go to next page

            status = current_database.print_database(decorator_type, number_of_entries, current_database_page_number, sound); // re-enter the menu and get status                                                                                                             
            continue;
        }
    }
    current_database.save_database(); // save database changes
    reload_database_vector();         // reload the database vector
    return 3;                         // return to load database menu
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
            if(sound)                       // make ANNOYING sound if sound turned on
                beep();

            if(decorator_choice == 10)      // if the user pressed Enter, go back
                break;

            while(decorator_choice  != 49 && decorator_choice != 50 && decorator_choice != 51 && decorator_choice != 52){ // until the user pressed a valid key, keep them trapped in an infinite loop
                decorator_choice = getch();                                                                               // wait user input
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
            if(sound)                                       // make ANNOYING sound if sound turned on 
                beep();

            if(text_choice == 10)                           // if the user pressed Enter, go back
                break;

            while(text_choice  != 49 && text_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                text_choice = getch();                      // wait user input
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
            if(sound)                                         // make ANNOYING sound if sound turned on
                beep();

            if(sound_choice == 10)                            // if the user pressed Enter, go back
                break;

            while(sound_choice  != 49 && sound_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                sound_choice = getch();                       // wait user input
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
            if(sound)                     // make ANNOYING sound if sound turned on 
                beep();

            if(entries_choice == 10)      // if the user pressed Enter, go back
                break;

            while(entries_choice  != 49 && entries_choice != 50 && entries_choice != 51){ // until the user pressed a valid key, keep them trapped in an infinite loop
                entries_choice = getch();                                                 // wait user input
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
            if(sound)                        // make ANNOYING sound if sound turned on   
                beep();

            if(continuous_choice == 10)      // if the user pressed Enter, go back
                break;

            while(continuous_choice  != 49 && continuous_choice != 50){ // until the user pressed a valid key, keep them trapped in an infinite loop
                continuous_choice = getch();                            // wait user input
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
    printw("Saving...");

    ofstream save_settings("../src/program_settings.txt");                        // open program_settings

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

    printw("Settings saved!");
    save_settings.close();                                                        // close the file
}

void Menu::reload_database_vector(){
    ifstream available_databases("../src/index_manager.txt"); // open index_manager
    string line;
    database_vector.clear();                                  // clear the database_vector
    while(getline(available_databases, line)){                // so long as there are lines to read
        Database temp_database;                               // init a temp database object
        temp_database.load_database(line);                    // and load it using the line extracted from the file
        database_vector.emplace_back(temp_database);          // then add the database object to the database_vector
    }
    available_databases.close();                              // and finally close the file
}