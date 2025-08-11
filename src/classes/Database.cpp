// Database.cpp
#include "Database.h" // Include the corresponding header file

using namespace std;

Database::Database(){}

Database::Database(const int& id, const string& name): database_id(id), database_name(name){}

int Database::get_database_id(){
    return database_id;
}

const string& Database::get_database_name(){
    return database_name;
}

int Database::get_number_of_objects(){
    return database_objects.size();
}

void Database::load_database(const string& database_identifiers){ 
    int pos = database_identifiers.find('.');                // find the . separating id from name
    database_id = stoi(database_identifiers.substr(0, pos)); // extract id
    database_name = database_identifiers.substr(pos + 1);    // extract name
    load_objects();                                          // call the function for loading objects from file
}

int Database::print_database(char decorator, int number_of_entries, int page_number, bool sound){
    while(true){
        // create the outline box using the preferred decorator, write the menu name and program title
        clear();
        draw_main_box(decorator);
        move(1, 3);
        printw("%s", database_name.c_str());
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        draw_line(decorator);
        refresh();
        if(check_resize())                   // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 1;

        bool is_empty = false;
        bool is_paged = false;
        if (database_objects.empty()){                                        // check if the database is empty
            move(8, COLS / 2 - 25);
            printw("Database is empty. No objects to display.");              // and tell the user there's nothing to display
            is_empty = true;
        }else{
            if(static_cast<int>(database_objects.size()) > number_of_entries) // if there IS something to display, check if you need pages
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

            string page_number_string = "Page " + to_string(page_number);     // Print the page number
            move(LINES / 2, COLS / 2 - page_number_string.length() / 2);
            printw("%s", page_number_string.c_str());

            for(int i = 0; i < number_of_entries; i++){                       // and then print the objects, as many on a page as selected by the user (number_of_entries)
                if(i + page_number * number_of_entries < static_cast<int>(database_objects.size())){
                    move(LINES - 4 - i, 3);
                    printw("%d",database_objects[i + page_number * number_of_entries].get_id());
                    move(LINES - 4 - i, 8);
                    printw("%s",database_objects[i + page_number * number_of_entries].get_name().c_str());
                    move(LINES - 4 - i, COLS - 10 - 50);
                    printw("%s", database_objects[i + page_number * number_of_entries].get_type().c_str());
                    move(LINES - 4 - i, COLS - 5 - to_string(database_objects[i + page_number * number_of_entries].get_quantity()).length() / 2 - 1);
                    printw("%d", database_objects[i + page_number * number_of_entries].get_quantity());
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
        if(sound)             // make ANNOYING sound if sound turned on
            beep();

        // determine the next step based on user input
        switch (choice){
            case 49: {                                         // 1 -> Previous page
                if(is_paged)                                   // if the button is shown (there are pages)
                    return 1;                                  // return to Load Databases and determine how to deal with the previous page command
                else
                    continue;                                  // otherwise just ignore this input
            }
            case 50:{                                          // 2 -> Add item to database
                add_object(decorator, sound);                  // call the function to add an object to the database
                continue;
            }
            case 51:{                                          // 3 -> Delete item from database
                bool status = delete_object(decorator, sound); // get deletion status
                while(status)                                  // keep calling the function and getting status until you get status 0 (finished, could be either deleted or cancelled, doesn't matter)
                    status = delete_object(decorator, sound);
                continue;
            }
            case 52:{                                          // 4 -> Back
                return 0;                                      // return to Load Database Menu
            }

                                                               // TODO: Add Find & Edit button
                                                               // TODO: Add number of elements counter to screen
                                                               // TODO: Add number of pages to screen:    Page X/XX

            case 53:{                                          // 5 -> Next page
                if(is_paged)                                   // if the button is shown (there are pages)            
                    return 2;                                  // return to Load Databases and determine how to deal with the next page command
                else
                    continue;                                  // otherwise just ignore this input
            }
            default:{                                          // ignore other inputs
                continue;
            }    
        }
        return 0;                                              // return to Load Database menu
    }
}

void Database::save_database(){
    string file = "../src/files/" + database_name + ".txt";            // craft the path to the file
    ofstream output(file);                                             // open the file
    
    output << "Database_id=" + to_string(database_id) + '\n';          // write database id
    output << "Next_object_id=" + to_string(next_object_id) + '\n';    // and name
    for(int i = 0; i < static_cast<int>(database_objects.size()); i++) // then write the objects
        output << save_objects_to_file(i);
    output.close();                                                    // and finally close the file
}

string Database::save_objects_to_file(int index){
    string object_in_string_format;
    // craft a string which can be safely written in a file from the object whose index you get as parameter on function call and return it (function called in save_database())
    object_in_string_format = to_string(database_objects[index].get_id()) + ' ' + database_objects[index].get_name() + ' ' + database_objects[index].get_type() + ' ' + to_string(database_objects[index].get_quantity()) + '\n';
    return object_in_string_format;
}

int Database::delete_database(char decorator, bool sound){
    // create the outline box using the preferred decorator, write the infos and warnings
    clear();
    draw_main_box(decorator);
    move(5, COLS / 2 - 21);
    printw("WARNING!! Deleting a database is PERMANENT!");
    move(7, COLS / 2 - 22);
    printw("Type DELETE to confirm deleting the database!");
    move(8, COLS / 2 - 19);
    printw("Leave blank and press Enter to cancel!");
    draw_line(decorator);                                    // draw the bottom line
    move(LINES - 2, 3);
    //TODO: ADD RESIZE!!!

    echo();                                      // echo on
    nocbreak();                                  // buffering on
    char confirmation[6];                        
    getnstr(confirmation, sizeof(confirmation)); // get user input
    if(sound)                                    // make ANNOYING sound if sound turned on
        beep(); 
    noecho();                                    // no echo
    cbreak();                                    // no buffering

    if(confirmation[0] == '\0')                  // if the user left blank and pressed Enter, go back
        return 0;

    if(strcmp(confirmation, "DELETE") != 0){                                    // if the user unsuccessfully typed "DELETE"
        move(LINES - 2, 3);
        printw("Invalid Input! Do you want to try again? (Y / N): ");           // warn them and ask if they want to try again
        int input = getch();
        if(sound)
            beep();

        if(input == 121 || input == 89)      // lower and upper case y in ASCII
            return 1;                                                           // re-enter the menu
        else if(input == 110 || input == 78) // lower and upper case n in ASCII
            return 0;                                                           // exit menu
    }
    string database_path = "../src/files/" + get_database_name() + ".txt";      // otherwise, craft the file path
    remove(database_path.c_str());                                              // and delete the file

    return 2;                                                                   // return status = 2 (confirmation)
}

void Database::load_objects(){
    string file_path = "../src/files/" + database_name + ".txt"; // craft the file path
    ifstream file(file_path);                                    // and open the file
    
    string line;

    getline(file, line);                         // get the first line
    int pos = line.find('=');                    // find the = 
    database_id = stoi(line.substr(pos + 1));    // and extract the database id    

    getline(file,line);                          // get the next line
    pos = line.find('=');                        // find the =
    next_object_id = stoi(line.substr(pos + 1)); // and extract the database next_object_id

    while(getline(file, line)){                                           // so long as there are still lines to get from the file
    // Temporary object features
    int temp_id = 0;
    string temp_name = "";
    string temp_type = "";
    int temp_quantity = 0;

    stringstream line_extractor(line);                                    // get a line
    line_extractor >> temp_id >> temp_name >> temp_type >> temp_quantity; // and extract the object features

    Object temp_object(temp_id, temp_name, temp_type, temp_quantity);     // then create an object with those features
    database_objects.emplace_back(temp_object);                           // and add it to the database objects vector
    }
}

int Database::add_object(char decorator, bool sound){
    while(true){
        // create the outline box using the preferred decorator, write the menu name and program title
        clear();
        draw_main_box(decorator);
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
        draw_line(decorator);                                        // draw the bottom line
        move(LINES - 2, 3);

        //TODO: ADD RESIZE!!!

        echo();                              // enable echo 
        nocbreak();                          // enable buffering
        char object[256];
        getnstr(object, sizeof(object) - 1); // get user input
        if(sound)                            // make ANNOYING sound if sound turned on
            beep();
        noecho();                            // no echo
        cbreak();                            // no buffering
            
        string object_string = object;       // make the input a string  
        if(object[0] == '\0')                // if the user left blank and pressed Enter, go back
            return 5;

        stringstream new_object(object_string);
        vector<string> temp_object;
        string temp_object_feature;

        while(getline(new_object, temp_object_feature, ' '))                                    // so long as there are features to remove from that input line
            temp_object.push_back(temp_object_feature);                                         // extract them into a vector
 
        try{                                                                                    // then try to create an object with them
            string temp_name = temp_object[0];
            string temp_type = temp_object[1];
            int temp_quantity = std::stoi(temp_object[2]);

            database_objects.emplace_back(next_object_id, temp_name, temp_type, temp_quantity); // and add it to the database object vector
            next_object_id++;

            printw("Object added to the database!");                                            // finally tell the user that the object was added to the database (never really shown cuz no getch() to pause)
            return 5;
        }catch(int errorCode){                                                                  // if not possible, tell the user to carefully follow the format
            printw("Invalid object! Please make sure to follow the correct format!"); 
        }
    }
}

bool Database::delete_object(char decorator, bool sound){
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
    draw_line(decorator);                              // draw the bottom line
    refresh();
        if(check_resize())                             // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 1;
    
    move(LINES - 2, 2);
    for(int i = 2; i < COLS - 1; i++)
        printw(" ");

    int choice = getch(); // get user input
    if(sound)             // make ANNOYING sound if sound turned on 
        beep();

    if(choice == 10)      // if the user pressed Enter, go back
        return 0;

    size_t initial_size = database_objects.size(); // get initial vector size for final comparison
    int deleted_count = 0;

    // determine the next step based on user input
    switch(choice){
    case 49:{                                                         // 1 -> delete objects by id
        move(3,3);
        printw("Delete by ID");
        move(1, COLS - 28);
        printw("Terminal Database Manager");

        for(int i = 5; i < 9; i++)
            for(int j = 3; j < 15; j++){
                move(i, j);
                printw(" ");
            }
        
        move(10, 3);
        printw("WARNING!!!");
        move(11, 3);
        printw("All matching objects will be deleted!!!");
        move(13, 3);        
        printw("Leave blank and press Enter to cancel and go back.");
        draw_line(decorator);                                         // draw the bottom line
        move(LINES - 2, 3);
        refresh();
        if(check_resize())                                            // check if the window was resized by the user and re-enter the menu to re-draw everything
            return 1;

        echo();                                                       // echo on
        nocbreak();                                                   // buffering on
        char id_to_delete[10];
        getnstr(id_to_delete, sizeof(id_to_delete) - 1);              // get user input
        if(sound)                                                     // make ANNOYING sound if sound turned on   
            beep();
        noecho();                                                     // no echo
        cbreak();                                                     // no buffering

        if(id_to_delete[0] == '\0')                                   // if the user left blank and pressed Enter, go back
            return 0;

        string id_to_delete_string = id_to_delete;                    // convert user id to string
        struct IsIdEqual{                                             // create a structure to easily check if the id from the user matches the current object id being checked
                int id_to_match;
                explicit IsIdEqual(int id) : id_to_match(id) {} 

                bool operator()(const Object& obj) const {
                    return obj.get_id() == id_to_match;
                }
            };

        // check if the id from the user matches any id from the vector using that structure from above
        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsIdEqual(stoi(id_to_delete_string))), database_objects.end());

        deleted_count = initial_size - database_objects.size(); // get deleted count (SHOULD ALWAYS BE 1 FOR DELETE BY ID, OTHERWISE SOMETHING WENT WRONG!!!)
        if (deleted_count > 0){
            move(LINES - 2, 3);
            printw("%d object(s) with ID %s deleted.", deleted_count, id_to_delete_string.c_str());
        }else{
            move(LINES - 2, 3);
            printw("No object found with ID %s.", id_to_delete_string.c_str());        
        }
        break;
    }
    case 50:{                                                         // 2 -> delete objects by name
        move(1,3);
        printw("Delete by Name");
        move(1, COLS - 28);
        printw("Terminal Database Manager");

        for(int i = 5; i < 9; i++)
            for(int j = 3; j < 15; j++){
                move(i, j);
                printw(" ");
            }
        
        move(10, 3);
        printw("WARNING!!!");
        move(11, 3);
        printw("All matching objects will be deleted!!!");
        move(13, 3);        
        printw("Leave blank and press Enter to cancel and go back.");
        draw_line(decorator);
        move(LINES - 2, 3);
        refresh();
        if(check_resize())
            return 1;

        echo();                                                       // echo on
        nocbreak();                                                   // buffering on
        char name_to_delete[256];
        getnstr(name_to_delete, sizeof(name_to_delete) - 1);          // get user input
        if(sound)                                                     // make ANNOYING sound if sound turned on   
            beep();
        noecho();                                                     // no echo
        cbreak();                                                     // no buffering

        string name_to_delete_string = name_to_delete;                // convert name to string
        struct IsNameEqual {                                          // create a structure to easily check if the name from the user matches the current object name being checked
            string name_to_match;
            explicit IsNameEqual(const string& name) : name_to_match(name) {}
                
            bool operator()(const Object& obj) const {
                return obj.get_name() == name_to_match;
            }
        };
        // check if the name from the user matches any name from the vector using that structure from above
        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsNameEqual(name_to_delete_string)), database_objects.end());

        deleted_count = initial_size - database_objects.size(); // get deleted count
        if (deleted_count > 0){
            move(LINES - 2, 3);
            printw("%d object(s) with Name %s deleted.", deleted_count, name_to_delete_string.c_str());
        }else{
            move(LINES - 2, 3);
            printw("No object found with Name %s.", name_to_delete_string.c_str());    
        }
        break;
    }
    case 51:{                                                         // 3 -> delete objects by type
        move(1,3);
        printw("Delete by Type");
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        
        for(int i = 5; i < 9; i++)
            for(int j = 3; j < 15; j++){
                move(i, j);
                printw(" ");
            }
        
        move(10, 3);
        printw("WARNING!!!");
        move(11, 3);
        printw("All matching objects will be deleted!!!");

        move(13, 3);        
        printw("Leave blank and press Enter to cancel and go back.");
        draw_line(decorator);
        move(LINES - 2, 3);
        refresh();
        if(check_resize())
            return 1;

        echo();                                                       // echo on
        nocbreak();                                                   // buffering on
        char type_to_delete[256];
        getnstr(type_to_delete, sizeof(type_to_delete) - 1);          // get user input
        if(sound)                                                     // make ANNOYING sound if sound turned on   
            beep();
        noecho();                                                     // no echo
        cbreak();                                                     // no buffering

        string type_to_delete_string = type_to_delete;                // convert type to string
        struct IsTypeEqual {                                          // create a structure to easily check if the type from the user matches the current object type being checked
            string type_to_match;
            explicit IsTypeEqual(const string& type) : type_to_match(type) {}
                
            bool operator()(const Object& obj) const {
                return obj.get_type() == type_to_match;
            }
        };
        // check if the type from the user matches any type from the vector using that structure from above
        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsTypeEqual(type_to_delete_string)), database_objects.end());

        deleted_count = initial_size - database_objects.size(); // get deleted count
        if (deleted_count > 0){
            move(LINES - 2, 3);
            printw("%d object(s) with Type %s deleted.", deleted_count, type_to_delete_string.c_str());
        }else{
            move(LINES - 2, 3);
            printw("No object found with Type %s.", type_to_delete_string.c_str()); 
        }
        break;
    }
    default:{     // re-enter menu
        return 1;
    }
    }

    return 0; // return status 0 (finished)
}
