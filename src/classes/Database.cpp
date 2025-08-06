// Database.cpp
#include "Database.h" // Include the corresponding header file

using namespace std;

Database::Database(){}

Database::Database(int id, string name): database_id(id), database_name(name){
    printw("Database initialized and ready.\n");
}

Database::~Database(){}

int Database::get_database_id(){
    return database_id;
}

string Database::get_database_name(){
    return database_name;
}

void Database::load_database(string database_identifiers){
    int pos = database_identifiers.find('.');
    database_id = stoi(database_identifiers.substr(0, pos));
    database_name = database_identifiers.substr(pos + 1);
    load_objects();
}

void Database::print_database(){
    while(true){
        clear();
        if (database_objects.empty())
            printw("\nDatabase is empty. No objects to display.\n");
        else{
            printw("\n--- Current Database Objects ---\n");
            for (const auto& obj : database_objects)
                printw("ID: %d,\tName: %s,\tType: %s,\tQuantity: %d\n",obj.get_id(), obj.get_name().c_str(), obj.get_type().c_str(), obj.get_quantity());
        }
        printw("--------------------------------\n");
        printw("1 Previous Page\t\t2 - Add item\t\t3 - Delete item\t\t4 - Quit\t\t5 - Next Page");  
        int choice = getch();
            switch (choice){
                case 49: 
                    break;
                case 50: 
                    add_object();
                break;
                case 51:
                    delete_object();
                    break;
                case 52:
                    return;
                    break;
                case 53:
                    break;
                default:
                    break;
            }
    }
}

void Database::save_database(){
    string file = "../src/files/" + get_database_name() + ".txt";
    ofstream output(file);
    
    output << "Database_id=" + to_string(database_id) + '\n';
    output << "Next_object_id=" + to_string(next_object_id) + '\n';
    for(int i = 0; i < database_objects.size(); i++)
        output << save_objects_to_file(i);
    output.close();
}

string Database::save_objects_to_file(int index){
    string object_in_string_format;
    object_in_string_format = to_string(database_objects[index].get_id()) + ' ' + database_objects[index].get_name() + ' ' + database_objects[index].get_type() + ' ' + to_string(database_objects[index].get_quantity()) + '\n';
    return object_in_string_format;
}

void Database::delete_database(){
    clear();
    printw("WARNING!!\n Deleting a database is PERMANENT!\n");
    printw("Type DELETE to confirm deleting the database!\n");
    printw("Press Esc to cancel\n");
    printw("----------------------------------\n\n");

    echo();
    nocbreak();
    char confirmation[6];
    getnstr(confirmation, sizeof(confirmation));
    noecho();
    cbreak();

    if(strcmp(confirmation, "DELETE") != 0){
        printw("Invalid Input! Do you want to try again? (Y / N): ");
        int input = getch();

        if(input == 121 || input == 89){ // lower and upper case y in ASCII
            clear();
            printw("WARNING!!\n Deleting a database is PERMANENT!\n");
            printw("Type DELETE to confirm deleting the database!\n");
            printw("Press Esc to cancel\n");
            printw("----------------------------------\n\n");

            echo();
            nocbreak();
            char confirmation[6];
            getnstr(confirmation, sizeof(confirmation));
            noecho();
            cbreak();

             if(strcmp(confirmation, "DELETE") != 0){
                printw("There have been too many failed attempts to delete the database!");
                return;
            }
        }else if(input == 'n')
            return;
    }
    string database_path = "../src/files/" + get_database_name() + ".txt";
    remove(database_path.c_str());
    // delete this;
}

void Database::load_objects(){
    string file_path = "../src/files/" + database_name + ".txt";
    ifstream file(file_path);
    
    string line;
    stringstream object_stream(line);
    vector<string> temp_object;
    string temp_object_feature;

    // get database id
    getline(file, line);
    int pos = line.find('=');
    database_id = stoi(line.substr(pos + 1));    

    // get next object id
    getline(file,line);
    pos = line.find('=');
    next_object_id = stoi(line.substr(pos + 1)); 

    while(getline(file, line)){      
        int temp_id;
        string temp_name;
        string temp_type;
        int temp_quantity;

		char *token;
		char *line_char = new char[line.length() + 1];
        strcpy(line_char, line.c_str());

        token = strtok(line_char, " ");
		if (token != NULL)
            temp_id = atoi(token);

		token = strtok(NULL, " ");
		if (token != NULL)
            temp_name = token;

		token = strtok(NULL, " ");
		if (token != NULL)
            temp_type = token;

		token = strtok(NULL, " ");
		if (token != NULL)
            temp_quantity = atoi(token);

        Object temp_object(temp_id, temp_name, temp_type, temp_quantity);
        database_objects.emplace_back(temp_object);
    }
}

void Database::add_object(){
    while(true){
        clear();
        printw( "Objects are created using this format:\n");
        printw( "\t\t**********************\n");
        printw( "\t\t* name type quantity *\n");
        printw( "\t\t**********************\n");
        printw("\nPress Enter to confirm");
        printw("\nPress Esc to cancel\n");
        printw("----------------------------------------\n");

        echo();
        nocbreak();
        char object[256];
        getnstr(object, sizeof(object) - 1);
        string object_string = object;
        noecho();
        cbreak();

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
            return;
        }catch(int errorCode){
            printw("Invalid object! Please make sure to follow the correct format!");
        }
    }
}

void Database::delete_object(){
    clear();
    printw("\nDelete by:\n");
    printw("1. ID\n");
    printw("2. Name\n");
    printw("3. Type\n");
    printw("\n\n0. Cancel\n");
    printw("\nWARNING\n All objects matching will be deleted!!!\n\n");

    int choice = getch();

    size_t initial_size = database_objects.size(); // get initial size for comparison
    int deleted_count = 0;

    switch(choice){
    case 49:{
        clear();
        printw("\n\nDelete by ID\n");
        printw("\n\nWARNING\n All objects matching will be deleted!!!\n\n");
        printw("Press Enter to confirm\n");
        printw("Press Esc to cancel\n");
        printw("------------------------------------------------------------\n");

        echo();
        nocbreak();
        char id_to_delete[10];
        getnstr(id_to_delete, sizeof(id_to_delete) - 1);
        noecho();
        cbreak();

        string id_to_delete_string = id_to_delete;
        
        // struct to check if object id matches user choice
        struct IsIdEqual{
                int id_to_match;
                IsIdEqual(int id) : id_to_match(id) {} 

                bool operator()(const Object& obj) const {
                    return obj.get_id() == id_to_match;
                }
            };

        // remove elements with remove_if
        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsIdEqual(stoi(id_to_delete_string))),
                                         database_objects.end());

        deleted_count = initial_size - database_objects.size();
            if (deleted_count > 0)
                printw("%d object(s) with ID %s deleted.\n", deleted_count, id_to_delete_string);
            else
                printw("No object found with ID %s.\n", id_to_delete_string);        
        break;
    }
    case 50:{
        clear();
        printw("\n\nDelete by Name\n");
        printw("\n\nWARNING\n All objects matching will be deleted!!!\n\n");
        printw("Press Enter to confirm\n");
        printw("Press Esc to cancel\n");
        printw("------------------------------------------------------------\n");

        echo();
        nocbreak();
        char name_to_delete[256];
        getnstr(name_to_delete, sizeof(name_to_delete) - 1);
        noecho();
        cbreak();

        string name_to_delete_string = name_to_delete;

        struct IsNameEqual {
            string name_to_match;
            IsNameEqual(const string& name) : name_to_match(name) {}
                
            bool operator()(const Object& obj) const {
                return obj.get_name() == name_to_match;
            }
        };

        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsNameEqual(name_to_delete_string)),
                                         database_objects.end());

        deleted_count = initial_size - database_objects.size();
        if (deleted_count > 0)
            printw("%d object(s) with Name %s deleted.\n", deleted_count, name_to_delete_string);
        else
            printw("No object found with Name %s.\n", name_to_delete_string);    
        break;
    }
    case 51:{
        clear();
        printw("\n\nDelete by Type\n");
        printw("\n\nWARNING\n All objects matching will be deleted!!!\n\n");
        printw("Press Enter to confirm\n");
        printw("Press Esc to cancel\n");
        printw("------------------------------------------------------------\n");

        echo();
        nocbreak();
        char type_to_delete[256];
        getnstr(type_to_delete, sizeof(type_to_delete) - 1);
        noecho();
        cbreak();

        string type_to_delete_string = type_to_delete;

        struct IsTypeEqual {
            string type_to_match;
            IsTypeEqual(const string& type) : type_to_match(type) {}
                
            bool operator()(const Object& obj) const {
                return obj.get_type() == type_to_match;
            }
        };

        database_objects.erase(remove_if(database_objects.begin(), database_objects.end(), IsTypeEqual(type_to_delete_string)),
                                         database_objects.end());

        deleted_count = initial_size - database_objects.size();
        if (deleted_count > 0)
            printw("%d object(s) with Type %s deleted.\n", deleted_count, type_to_delete_string);
        else
            printw("No object found with Type %s.\n", type_to_delete_string); 
        break;
    }
    default:
        printw("Invalid choice. Please enter 1, 2 or 3.\n");
        break;
    }
}
