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
        draw_main_box(decorator_type);
        move(1,3);
        printw("Main Menu");
        move(1,COLS - 28);
        printw("Terminal Database Manager");
        if(text_position == 1)
            move(LINES - 6,3);
        else
            move(LINES - 6, COLS / 2 - 9);
        printw("1. Manage Databases");
        if(text_position == 1)
            move(LINES - 5,3);
        else
            move(LINES - 5, COLS / 2 - 5);
        printw("2. Settings");
        if(text_position == 1)
            move(LINES - 4,3);
        else
            move(LINES - 4, COLS / 2 - 7);
        printw("3. Quit Program");
        draw_line(decorator_type);
        refresh();
        if(check_resize())
            return 0;

        int choice = getch();
        if(sound)
            beep();

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
        draw_main_box(decorator_type); 
        move(1,3);
        printw("Manage Available databases");
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        if(text_position == 1)
            move(LINES - 7, 3);
        else
            move(LINES - 7, COLS / 2 - 9);
        printw("1. Create Database");
        if(text_position == 1)
            move(LINES - 6, 3);
        else
            move(LINES - 6, COLS / 2 - 8);
        printw("2. Load Database");
        if(text_position == 1)
            move(LINES - 5, 3);
        else
            move(LINES - 5, COLS / 2 - 9);
        printw("3. Delete Database");
        if(text_position == 1)
            move(LINES - 4, 3);
        else
            move(LINES - 4, COLS / 2 - 14);
        printw("4. Print available databases");
        draw_line(decorator_type);
        if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 17);
        printw("Press Enter to go back to main menu");
        refresh();
        if(check_resize())
            return 1;

        int choice = getch();
        if(sound)
            beep();

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
    draw_main_box(decorator_type);
    move(1,3);
    printw("Load Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    
    move(LINES / 2 - 4, COLS / 2 - 11);
    printw("Press Enter to go back.");
    refresh();
    if(check_resize())
            return 3;

    int i = static_cast<int>(database_vector.size()) - 1;
    if(text_position == 1)
            move(LINES - 6 - i, 3);
        else
            move(LINES - 6 - i, COLS / 2 - 9);
    printw("Available databases");

    for(i; i >= 0; i--){
        if(text_position == 1)
            move(LINES - 4 - i, 3);
        else
            move(LINES - 4 - i, COLS / 2 - (database_vector[i].get_database_name().length() / 2));
        printw("%d.%s", i, database_vector[i].get_database_name().c_str());
    }
    
    draw_line(decorator_type);
    if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 8);
    printw("Select database:");

    int choice = getch();
    if(sound)
        beep();

    if(choice == 10)
        return 1;

    if((choice - '0') > static_cast<int>(database_vector.size())){
        if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 35);
        printw("Invalid choice! Try entering a valid index. Press Enter to continue...");
        choice = getch();
        return 3;
    }
    current_database = database_vector.at(choice - '0');
    return 6; // Go to The selected database menu
}

int Menu::create_database(){
    clear();
    draw_main_box(decorator_type);
    move(1,3);
    printw("Create Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    move(LINES / 2 - 4, COLS / 2 - 25);
    printw("Leave blank and press Enter to cancel and go back!");
    draw_line(decorator_type);
    if(text_position == 1)
            move(LINES - 2,3);
        else
            move(LINES - 2, COLS / 2 - 7);
    printw("Database name: ");
    if(check_resize())
        return 2;

    echo();     // enable echo 
    nocbreak(); // enable buffering

    char database_name [256];
    getnstr(database_name, sizeof(database_name) - 1);
    if(sound)
        beep();

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

    if(text_position == 1)
            move(LINES - 2,3);
        else
            move(LINES - 2, COLS / 2 - 15);
    printw("Database created successfully!");
    return 1;
}

int Menu::delete_database(){
    clear();
    draw_main_box(decorator_type);
    move(1, 3);
    printw("Delete Database");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    draw_line(decorator_type);
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
    if(text_position == 1)
        move(LINES - 5 - i, 3);
    else
        move(LINES - 5 - i, COLS / 2 - 9);
    printw("Available databases");
    if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 8);
    printw("Select database:");

    for(i; i >= 0; i--){
        if(text_position == 1)
            move(LINES - 4 - i, 3);
        else
            move(LINES - 4 - i, COLS / 2 - (database_vector[i].get_database_name().length() / 2));    
        printw("%d.%s", i, database_vector[i].get_database_name().c_str());
    }

    int choice = getch();
    if(sound)
        beep();

    if(choice == 10)
        return 1;

    if((choice - '0') > static_cast<int>(database_vector.size())){
        if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 29);
        printw("Invalid choice! Try entering a valid index. Select Index: ");
        choice = getch();
        if(sound)
            beep();

        if(choice > static_cast<int>(database_vector.size())){
            if(text_position == 1)
                move(LINES - 2, 3);
            else
                move(LINES - 2, COLS / 2 - 20);
            printw("There have been too many invalid choices!");
            for(int j = 44; j < COLS; j++)
                printw(" ");
            return 1;
        }
    }
    int status = database_vector[choice - '0'].delete_database(decorator_type, sound);
    if(status == 1)
        database_vector[choice - '0'].delete_database(decorator_type, sound);
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
    draw_main_box(decorator_type);
    move(1, 3);
    printw("Available Databases");
    move(1, COLS - 28);
    printw("Terminal Database Manager");
    draw_line(decorator_type);
    refresh();
    if(check_resize())
        return 1;

    int i = static_cast<int>(database_vector.size()) - 1;

    if(text_position == 1)
        move(LINES - i - 6, 3);
    else
        move(LINES - i - 6, COLS / 2 - 3 - 11 - 5);
    printw("Database ID");

    if(text_position == 1)
        move(LINES - i - 6, 20);
    else
        move(LINES - i - 6, COLS / 2 + 6 + 5);
    printw("Database Name");

    for(i; i >= 0; i--){
        if(text_position == 1){
            move(LINES - 4 - i, 3);    
            printw("%d", i);
            move(LINES - 4 - i, 20);
            printw("%s", database_vector[i].get_database_name().c_str());
        }else{
            move(LINES - 4 - i, COLS / 2 - 3 - 5 - to_string(i).length() - 5);    
            printw("%d", i);
            move(LINES - 4 - i, COLS / 2 + 13 - database_vector[i].get_database_name().length() / 2 + 5);
            printw("%s", database_vector[i].get_database_name().c_str());
        }
    }

    if(text_position == 1)
        move(LINES - 2, 3);
    else
        move(LINES - 2, COLS / 2 - 14);
    printw("Press any key to go back...");
    getch();
    if(sound)
        beep();
    return 1;
}

int Menu::print_current_database(){
    // status: 0 - finished; 1 - previous page; 2 - next page;
    int status = current_database.print_database(decorator_type, number_of_entries, current_database_page_number, sound);
    while(true){
        switch (status){
        case 0:
            current_database.save_database();
            reload_database_vector();
            return 3;
            break;
        case 1: 
            if(continuous_mode)
                if(current_database_page_number == 0)
                    current_database_page_number = current_database.get_number_of_objects() / number_of_entries;
                else
                    current_database_page_number--;
            else
                if(current_database_page_number > 0)
                    current_database_page_number --;

            status = current_database.print_database(decorator_type, number_of_entries, current_database_page_number, sound);
            break;
        case 2:
            if(continuous_mode)
                if(current_database_page_number == current_database.get_number_of_objects() / number_of_entries)
                    current_database_page_number = 0;
                else
                    current_database_page_number++;
            else
                if(current_database_page_number < current_database.get_number_of_objects() / number_of_entries)
                    current_database_page_number ++;

            status = current_database.print_database(decorator_type, number_of_entries, current_database_page_number, sound);
            break;
        }
    }
    current_database.save_database();
    reload_database_vector();
    return 3;
}

int Menu::settings(){
    while(true){
        clear();
        draw_main_box(decorator_type);
        move(1,3);
        printw("Settings");
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        if(text_position == 1)
            move(LINES - 9, 3);
        else
            move(LINES - 9, COLS / 2 - 8);
        printw("1. Decorator type");
        if(text_position == 1)
            move(LINES - 8, 3);
        else
            move(LINES - 8, COLS / 2 - 8);
        printw("2. Text position");
        if(text_position == 1)
            move(LINES - 7, 3);
        else
            move(LINES - 7, COLS / 2 - 4);
        printw("3. Sound");
        if(text_position == 1)
            move(LINES - 6, 3);
        else
            move(LINES - 6, COLS / 2 - 10);
        printw("4. Number of entries");
        // if(text_position == 1)
        //     move(LINES - 6, 3);
        // else
        //     move(LINES - 6, COLS / 2 - 7);
        // printw("5. Compact mode");
        if(text_position == 1)
            move(LINES - 5, 3);
        else
            move(LINES - 5, COLS / 2 - 9);
        printw("5. Continuous mode");
        if(text_position == 1)
            move(LINES - 4, 3);
        else
            move(LINES - 4, COLS / 2 - 8);
        printw("6. Save settings");
        draw_line(decorator_type);
        if(text_position == 1)
            move(LINES - 2, 3);
        else
            move(LINES - 2, COLS / 2 - 18);
        printw("Press Enter to go back to main menu.");
        refresh();
        if(check_resize())
            return 7;

        int choice = getch();
        if(sound)
            beep();

        switch (choice){
        case 49:{
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

            switch (decorator_type){
            case 1:
                move(LINES / 2 - 4, COLS - 38 + 6);
                printw("(Current)");
                break;
            case 2:
                move(LINES / 2 - 3, COLS - 38 + 6);
                printw("(Current)");
                break;
            case 3:
                move(LINES / 2 - 2, COLS - 38 + 6);
                printw("(Current)");
                break;
            case 4:
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
            

            int decorator_choice = getch();
            if(sound)
                beep();
            if(decorator_choice == 10)
                break;

            while(decorator_choice  != 49 && decorator_choice != 50 && decorator_choice != 51 && decorator_choice != 52){
                decorator_choice = getch();
                if(sound)
                    beep();

                if(decorator_choice == 10)
                    break;
            }
            switch (decorator_choice){
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
        case 50:{
            move(LINES - 8, 3);
            for(int i = 0; i < 16; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 7);
            printw("Text position:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. Left side");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. Centered");
            
            if(text_position){
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

            int text_choice = getch();
            if(sound)
                beep();

            if(text_choice == 10)
                break;

            while(text_choice  != 49 && text_choice != 50){
                text_choice = getch();
                if(sound)
                    beep();

                if(text_choice == 10)
                    break;
            }

            if(text_choice == 49)
                text_position = true;
            else
                text_position = false;
            break;
        }
        case 51:{
            move(LINES - 7, 3);
            for(int i = 0; i < 8; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 2);
            printw("Sound:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. ON");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. OFF");

            if(sound){
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

            int sound_choice = getch();
            if(sound_choice == 10)
                break;

            while(sound_choice  != 49 && sound_choice != 50){
                sound_choice = getch();
                if(sound)
                    beep();

                if(sound_choice == 10)
                    break;
            }
            if(sound_choice == 49)
                sound = true;
            else
                sound = false;
            if(sound)
                beep();
            
            break;
        }
        case 52:{
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

            switch (number_of_entries){
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

            int entries_choice = getch();
            if(sound)
                beep();

            if(entries_choice == 10)
                break;

            while(entries_choice  != 49 && entries_choice != 50 && entries_choice != 51){
                entries_choice = getch();
                if(sound)
                    beep();

                if(entries_choice == 10)
                    break;
            }
            switch (entries_choice){
            case 49:
                number_of_entries = 3;
                break;
            case 50:
                number_of_entries = 5;
                break;
            case 51:
                number_of_entries = 7;
            }
            break;
        }
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
        case 53:{
            move(LINES - 5, 3);
            for(int i = 0; i < 18; i++)
                printw(" ");
            move(LINES / 2 - 4, COLS / 2 - 8);
            printw("Continuous mode:");
            move(LINES / 2 - 4, COLS - 38);
            printw("1. ON");
            move(LINES / 2 - 3, COLS - 38);
            printw("2. OFF");
            
            if(continuous_mode){
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

            int continuous_choice = getch();
            if(sound)
                beep();

            if(continuous_choice == 10)
                break;

            while(continuous_choice  != 49 && continuous_choice != 50){
                continuous_choice = getch();
                if(sound)
                    beep();

                if(continuous_choice == 10)
                    break;
            }
            if(continuous_choice == 49)
                continuous_mode = true;
            else
                continuous_mode = false;
            break;
        }
        case 54:{
            save_settings();
            return 0;
        }
        case 10:{
            return 0;
        }
        }
    }
}

void Menu::save_settings(){
    printw("Saving...");

    ofstream save_settings("../src/files/program_settings.txt");

    save_settings << "Current_database_index=" << current_database_index << endl;
    switch (decorator_type){
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