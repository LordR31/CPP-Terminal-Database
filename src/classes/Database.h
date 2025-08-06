// Database.h
#ifndef DATABASE_H 
#define DATABASE_H

#include "Object.h"
#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>  // Required for std::remove_if, std::find_if
#include <sstream>
#include <vector>
#include <fstream>
#include <curses.h>
#include <fstream>

class Database{
public:
    Database();
    Database(int id, std::string name);                   // Database constructor for "create_database()"
    ~Database();
    int get_database_id();
    std::string get_database_name();
    
    void load_database(std::string database_identifiers); // Loads database identifiers & objects from file
    void print_database();                           // Prints the database
    void save_database(); // ADD TO NOTES!           // Saved database to file
    void delete_database();                          // Deletes the database & file

    void add_object();                               // Adds object to database
    void delete_object();                            // Deletes objects from database
    
private: 
    void load_objects();                             // Loads objects from file. Called in "load_database()"
    std::string save_objects_to_file(int index);

    int database_id = 0;
    std::string database_name;
    std::vector<Object> database_objects;
    int next_object_id = 0;
};

#endif // DATABASE