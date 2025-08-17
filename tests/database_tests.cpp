#include "gtest/gtest.h"
#include "../src/classes/Database.hpp"
#include "../src/classes/Object.hpp"
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
    ASSERT_EQ(database->get_next_object_id(), 0);
}

// test for the empty constructor
TEST_F(DatabaseTest, test_empty_constructor_and_getters) {
    Database empty_database;

    ASSERT_EQ(empty_database.get_database_id(), 0);       // check id                      SHOULD BE (0)
    ASSERT_EQ(empty_database.get_database_name(), "");    // check name                    SHOULD BE (*nothing*)  
    ASSERT_EQ(empty_database.get_number_of_objects(), 0); // check number of objects       SHOULD BE (0)     
    ASSERT_EQ(database->get_next_object_id(), 0); 
}

// test add object -> save database -> load database
TEST_F(DatabaseTest, test_add_save_load_database) {
    string database_name = database->get_database_name() + ".txt";

    // add objects to database
    ASSERT_TRUE(database->add_object("test_item_1", "test_type_1", 1));
    ASSERT_TRUE(database->add_object("test_item_2", "test_type_2", 2));
    ASSERT_TRUE(database->add_object("test_item_3", "test_type_3", 3));
    
    int expected_objects = 3;

    // check the elements were added 
    ASSERT_EQ(database->get_number_of_objects(), expected_objects);                                   // use the getter and compare to the expected number of elements
    
    string test_object = "1 test_item_2 test_type_2 2";
    ASSERT_EQ(database->get_object_by_id(1), test_object);

    // check the database was saved
    database->save_database();                                                                        // use the save method
    ASSERT_TRUE(std::filesystem::exists("files/" + database_name));                                   // and check that it created the database file

    // check the database loading
    Database loaded_database;                                                                         // create an empty database
    string identifier = to_string(database->get_database_id()) + "." + database->get_database_name(); // create the database identifiers
    loaded_database.load_database(identifier);                                                        // and load the database
    ASSERT_EQ(loaded_database.get_number_of_objects(), expected_objects);                             // then check the number of elements to see if it loaded correctly

    vector<Object> loaded_objects = loaded_database.get_database_objects();                      // and finally get the object vector from the database and check the elements
    ASSERT_EQ(loaded_objects[0].get_name(), "test_item_1");
    ASSERT_EQ(loaded_objects[1].get_type(), "test_type_2");
    ASSERT_EQ(loaded_objects[2].get_quantity(), 3);
    ASSERT_EQ(database->get_next_object_id(), 3);
}

TEST_F(DatabaseTest, test_add_object_failure) {
    ASSERT_FALSE(database->add_object("", "empty_type", 10));                // test for empty name
    ASSERT_FALSE(database->add_object("empty_name", "", 10));                // test for empty type
}

// test delete_objcet()
TEST_F(DatabaseTest, test_delete_object) {
    database->add_object("test_item_1", "test_type_1", 1);
    database->add_object("test_item_2", "test_type_2", 2);
    database->add_object("test_item_3", "test_type_3", 3);
    
    ASSERT_EQ(database->get_number_of_objects(), 3);                        // check the number of elements
    bool delete_success = database->delete_object(1);                       // then delete an element by id

    ASSERT_TRUE(delete_success);                                            // check that it was deleted
    ASSERT_EQ(database->get_number_of_objects(), 2);                        // but also check the number of elements (to make sure)
    
    bool delete_failure = database->delete_object(999);                     // check delete_object(int) with a wrong id
    ASSERT_FALSE(delete_failure);                                           // check if the function returned false
    ASSERT_EQ(database->get_number_of_objects(), 2);                        // make sure nothing was deleted

    delete_success = database->delete_object("0.test_item_3");              // check delete by word (for name)
    ASSERT_TRUE(delete_success);                                            

    delete_failure = database->delete_object("0.this_should_return_false"); // check delete_object(string) with a wrong_string
    ASSERT_FALSE(delete_failure);                                           // check if the function returned false
    ASSERT_EQ(database->get_number_of_objects(), 1);                        // make sure nothing was deleted
}

TEST_F(DatabaseTest, test_find_object){
    database->add_object("test_item_1", "test_type_1", 1);
    database->add_object("test_item_2", "test_type_2", 2);
    database->add_object("test_item_3", "test_type_3", 3);

    vector<Object> test_vector = database->find_object_by_id("0");
    ASSERT_EQ(test_vector.size(), 1);

    test_vector.clear();
    test_vector = database->find_object_by_name("test");
    ASSERT_EQ(test_vector.size(), 3);

    test_vector.clear();
    test_vector = database->find_object_by_type("test");
    ASSERT_EQ(test_vector.size(), 3);

    test_vector.clear();
    test_vector = database->find_object_by_quantity("2");
    ASSERT_EQ(test_vector.size(), 1);
}

TEST_F(DatabaseTest, test_get_object_by_invalid_id) {
    database->add_object("test_item_1", "test_type_1", 1);
    
    string result = database->get_object_by_id(999);
    ASSERT_TRUE(result.empty());
}

TEST_F(DatabaseTest, test_find_object_no_match) {
    database->add_object("test_item_1", "test_type_1", 1);

    vector<Object> results_name = database->find_object_by_name("non_existent");
    ASSERT_TRUE(results_name.empty());
    
    vector<Object> results_id = database->find_object_by_id("999");
    ASSERT_TRUE(results_id.empty());

    vector<Object> results_empty = database->find_object_by_name("nothing");
    ASSERT_TRUE(results_empty.empty());
}

TEST_F(DatabaseTest, test_edit_database_and_edit_object){
    ASSERT_TRUE(database->add_object("test_item_1", "test_type_1", 1));
    ASSERT_TRUE(database->add_object("test_item_2", "test_type_2", 2));
    ASSERT_TRUE(database->add_object("test_item_3", "test_type_3", 3));
    ASSERT_TRUE(database->add_object("test_item_4", "test_type_4", 4));
    ASSERT_TRUE(database->add_object("test_item_5", "test_type_5", 5));
    ASSERT_TRUE(database->add_object("test_item_6", "test_type_6", 6));
    ASSERT_TRUE(database->add_object("test_item_7", "test_type_7", 7));

    vector<Object> loaded_objects = database->get_database_objects();

    ASSERT_EQ(loaded_objects[0].get_name(), "test_item_1");
    ASSERT_EQ(loaded_objects[0].get_type(), "test_type_1");
    ASSERT_EQ(loaded_objects[0].get_quantity(), 1);

    ASSERT_EQ(loaded_objects[1].get_name(), "test_item_2");
    ASSERT_EQ(loaded_objects[1].get_type(), "test_type_2");
    ASSERT_EQ(loaded_objects[1].get_quantity(), 2);

    ASSERT_EQ(loaded_objects[2].get_name(), "test_item_3");
    ASSERT_EQ(loaded_objects[2].get_type(), "test_type_3");
    ASSERT_EQ(loaded_objects[2].get_quantity(), 3);

    ASSERT_EQ(loaded_objects[3].get_name(), "test_item_4");
    ASSERT_EQ(loaded_objects[3].get_type(), "test_type_4");
    ASSERT_EQ(loaded_objects[3].get_quantity(), 4);

    ASSERT_EQ(loaded_objects[4].get_name(), "test_item_5");
    ASSERT_EQ(loaded_objects[4].get_type(), "test_type_5");
    ASSERT_EQ(loaded_objects[4].get_quantity(), 5);

    ASSERT_EQ(loaded_objects[5].get_name(), "test_item_6");
    ASSERT_EQ(loaded_objects[5].get_type(), "test_type_6");
    ASSERT_EQ(loaded_objects[5].get_quantity(), 6);

    ASSERT_EQ(loaded_objects[6].get_name(), "test_item_7");
    ASSERT_EQ(loaded_objects[6].get_type(), "test_type_7");
    ASSERT_EQ(loaded_objects[6].get_quantity(), 7);

    // Function call arguments: id, name, type, quantity, is_name, is_type, is_quantity
    database->edit_object(0, "",              "",              101,  0, 0, 1);
    database->edit_object(1, "",              "edited_type_2", '\0', 0, 1, 0);
    database->edit_object(2, "",              "edited_type_3", 103,  0, 1, 1);
    database->edit_object(3, "edited_name_4", "",              '\0', 1, 0, 0);
    database->edit_object(4, "edited_name_5", "",              105,  1, 0, 1);
    database->edit_object(5, "edited_name_6", "edited_type_6", '\0', 1, 1, 0);
    database->edit_object(6, "edited_name_7", "edited_type_7", 107,  1, 1, 1);
    
    loaded_objects.clear();
    loaded_objects = database->get_database_objects();

    ASSERT_EQ(loaded_objects[0].get_name(), "test_item_1");
    ASSERT_EQ(loaded_objects[0].get_type(), "test_type_1");
    ASSERT_EQ(loaded_objects[0].get_quantity(), 101);

    ASSERT_EQ(loaded_objects[1].get_name(), "test_item_2");
    ASSERT_EQ(loaded_objects[1].get_type(), "edited_type_2");
    ASSERT_EQ(loaded_objects[1].get_quantity(), 2);

    ASSERT_EQ(loaded_objects[2].get_name(), "test_item_3");
    ASSERT_EQ(loaded_objects[2].get_type(), "edited_type_3");
    ASSERT_EQ(loaded_objects[2].get_quantity(), 103);

    ASSERT_EQ(loaded_objects[3].get_name(), "edited_name_4");
    ASSERT_EQ(loaded_objects[3].get_type(), "test_type_4");
    ASSERT_EQ(loaded_objects[3].get_quantity(), 4);

    ASSERT_EQ(loaded_objects[4].get_name(), "edited_name_5");
    ASSERT_EQ(loaded_objects[4].get_type(), "test_type_5");
    ASSERT_EQ(loaded_objects[4].get_quantity(), 105);

    ASSERT_EQ(loaded_objects[5].get_name(), "edited_name_6");
    ASSERT_EQ(loaded_objects[5].get_type(), "edited_type_6");
    ASSERT_EQ(loaded_objects[5].get_quantity(), 6);

    ASSERT_EQ(loaded_objects[6].get_name(), "edited_name_7");
    ASSERT_EQ(loaded_objects[6].get_type(), "edited_type_7");
    ASSERT_EQ(loaded_objects[6].get_quantity(), 107);

    ASSERT_EQ(database->get_database_name(), "test_database");
    database->set_database_name("new_test_name");
    ASSERT_EQ(database->get_database_name(), "new_test_name");
}

// test delete_database();
TEST_F(DatabaseTest, test_delete_database) {
    database->save_database();                                              // make sure the database file exists (use save_database())
    ASSERT_TRUE(filesystem::exists(test_database_filename));                // check that the file was created
         
    bool return_value = database->delete_database();                        // call delete_database()
    ASSERT_TRUE(return_value);                                              // check the response
    ASSERT_FALSE(filesystem::exists(test_database_filename));               // check if the file still exists
}

TEST_F(DatabaseTest, test_delete_database_with_no_file) {           // should NEVER be possible to have a database without a file
    Database* new_database = new Database(1, "new_database");
    string path = "files/" + new_database->get_database_name() + ".txt";
    ASSERT_FALSE(filesystem::exists(path));

    bool return_value = database->delete_database();                     // this is a weird case... the function returns true when the file doesn't exist anymore
    ASSERT_TRUE(return_value);                                           // but the file NEVER existed to begin with                                                                                 
}

