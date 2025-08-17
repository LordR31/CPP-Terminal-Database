// Database.cpp
#include "Database.hpp" // Include the corresponding header file

using namespace std;

Database::Database(){}

Database::Database(const int& id, const string& name): database_id(id), database_name(name){}

int Database::get_database_id(){
    return database_id;
}

const string& Database::get_database_name() const{
    return database_name;
}

int Database::get_number_of_objects(){
    return database_objects.size();
}

std::vector<Object> Database::get_database_objects(){
    return database_objects;
}

int Database::get_next_object_id(){
    return next_object_id;
}

string Database::get_object_by_id(int object_id){
    string temp_string;
    for(int i = 0; i < database_objects.size(); i++)
        if(database_objects[i].get_id() == object_id)
            temp_string = to_string(database_objects[i].get_id()) + " " + database_objects[i].get_name() + " " + database_objects[i].get_type() + " " + to_string(database_objects[i].get_quantity());
    return temp_string;
}

void Database::set_database_name(const string& new_name){
    database_name = new_name;
}

void Database::load_database(const string& database_identifiers){ 
    int pos = database_identifiers.find('.');                // find the . separating id from name
    database_id = stoi(database_identifiers.substr(0, pos)); // extract id
    database_name = database_identifiers.substr(pos + 1);    // extract name
    load_objects();                                          // call the function for loading objects from file
}

void Database::save_database(){
    string file = "files/" + database_name + ".txt";            // craft the path to the file
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

bool Database::delete_database(){
    string database_path = "files/" + get_database_name() + ".txt";      // otherwise, craft the file path
    remove(database_path.c_str());                                              // and delete the file
    if(filesystem::exists(database_path))
        return false;
    return true;                                                                // return true (confirmation)
}

void Database::load_objects(){
    string file_path = "files/" + database_name + ".txt";   // craft the file path
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

bool Database::add_object(const string& name, const string& type, int quantity){
    if(name.empty())
        return false;
    if(type.empty())
        return false;
    
    Object new_object(next_object_id, name, type, quantity);
    next_object_id++;
    database_objects.emplace_back(new_object);
    return true; // Confirmation
}

bool Database::delete_object(int id_to_find){
        int initial_size = database_objects.size();
        // check if the id from the user matches any id from the vector using that structure from above
        for (auto i = database_objects.begin(); i != database_objects.end();)
            if(i->get_id() == id_to_find)
                i = database_objects.erase(i);
            else
                i++;
    
        int final_size = database_objects.size();
        if(initial_size == final_size)
            return false;

        return true;
}

bool Database::delete_object(const string& string_to_find){
    string deletion_type;
    size_t position = string_to_find.find(".");
    deletion_type = string_to_find.substr(0, position);
    
    string temp_string_to_find = string_to_find.substr(position + 1);

    int initial_size = database_objects.size();
    switch (stoi(deletion_type)) {
        case 0:
            for (auto i = database_objects.begin(); i != database_objects.end();) {
                if (i->get_name() == temp_string_to_find) 
                    i = database_objects.erase(i);
                else
                    i++;
            }
            break;
        case 1:
            for (auto i = database_objects.begin(); i != database_objects.end();) {
                if (i->get_type() == temp_string_to_find)
                    i = database_objects.erase(i);
                else 
                    i++;
            }
            break;
    }
    int final_size = database_objects.size();

    return initial_size != final_size;
}

bool Database::operator<(const Database& database_object){
        return database_id < database_object.database_id;
}

vector<Object> Database::find_object_by_id(const string& match_word){
    vector<Object> temp_vector;
    for (auto& object : database_objects) {
        string object_id = to_string(object.get_id());
        if (object_id.rfind(match_word, 0) == 0)
            temp_vector.push_back(object);
    }

    return temp_vector;
}

vector<Object> Database::find_object_by_name(const string& match_word){
    vector<Object> temp_vector;

    for (auto& object : database_objects) {
        string object_name = object.get_name();
        if (object_name.rfind(match_word, 0) == 0)
            temp_vector.push_back(object);
    }

    return temp_vector;
}

vector<Object> Database::find_object_by_type(const string& match_word){
    vector<Object> temp_vector;

    for (auto& object : database_objects) {
        string object_type = object.get_type();
        if (object_type.rfind(match_word, 0) == 0)
            temp_vector.push_back(object);
    }

    return temp_vector;
}

vector<Object> Database::find_object_by_quantity(const string& match_word){
    vector<Object> temp_vector;

    for (auto& object : database_objects) {
        string object_quantity = to_string(object.get_quantity());
        if (object_quantity.rfind(match_word, 0) == 0)
            temp_vector.push_back(object);
    }

    return temp_vector;
}

void Database::edit_object(int item_id, const std::string& new_name, const std::string& new_type, int new_quantity, bool is_name, bool is_type, bool is_quantity){
    for(int i = 0; i < static_cast<int>(database_objects.size()); i++)
        if(database_objects[i].get_id() == item_id){
                if(is_name)
                    database_objects[i].set_object_name(new_name);
                if(is_type)
                    database_objects[i].set_object_type(new_type);
                if(is_quantity)
                    database_objects[i].set_object_quantity(new_quantity);
        }
}