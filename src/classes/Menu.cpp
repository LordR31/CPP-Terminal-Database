// Menu.cpp
#include "Menu.h" // Include the corresponding header file

using namespace std;

Menu::Menu(){
    string input_settings = "../src/files/program_settings.txt";
    ifstream input(input_settings);

    int settings_parameters[7] = {0};
    if(input.is_open()){
        int i = 0;
        string line;
        while(getline(input, line)){
            size_t position = line.find("=");
            settings_parameters[i] = line[position + 1] - '0';
            i++;
        }
    }

    input.close();

    current_database_index=settings_parameters[0];
    decorator_type = settings_parameters[1];
    text_position = settings_parameters[2];
    sound = settings_parameters[3];
    number_of_entries = settings_parameters[4];
    compact_mode = settings_parameters[5];
    continuous_mode = settings_parameters[6];

    // Init Databases from file
    ifstream available_databases("../src/files/index_manager.txt");
    string line;
    while(getline(available_databases, line)){
        Database temp_database;
        temp_database.load_database(line);
        database_vector.emplace_back(temp_database);
    }
    available_databases.close();

    // printw("Menu initialized and ready.\n");
}

int Menu::print_menu(){
    while(true){
        clear();
        draw_main_box();
        move(1,3);
        printw("Main Menu");
        move(1,COLS - 28);
        printw("Terminal Database Manager");
        move(LINES - 6,3);
        printw("1. Manage Databases");
        move(LINES - 5,3);
        printw("2. Settings");
        move(LINES - 4,3);
        printw("3. Quit Program");
        draw_line();
        refresh();
        if(check_resize())
            return 0;

        int choice = getch();

        switch (choice){
        case 49: // 1 in ASCII
            return 1;
            break;
        case 50: // 2 in ASCII
            return 7;
            break;
        case 51: // 3 in ASCII
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
        clear(); 
        draw_main_box(); 
        move(1,3);
        printw("Manage Available databases");
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        move(LINES - 7, 3);
        printw("1. Create Database");
        move(LINES - 6, 3);
        printw("2. Load Database");
        move(LINES - 5, 3);
        printw("3. Delete Database");
        move(LINES - 4, 3);
        printw("4. Print available databases");
        draw_line();
        move(LINES - 2, 3);
        printw("Press Enter to go back to main menu.");
        refresh();
        if(check_resize())
            return 1;

        int choice = getch();

        switch (choice){
        case 10: // Enter in ASCII
            return 0;
        case 49: // 1 in ASCII
            return 2;
            break;
        case 50: // 2 in ASCII
            return 3;
            break;
        case 51: // 3 in ASCII
            return 4;
            break;
        case 52: // 4 in ASCII
            return 5;
            break;
        default:
            printw("Invalid Choice!");
            break;
        }
    }
}

int Menu::load_database(){
    clear();
    draw_main_box();
    move(1,3);
    printw("Load Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    
    move(LINES / 2 - 4, COLS / 2 - 12);
    printw("Press Enter to go back.");
    refresh();
    if(check_resize())
            return 3;

    int i = static_cast<int>(database_vector.size()) - 1;
    move(LINES - 4 - i, 3);
    printw("Available databases");

    for(i; i > 0; i--){
        move(LINES - 3 - i, 3);
        printw("%d.%s", database_vector[i].get_database_id(), database_vector[i].get_database_name().c_str());
    }
    
    draw_line();
    move(LINES - 2, 3);
    printw("Select database:");

    int choice = getch();

    if(choice == 10)
        return 1;

    if((choice - '0') > static_cast<int>(database_vector.size())){
        move(LINES - 1, 3);
        printw("\n Invalid choice! Try entering a valid index. Press Enter to continue...\n");
        choice = getch();
        return 3;
    }
    current_database = database_vector.at(choice - '0');
    return 6; // Go to The selected database menu
}

int Menu::create_database(){
    clear();
    draw_main_box();
    move(1,3);
    printw("Create Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    move(LINES / 2 - 4, COLS / 2 - 25);
    printw("Leave blank and press Enter to cancel and go back!");
    draw_line();
    move(LINES - 2,3);
    printw("Database name: ");
    if(check_resize())
        return 2;

    echo();     // enable echo 
    nocbreak(); // enable buffering

    char database_name [256];
    getnstr(database_name, sizeof(database_name) - 1);

    noecho(); // no echo
    cbreak(); // no buffering

    if(database_name[0] == '\0')
        return 1;

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
    return 1;
}

int Menu::delete_database(){
    clear();
    draw_main_box();
    move(1, 3);
    printw("Delete Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    draw_line();
    refresh();
    if(check_resize())
        return 4;

    int i = static_cast<int>(database_vector.size()) - 1;

    move(LINES / 2 - 4, COLS / 2 - 5);
    printw("WARNING!!!");
    move(LINES / 2 - 3, COLS / 2 - 17);
    printw("Deleting a database is PERMANENT!!!");
    move(LINES / 2, COLS / 2 - 19);
    printw("Leave blank and press Enter to go back");
    move(LINES - 5 - i, 3);
    printw("Available databases");
    move(LINES - 2, 3);
    printw("Select database:");

    for(i; i >= 0; i--){
        move(LINES - 4 - i, 3);    
        printw("%d.%s", i, database_vector[i].get_database_name().c_str());
    }

    int choice = getch();

    if(choice == 10)
        return 1;

    if((choice - '0') > static_cast<int>(database_vector.size())){
        move(LINES - 2, 3);
        printw("Invalid choice! Try entering a valid index. Select Index: ");
        choice = getch();

        if(choice > static_cast<int>(database_vector.size())){
            move(LINES - 2, 3);
            printw("There have been too many invalid choices!");
            for(int j = 44; j < COLS; j++)
                printw(" ");
            return 1;
        }
    }
    int status = database_vector[choice - '0'].delete_database();
    if(status == 1)
        database_vector[choice - '0'].delete_database();
    if(status == 2){
        database_vector.erase(database_vector.begin() + (choice - '0'));
        current_database_index = database_vector.size();
    }

    ofstream index_manager("../src/files/index_manager.txt");
    for(int j = 0; j < static_cast<int>(database_vector.size()); j++)
        index_manager << j << '.' << database_vector[j].get_database_name() << '\n';
    index_manager.close();
    save_settings();
    reload_database_vector();
    return 1;
}

int Menu::print_databases(){
    clear();
    draw_main_box();
    move(1, 3);
    printw("Available Databases");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    draw_line();
    refresh();
    if(check_resize())
        return 1;

    int i = static_cast<int>(database_vector.size()) - 1;

    move(LINES - i - 5, 3);
    printw("Database ID");

    move(LINES - i - 5, 20);
    printw("Database Name");

    for(i; i >= 0; i--){
        move(LINES - 4 - i, 8);    
        printw("%d", i);
        move(LINES - 4 - i, 24);
        printw("%s", database_vector[i].get_database_name().c_str());
    }

    move(LINES - 2, 3);
    printw("Press any key to go back...\n");
    getch();
    return 1;
}

int Menu::print_current_database(){
    bool status = current_database.print_database();
    while(status)
        status = current_database.print_database();
    current_database.save_database();
    reload_database_vector();
    return 3;
}

int Menu::settings(){
    while(true){
        clear();
        draw_main_box();
        move(1,3);
        printw("Settings");
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        move(LINES - 10, 3);
        printw("1. Decorator type");
        move(LINES - 9, 3);
        printw("2. Text position");
        move(LINES - 8, 3);
        printw("3. Sound");
        move(LINES - 7, 3);
        printw("4. Number of entries");
        move(LINES - 6, 3);
        printw("5. Compact mode");
        move(LINES - 5, 3);
        printw("6. Continuous mode");
        move(LINES - 4, 3);
        printw("7. Save settings");
        draw_line();
        move(LINES - 2, 3);
        printw("Press Enter to go back to main menu.");
        refresh();
        if(check_resize())
            return 7;

        int choice = getch();

        switch (choice){
        case 49:{
            move(LINES - 10, 3);
            for(int i = 0; i < 17; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 8);
            printw("Decorator Type:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. * (Current)");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. ~");
            move(LINES / 2 - 2, COLS - 38);
            printw("3. +");
            move(LINES / 2 - 1, COLS - 38);
            printw("4. /");

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
            

            int decorator_choice = getch();
            decorator_type = decorator_choice - '0';
            break;
        }
        case 50:{
            move(LINES - 9, 3);
            for(int i = 0; i < 16; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 7);
            printw("Text position:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. Left side (Default)");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. Centered");
            
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

            int text_choice = getch();
            text_position = text_choice - '0';
            break;
        }
        case 51:{
            move(LINES - 8, 3);
            for(int i = 0; i < 8; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 2);
            printw("Sound:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. ON");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. OFF (Current)");

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

            int sound_choice = getch();
            sound = sound_choice - '0';
            break;
        }
        case 52:{
            move(LINES - 7, 3);
            for(int i = 0; i < 20; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 9);
            printw("Number of entries:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. 3 Entries per page");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. 5 Entries per page (Current)");
            move(LINES / 2 - 2, COLS - 38);
            printw("3. 7 Entries per page");

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

            int entries_choice = getch();
            number_of_entries = entries_choice - '0';
            break;
        }
        case 53:{
            move(LINES - 6, 3);
            for(int i = 0; i < 18; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 6);
            printw("Compact mode:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. OFF (Current)");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. ON");
            
            move(LINES / 2 - 5, COLS / 2 - 8);
            for(int i = 0; i < 17; i++)
                printw("*");

            move(LINES / 2 - 3, COLS / 2 - 8);
            for(int i = 0; i < 17; i++)
                printw("*");

            move(LINES / 2 - 5, COLS - 40);
            for(int i = 0; i < 20; i++)
                printw("*");
            
            move (LINES / 2 - 2, COLS - 40);
            for(int i = 0; i < 20; i++)
                printw("*");

            int compact_choice = getch();
            compact_mode = compact_choice - '0';
            break;
        }
        case 54:{
            move(LINES - 5, 3);
            for(int i = 0; i < 15; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 8);
            printw("Continuous mode:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. OFF (Current)");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. ON");
            
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

            int continuous_choice = getch();
            continuous_mode = continuous_choice - '0';
            break;
        }
        case 55:{
            save_settings();
            return 0;
        }
        case 56:{
            return 0;
            break;
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