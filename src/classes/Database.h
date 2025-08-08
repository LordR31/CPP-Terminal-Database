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
#include "../utils.h"

class Database{
public:
    Database();
    Database(const int& id, const std::string& name);  // Database constructor for "create_database()"
    
               int     get_database_id();
    const std::string& get_database_name();
               int     get_number_of_objects();
    
    void load_database(const std::string& database_identifiers); // Loads database identifiers & objects from file
    int print_database(char decorator, int number_of_entries, int page_number, bool sound);                           // Prints the database
    void save_database(); // ADD TO NOTES!           // Saved database to file
    int delete_database(char decorator, bool sound);                          // Deletes the database & file

    int add_object(char decorator, bool sound);                               // Adds object to database
    bool delete_object(char decorator, bool sound);                            // Deletes objects from database
    
private: 
         void   load_objects();                  // Loads objects from file. Called in "load_database()"
    std::string save_objects_to_file(int index);

         int            database_id = 0;
    std::string         database_name; // cppcheck-suppress unusedStructMember
    std::vector<Object> database_objects;
         int            next_object_id = 0;
};

#endif // DATABASE