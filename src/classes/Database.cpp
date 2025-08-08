// Database.cpp
#include "Database.h" // Include the corresponding header file

using namespace std;

Database::Database(){}

Database::Database(const int& id, const string& name): database_id(id), database_name(name){
    // printw("Database initialized and ready.\n");
}

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
    int pos = database_identifiers.find('.');
    database_id = stoi(database_identifiers.substr(0, pos));
    database_name = database_identifiers.substr(pos + 1);
    load_objects();
}

int Database::print_database(char decorator, int number_of_entries, int page_number, bool sound){
    while(true){
        clear();
        draw_main_box(decorator);
        move(1, 3);
        printw("%s", database_name.c_str());
        move(1, COLS - 28);
        printw("Terminal Database Manager");
        draw_line(decorator);
        refresh();
        if(check_resize())
            return 1;

        bool is_empty = false;
        bool is_paged = false;
        if (database_objects.empty()){
            move(8, COLS / 2 - 25);
            printw("Database is empty. No objects to display.");
            is_empty = true;
        }else{
            if(database_objects.size() > number_of_entries)
                is_paged = true;

            move(LINES - 4 - number_of_entries - 1, 3);
            printw("ID");
            move(LINES - 4 - number_of_entries - 1, 8);
            printw("Name");
            move(LINES - 4 - number_of_entries - 1, COLS - 10 - 50);
            printw("Type");
            move(LINES - 4 - number_of_entries - 1, COLS - 10);
            printw("Quantity");

            string page_number_string = "Page " + to_string(page_number);
            move(LINES / 2, COLS / 2 - page_number_string.length() / 2);
            printw("%s", page_number_string.c_str());

            for(int i = 0; i < number_of_entries; i++){
                if(i + page_number * number_of_entries < database_objects.size()){
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

        if(is_paged){
            move(LINES - 2, 2);
            printw("1 - Previous Page");
            move(LINES - 2, COLS - 15);
            printw("5 - Next Page");
        }
        if(!is_empty){
            move(LINES - 2, COLS - 15 - 14 - 8 - 15);
            printw("3 - Delete item");
        }

        move(LINES - 2, COLS - 15 - 21 - 8 - 15 - 15);
        printw("2 - Add item");
        
        move(LINES - 2, COLS - 15 - 7 - 8);
        printw("4 - Back");
        
        int choice = getch();
        if(sound)
            beep();
            switch (choice){
                case 49: {
                    if(is_paged)
                        return 1;
                    else
                        continue;
                }
                case 50:{
                    add_object(decorator, sound);
                    continue;
                }
                case 51:{
                    bool status = delete_object(decorator, sound);
                    while(status)
                        status = delete_object(decorator, sound);
                    continue;
                }
                case 52:{
                    return 0;
                }
                case 53:{
                    if(is_paged)                    
                        return 2;
                    else
                        continue;
                }
                default:{
                    continue;
                }    
            }
        return 0;
    }
}

void Database::save_database(){
    string file = "../src/files/" + database_name + ".txt";
    ofstream output(file);
    
    output << "Database_id=" + to_string(database_id) + '\n';
    output << "Next_object_id=" + to_string(next_object_id) + '\n';
    for(int i = 0; i < static_cast<int>(database_objects.size()); i++)
        output << save_objects_to_file(i);
    output.close();
}

string Database::save_objects_to_file(int index){
    string object_in_string_format;
    object_in_string_format = to_string(database_objects[index].get_id()) + ' ' + database_objects[index].get_name() + ' ' + database_objects[index].get_type() + ' ' + to_string(database_objects[index].get_quantity()) + '\n';
    return object_in_string_format;
}

int Database::delete_database(char decorator, bool sound){
    clear();
    draw_main_box(decorator);
    move(5, COLS / 2 - 21);
    printw("WARNING!! Deleting a database is PERMANENT!");
    move(7, COLS / 2 - 22);
    printw("Type DELETE to confirm deleting the database!");
    move(8, COLS / 2 - 19);
    printw("Leave blank and press Enter to cancel!");
    draw_line(decorator);
    move(LINES - 2, 3);

    echo();
    nocbreak();
    char confirmation[6];
    getnstr(confirmation, sizeof(confirmation));
    if(sound)
        beep();
    noecho();
    cbreak();

    if(confirmation[0] == '\0')
        return 0;

    if(strcmp(confirmation, "DELETE") != 0){
        move(LINES - 2, 3);
        printw("Invalid Input! Do you want to try again? (Y / N): ");
        int input = getch();
        if(sound)
            beep();

        if(input == 121 || input == 89) // lower and upper case y in ASCII
            return 1;
        else if(input == 110 || input == 78)
            return 0;
    }
    string database_path = "../src/files/" + get_database_name() + ".txt";
    remove(database_path.c_str());

    return 2;
}

void Database::load_objects(){
    string file_path = "../src/files/" + database_name + ".txt";
    ifstream file(file_path);
    
    string line;

    // get database id
    getline(file, line);
    int pos = line.find('=');
    database_id = stoi(line.substr(pos + 1));    

    // get next object id
    getline(file,line);
    pos = line.find('=');
    next_object_id = stoi(line.substr(pos + 1)); 

    while(getline(file, line)){
    int temp_id = 0;
    string temp_name = "";
    string temp_type = "";
    int temp_quantity = 0;

    stringstream line_extractor(line);
    line_extractor >> temp_id >> temp_name >> temp_type >> temp_quantity;

    Object temp_object(temp_id, temp_name, temp_type, temp_quantity);
    database_objects.emplace_back(temp_object);
}
}

int Database::add_object(char decorator, bool sound){
    while(true){
        clear();
        draw_main_box(decorator);
        move(1,3);
        printw("Add Object");
        move(1, COLS - 28);
        printw("Terminal Database Manager");

        move(LINES / 2 - 4, COLS / 2 - 23);
        printw( "Objects are created using the following format:");
        move(LINES / 2 - 5, COLS - 30);
        printw( "**********************");
        move(LINES / 2 - 4, COLS - 28);
        printw( "name type quantity");
        move(LINES / 2 - 3, COLS - 30);
        printw( "**********************");
        move(LINES - 4, 3);
        printw("Leave blank and press Enter to cancel and go back");
        draw_line(decorator);
        move(LINES - 2, 3);

        echo();
        nocbreak();
        char object[256];
        getnstr(object, sizeof(object) - 1);
        if(sound)
            beep();
        string object_string = object;
        noecho();
        cbreak();

        if(object[0] == '\0')
            return 5;

        stringstream new_object(object_string);
        vector<string> temp_object;
        string temp_object_feature;

        while(getline(new_object, temp_object_feature, ' '))
            temp_object.push_back(temp_object_feature);

        try{
            string temp_name = temp_object[0];
            string temp_type = temp_object[1];
            int temp_quantity = std::stoi(temp_object[2]);

            database_objects.emplace_back(next_object_id, temp_name, temp_type, temp_quantity);
            next_object_id++;

            printw("Object added to the database!");
            return 5;
        }catch(int errorCode){
            printw("Invalid object! Please make sure to follow the correct format!");
        }
    }
}

bool Database::delete_object(char decorator, bool sound){
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
    draw_line(decorator);
    refresh();
        if(check_resize())
            return 1;
    
    move(LINES - 2, 2);
    for(int i = 2; i < COLS - 1; i++)
        printw(" ");

    int choice = getch();
    if(sound)
        beep();

    if(choice == 10)
        return 0;

    size_t initial_size = database_objects.size(); // get initial size for comparison
    int deleted_count = 0;

    switch(choice){
    case 49:{
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
        draw_line(decorator);
        move(LINES - 2, 3);
        refresh();
        if(check_resize())
            return 1;

        echo();
        nocbreak();
        char id_to_delete[10];
        getnstr(id_to_delete, sizeof(id_to_delete) - 1);
        if(sound)
            beep();
        noecho();
        cbreak();

        if(id_to_delete[0] == '\0')
            return 0;

        string id_to_delete_string = id_to_delete;
        struct IsIdEqual{
                int id_to_match;
                explicit IsIdEqual(int id) : id_to_match(id) {} 

                bool operator()(const Object& obj) const {
                    return obj.get_id() == id_to_match;
                }
            };

        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsIdEqual(stoi(id_to_delete_string))),
                                         database_objects.end());

        deleted_count = initial_size - database_objects.size();
        if (deleted_count > 0){
            move(LINES - 2, 3);
            printw("%d object(s) with ID %s deleted.", deleted_count, id_to_delete_string.c_str());
        }else{
            move(LINES - 2, 3);
            printw("No object found with ID %s.", id_to_delete_string.c_str());        
        }
        break;
    }
    case 50:{
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

        echo();
        nocbreak();
        char name_to_delete[256];
        getnstr(name_to_delete, sizeof(name_to_delete) - 1);
        if(sound)
            beep();
        noecho();
        cbreak();

        string name_to_delete_string = name_to_delete;

        struct IsNameEqual {
            string name_to_match;
            explicit IsNameEqual(const string& name) : name_to_match(name) {}
                
            bool operator()(const Object& obj) const {
                return obj.get_name() == name_to_match;
            }
        };

        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsNameEqual(name_to_delete_string)),
                                         database_objects.end());

        deleted_count = initial_size - database_objects.size();
        if (deleted_count > 0){
            move(LINES - 2, 3);
            printw("%d object(s) with Name %s deleted.", deleted_count, name_to_delete_string.c_str());
        }else{
            move(LINES - 2, 3);
            printw("No object found with Name %s.", name_to_delete_string.c_str());    
        }
        break;
    }
    case 51:{
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

        echo();
        nocbreak();
        char type_to_delete[256];
        getnstr(type_to_delete, sizeof(type_to_delete) - 1);
        if(sound)
            beep();
        noecho();
        cbreak();

        string type_to_delete_string = type_to_delete;

        struct IsTypeEqual {
            string type_to_match;
            explicit IsTypeEqual(const string& type) : type_to_match(type) {}
                
            bool operator()(const Object& obj) const {
                return obj.get_type() == type_to_match;
            }
        };

        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsTypeEqual(type_to_delete_string)),
                                         database_objects.end());

        deleted_count = initial_size - database_objects.size();
        if (deleted_count > 0){
            move(LINES - 2, 3);
            printw("%d object(s) with Type %s deleted.", deleted_count, type_to_delete_string.c_str());
        }else{
            move(LINES - 2, 3);
            printw("No object found with Type %s.", type_to_delete_string.c_str()); 
        }
        break;
    }
    default:{
        return 1;
    }
    }

    return 0;
}
