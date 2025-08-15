// Database.h
#ifndef DATABASE_H 
#define DATABASE_H

#include "Object.hpp"
#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>  // for remove_if and find_if
#include <sstream>
#include <vector>
#include <fstream>
#include <fstream>
#include <filesystem>

class Database{
public:
    Database();                                                        // Empty constructor
    Database(const int& id, const std::string& name);                  // Database constructor for "create_database()"
    
    int get_database_id();
    const std::string& get_database_name();
    int get_number_of_objects();
    std::vector<Object> get_database_objects();                        // Prints the database
    int get_next_object_id();
    std::string get_object_by_id(int object_id);

    void set_database_name(const std::string& new_name);

    void load_database(const std::string& database_identifiers);       // Loads database identifiers & objects from file
    void save_database();                                              // Saved database to file
    bool delete_database();                                            // Deletes the database & file

    bool add_object(const std::string& name, const std::string& type, int quantity); // Adds object to database
    bool delete_object(int id_to_find);                                // Deletes objects from database
    bool delete_object(const std::string& string_to_find);
    
    std::vector<Object> find_object_by_id(const std::string& match_word);
    std::vector<Object> find_object_by_name(const std::string& match_word);
    std::vector<Object> find_object_by_type(const std::string& match_word);
    std::vector<Object> find_object_by_quantity(const std::string& match_word);

    void edit_object(int item_id, const std::string& new_name, const std::string& new_type, int new_quantity);
    void edit_object(int item_id, const std::string& new_name, const std::string& new_type);
    void edit_object(int item_id, const std::string& new_name);
    
    bool operator<(const Database& database_object);
private: 
    void load_objects();                                               // Loads objects from file. Called in "load_database()"
    std::string save_objects_to_file(int index);                       // Saves the database objects to the database file

    int database_id = 0;                                               // "unique" database id
                                                                       // database name
    std::string database_name;                                         // cppcheck-suppress unusedStructMember
    std::vector<Object> database_objects;                              // vector with all the database objects
    int next_object_id = 0;                                            // the index used when creating a new object and adding it to the database
};

#endif // DATABASE