#include "gtest/gtest.h"
#include "../src/classes/Database.h"
#include "../src/classes/Object.h"
#include <fstream>
#include <filesystem>

using namespace std;

// create test fixture class
class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        filesystem::create_directory("files");   // create the files folder

        test_database_filename = "files/test_database.txt"; // create a database file
        ofstream output(test_database_filename);
        output << "Database_id=1\n";
        output << "Next_object_id=0";
        output.close();
        database = new Database(1, "test_database");        // create a database
    }

    void TearDown() override {
        delete database; // delete the database

        if (filesystem::exists(test_database_filename)) // if the database file exists
            filesystem::remove(test_database_filename); // delete it 
    }

    // DATABASE OBJECT AND VARIABLES
    Database* database;
    string test_database_filename;
};

// test for main constructor and getters
TEST_F(DatabaseTest, test_constructor_and_getters) {
    ASSERT_EQ(database->get_database_id(), 1);                 // check id                 SHOULD BE (1)
    ASSERT_EQ(database->get_database_name(), "test_database"); // check name               SHOULD BE (test_database)
    ASSERT_EQ(database->get_number_of_objects(), 0);           // check number of objects  SHOULD BE (0)
}

// test for the empty constructor
TEST_F(DatabaseTest, test_empty_constructor_and_getters) {
    Database empty_database;

    ASSERT_EQ(empty_database.get_database_id(), 0);       // check id                      SHOULD BE (0)
    ASSERT_EQ(empty_database.get_database_name(), "");    // check name                    SHOULD BE (*nothing*)  
    ASSERT_EQ(empty_database.get_number_of_objects(), 0); // check number of objects       SHOULD BE (0)      
}

// test add object -> save database -> load database
TEST_F(DatabaseTest, test_add_save_load_database) {
    string database_name = database->get_database_name() + ".txt";

    // add objects to database
    ASSERT_TRUE(database->add_object("Apple", "Fruit", 10));
    ASSERT_TRUE(database->add_object("Laptop", "Electronics", 2));
    ASSERT_TRUE(database->add_object("Desk", "Furniture", 1));
    
    int expected_objects = 3;

    // check the elements were added 
    ASSERT_EQ(database->get_number_of_objects(), expected_objects);                                   // use the getter and compare to the expected number of elements
    
    // check the database was saved
    database->save_database();                                                                        // use the save method
    ASSERT_TRUE(std::filesystem::exists("files/" + database_name));                                   // and check that it created the database file

    // check the database loading
    Database loaded_database;                                                                         // create an empty database
    string identifier = to_string(database->get_database_id()) + "." + database->get_database_name(); // create the database identifiers
    loaded_database.load_database(identifier);                                                        // and load the database
    ASSERT_EQ(loaded_database.get_number_of_objects(), expected_objects);                             // then check the number of elements to see if it loaded correctly

    std::vector<Object> loaded_objects = loaded_database.get_database_objects();                      // and finally get the object vector from the database and check the elements
    ASSERT_EQ(loaded_objects[0].get_name(), "Apple");
    ASSERT_EQ(loaded_objects[1].get_type(), "Electronics");
    ASSERT_EQ(loaded_objects[2].get_quantity(), 1);
}

// test delete_objcet()
TEST_F(DatabaseTest, test_delete_object) {
    database->add_object("Apple", "Fruit", 10);                             // add objects to the database
    database->add_object("Laptop", "Electronics", 2);
    database->add_object("Desk", "Furniture", 1);
    
    ASSERT_EQ(database->get_number_of_objects(), 3);                        // check the number of elements
    bool delete_success = database->delete_object(1);                       // then delete an element

    ASSERT_TRUE(delete_success);                                            // check that it was deleted
    ASSERT_EQ(database->get_number_of_objects(), 2);                        // but also check the number of elements (to make sure)
    
    bool delete_failure = database->delete_object(999);                     // check delete_object(int) with a wrong id
    ASSERT_FALSE(delete_failure);                                           // check if the function returned false
    ASSERT_EQ(database->get_number_of_objects(), 2);                        // make sure nothing was deleted

    delete_failure = database->delete_object("0.this_should_return_false"); // check delete_object(string) with a wrong_string
    ASSERT_FALSE(delete_failure);                                           // check if the function returned false
    ASSERT_EQ(database->get_number_of_objects(), 2);                        // make sure nothing was deleted
}

// test delete_database();
TEST_F(DatabaseTest, test_delete_database) {
    database->save_database();                                     // make sure the database file exists (use save_database())
    ASSERT_TRUE(std::filesystem::exists(test_database_filename));  // check that the file was created

    bool return_value = database->delete_database();               // call delete_database()
    ASSERT_TRUE(return_value);                                     // check the response
    ASSERT_FALSE(std::filesystem::exists(test_database_filename)); // check if the file still exists
}
