// Menu.cpp
#include "Menu.h" // Include the corresponding header file

using namespace std;

Menu::Menu(){
    string input_settings = "../src/files/program_settings.txt";
    ifstream input(input_settings);

    int settings[7];
    int i = 0;
    if(input.is_open()){
        string line;
        int index;
        while(getline(input, line)){
            size_t position = line.find("=");
            settings[i] = line[position + 1] - '0';
            i++;
        }
    }

    input.close();

    current_database_index=settings[0];
    decorator_type = settings[1];
    text_position = settings[2];
    sound = settings[3];
    number_of_entries = settings[4];
    compact_mode = settings[5];
    continuous_mode = settings[6];

    // Init Databases from file
    ifstream available_databases("../src/files/index_manager.txt");
    string line;
    while(getline(available_databases, line)){
        Database temp_database;
        temp_database.load_database(line);
        database_vector.emplace_back(temp_database);
    }
    available_databases.close();

    printw("Menu initialized and ready.\n");
}

void Menu::print_menu(){
    while(true){
        clear();
        printw("Main Menu \t\t\t\tTerminal Database");
        printw("\n\n\n\n\n\n1.Manage Databases\n");
        printw("2.Settings\n");
        printw("3.Quit Program\n");
        printw("-------------------------------------\n");

        int choice = getch();

        switch (choice){
        case 49: // 1 in ASCII
            manage_database();
            break;
        case 50: // 2 in ASCII
            settings();
            break;
        case 51: // 3 in ASCII
            endwin();
            printw("Exitting program!\n");
            exit(0);
            break;
        default:
            printw("Invalid Input!");
            break;
        }
    }
}

void Menu::manage_database(){
    while(true){
        clear();
        printw("Manage Available databases\n\n");
        printw("1. Create Database\n");
        printw("2. Load Database\n");
        printw("3. Delete Database\n");
        printw("4. Print available databases\n");
        printw("0. Back\n");
        printw("------------------------\n");

        int choice = getch();

        switch (choice){
        case 48: // 0 in ASCII
            print_menu();
        case 49: // 1 in ASCII
            create_database();
            break;
        case 50: // 2 in ASCII
            load_database();
            break;
        case 51: // 3 in ASCII
            delete_database();
            break;
        case 52: // 4 in ASCII
            print_databases();
            break;
        default:
            printw("Invalid Choice!");
            break;
        }
    }
}

void Menu::load_database(){
    clear();
    printw("Load Database\n\n");
    printw("Available databases\n");
    printw("-----------------------\n");

    for(int i = 0; i < database_vector.size(); i++)
        printw("%d.%s\n", database_vector[i].get_database_id(), database_vector[i].get_database_name().c_str());
    
    printw("-------------------\n");
    printw("\nSelect database:");

    int choice = getch();

    if((choice - '0') > database_vector.size()){
        printw("\n Invalid choice! Try entering a valid index.\n");
        printw("\nSelect database:");
        choice = getch();

        if(choice > database_vector.size()){
            printw("\n There have been too many invalid choices!");
            return;
        }
    }
    current_database = database_vector.at(choice - '0');
    current_database.print_database();
    current_database.save_database();
    reload_database_vector();
}

void Menu::create_database(){
    printw("Create Database\n\n");
    printw("Database name: ");
    
    echo();     // enable echo 
    nocbreak(); // enable buffering

    char database_name [256];
    getnstr(database_name, sizeof(database_name) - 1);

    noecho(); // no echo
    cbreak(); // no buffering

    string database_path = "../src/files/" + (string)database_name + ".txt";

    ofstream output;
    output.open(database_path, ios::app);
    output << "Database_id=" << current_database_index << endl;
    output << "Next_object_id=0\n";
    output.close();

    ofstream index_manager;
    index_manager.open("../src/files/index_manager.txt", ios::app);
    index_manager << current_database_index << "." << database_name << endl;
    index_manager.close();

    current_database_index = database_vector.size() + 1;
    save_settings();
    reload_database_vector();

    printw("\nDatabase created successfully!\n");
}

void Menu::delete_database(){
    clear();
    printw("Delete Database \t\t\t\t\t Terminal Database Manager\n");
    printw("\n\n\nWARNING!!\nDeleting a database is PERMANENT!\n\n\n");
    printw("Select the database you want to delete\n\n\n");

    for(int i = 0; i < database_vector.size(); i++)
        printw("%d.%s\n", i, database_vector[i].get_database_name().c_str());

    int choice = getch();

    if((choice - '0') > database_vector.size()){
        printw("\n Invalid choice! Try entering a valid index.\n");
        printw("\nSelect database:");
        choice = getch();

        if(choice > database_vector.size()){
            printw("\n There have been too many invalid choices!");
            return;
        }
    }
    database_vector[choice - '0'].delete_database();
    database_vector.erase(database_vector.begin() + (choice - '0'));
    current_database_index = database_vector.size();

    ofstream index_manager("../src/files/index_manager.txt");
    for(int i = 0; i < database_vector.size(); i++)
        index_manager << i << '.' << database_vector[i].get_database_name() << '\n';
    index_manager.close();
    save_settings();
    reload_database_vector();
}

void Menu::print_databases(){
    clear();
    printw("Available Databases \t\t\t\t\tTerminal Database Manager\n");
    printw("\n\n\n\n");

    for(int i = 0; i < database_vector.size(); i++)
        printw("%d.%s\n", i, database_vector[i].get_database_name().c_str());

    printw("Press any key to go back...\n");
    getch();
    manage_database();
}

void Menu::settings(){
    while(true){
        clear();
        printw("Settings \t\t\t Terminal Database Manager\n\n\n\n\n\n");
        printw("1. Decorator type\n");
        printw("2. Text position\n");
        printw("3. Sound\n");
        printw("4. Number of entries\n");
        printw("5. Compact mode\n");
        printw("6. Continuous mode\n");
        printw("7. Save settings\n");
        printw("----------------------------------------------\n");

        int choice = getch();

        switch (choice){
        case 49:{
            clear();
            printw("Decorator Type\n");
            printw("1. * (Current)\n");
            printw("2. ~\n");
            printw("3. +\n");
            printw("4. /\n");
            printw("-------------------------------------\n");

            int decorator_choice = getch();
            decorator_type = decorator_choice - '0';
            break;
        }
        case 50:{
            clear();
            printw("Text position\n");
            printw("1. Left side (Default)\n");
            printw("2. Centered\n");
            printw("-----------------------------\n");
            
            int text_choice = getch();
            text_position = text_choice - '0';
            break;
        }
        case 51:{
            clear();
            printw("Sound\n");
            printw("1. ON (Default)\n");
            printw("2. OFF\n");
            printw("-----------------------------\n");

            int sound_choice = getch();
            sound = sound_choice - '0';
            break;
        }
        case 52:{
            clear();
            printw("Number of entries\n");
            printw("1. 3 Entries per page\n");
            printw("2. 5 Entries per page (Default)\n");
            printw("3. 7 Entries per page\n");
            printw("----------------------------------------\n");

            int entries_choice = getch();
            number_of_entries = entries_choice - '0';
            break;
        }
        case 53:{
            clear();
            printw("Compact mode\n");
            printw("1. OFF (Default)\n");
            printw("2. ON\n");
            printw("---------------------------------\n");

            int compact_choice = getch();
            compact_mode = compact_choice - '0';
            break;
        }
        case 54:{
            clear();
            printw("Continuous mode\n");
            printw("1. OFF (Default)\n");
            printw("2. ON\n");
            printw("--------------------------------\n");
            
            int continuous_choice = getch();
            continuous_mode = continuous_choice - '0';
            break;
        }
        case 55:{
            save_settings();
            print_menu();
        }
        default:
            printw("\nInvalid choice!\n");
            break;
        }
    }
}

void Menu::save_settings(){
    printw("Saving...");

    ofstream save_settings("../src/files/program_settings.txt");

    save_settings << "Current_database_index=" << current_database_index << endl;
    save_settings << "Decorator_type="         << decorator_type         << endl;
    save_settings << "Text_position="          << text_position          << endl;
    save_settings << "Sound="                  << sound                  << endl;
    save_settings << "Number_of_entries="      << number_of_entries      << endl;
    save_settings << "Compact_mode="           << compact_mode           << endl;
    save_settings << "Continuous_mode="        << continuous_mode        << endl;

    printw("Settings saved!");
    save_settings.close();
}

void Menu::reload_database_vector(){
    ifstream available_databases("../src/files/index_manager.txt");
    string line;
    database_vector.clear();
    while(getline(available_databases, line)){
        Database temp_database;
        temp_database.load_database(line);
        database_vector.emplace_back(temp_database);
    }
    available_databases.close();
}