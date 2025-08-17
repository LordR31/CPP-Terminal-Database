#include "gtest/gtest.h"
#include "../src/classes/MenuLogic.hpp"
#include "../src/classes/Database.hpp"
#include "../src/utils.hpp"
#include <vector>
#include <queue>
#include <string>

using namespace std;

class MockMenuUI : public MenuUI {
public:
    // PRINT MENUS
    void print_main_menu(char decorator_type, bool text_position) override {}             
    void print_manage_databases_menu(char decorator_type, bool text_position) override {} 
    void print_create_database_menu(char decorator_type, bool text_position) override {}
    void print_delete_database_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, std::vector<Database> database_vector) override {} 
    void print_current_database_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, Database current_database) override {}            
    void print_settings_menu(char decorator_type, bool text_position) override {}

    // Manage Database Sub-Menus
    void print_load_database(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, std::vector<Database> database_vector) override {} 
    void print_choose_database(bool text_position) override {}
    void print_found_databases(bool text_position, int number_of_entries, int page_number, std::vector<Database> found_databases) override {}
    void print_find_menu(bool text_position, int number_of_entries, bool is_database) override {}
    void print_search_word(const std::string& search_word) override {}
    void print_load_database_options(bool is_empty, bool is_paged, bool show_clear_button) override {}
    void print_edit_database_menu(bool text_position) override {}
    void print_edit_database_name() override {}

    // Create Database Sub-Menus
    void print_database_already_exists(bool text_position) override {}
    void print_database_created_successfully(bool text_position) override {}

    // Delete Database Sub-Menus
    void print_delete_database_menu_options(bool is_empty, bool is_paged, bool show_clear_button) override {}
    void print_delete_database_choose_menu(bool text_position) override {}
    void print_delete_database_confirm_deletion_menu() override {}
    void print_delete_database_confirm_deletion_prompt(bool is_confirmed) override {}

    // Available Databases Sub-Menus
    void print_available_databases_menu(char decorator_type, bool text_position, int number_of_entries, bool is_empty, bool is_paged, int page_number, std::vector<Database> database_vector) override {}
    void print_available_databases_options(bool is_empty, bool is_paged) override {}
    
    // Current Database Sub-Menus
    void print_current_database_menu_options(bool is_empty, bool is_paged, bool show_clear_button) override {}
    void print_current_database_menu_delete_object_prompt(int number_of_elements_deleted) override {}
    void print_add_object_menu(char decorator_type) override {}
    void print_add_object_result(bool result) override {}
    void print_delete_object_menu() override {}
    void print_delete_by_id() override {}
    void print_delete_by_name() override {}
    void print_delete_by_type() override {}
    void print_edit_object_menu() override {}
    void print_edit_object(char decorator_type, int object_id, Database current_database) override {}
    void print_found_objects(int number_of_entries, int page_number, const std::vector<Object>& found_objects) override {}

    // Settings Sub-Menus
    void print_decorator_settings(char decorator_type) override {}
    void print_text_position_settings(bool text_position) override {}
    void print_sound_settings(bool sound) override {}
    void print_number_of_entries_settings(int number_of_entries) override {}
    void print_continuous_mode_settings(bool continuous_mode) override {}

    // Special Functions
    void print_program_tittle() override {}
    void print_invalid_input(bool text_position) override {}
    void clear_user_input_zone() override {}
    void clear_search_results(int number_of_entries) override {}
};

class MockInputSource : public MenuInput {
public:
    queue<int> char_input;
    queue<string> string_input;

    void push_char(int value) {
        char_input.push(value);
    }

    int get_char() override {
        if (!char_input.empty()) {
            int temp_char = char_input.front();
            char_input.pop();
            return temp_char;
        }
        return 121;
    }

    string get_string(int read_size) override {
        if (!string_input.empty()) {
            string temp_string = string_input.front();
            string_input.pop();
            return temp_string;
        }
        return "";
    }
};

class MenuLogicTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {
        filesystem::remove("program_settings.txt");
        filesystem::remove("index_manager.txt");
        filesystem::remove_all("files");
    }
};

// TESTS

TEST_F(MenuLogicTest, test_main_menu_exit_program) {
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    mock_input.push_char(ASCII_3);
    int next_menu_id = menu_logic.main_menu();
    int current_menu_id = menu_logic.get_current_menu_id();
    ASSERT_EQ(current_menu_id, MAIN_MENU_ID);
    ASSERT_EQ(next_menu_id, EXIT_PROGRAM);
}

TEST_F(MenuLogicTest, test_settings_menu){
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);
    
    mock_input.char_input.push(ASCII_2);
    int next_menu_id = menu_logic.main_menu();
    int current_menu_id = menu_logic.get_current_menu_id();

    ASSERT_EQ(next_menu_id, SETTINGS_MENU_ID);
    ASSERT_EQ(current_menu_id, MAIN_MENU_ID);

    mock_input.char_input.push(ASCII_ENTER);
    menu_logic.settings_menu();
    current_menu_id = menu_logic.get_current_menu_id();
    ASSERT_EQ(current_menu_id, SETTINGS_MENU_ID);

    char decorator_type = menu_logic.get_decorator_type();
    bool text_position = menu_logic.get_text_position();
    bool sound = menu_logic.get_sound();
    int number_of_entries = menu_logic.get_number_of_entries();
    bool continuous_mode = menu_logic.get_continuous_mode();

    ASSERT_EQ(decorator_type, '*');
    ASSERT_EQ(text_position, 1);
    ASSERT_EQ(sound, 0);
    ASSERT_EQ(number_of_entries, 5);
    ASSERT_EQ(continuous_mode, 1);

    mock_input.char_input.push(ASCII_1); // decorator type
    mock_input.char_input.push(ASCII_3); // option 2
    menu_logic.settings_menu();

    mock_input.char_input.push(ASCII_2); // text position
    mock_input.char_input.push(ASCII_2); // centered
    menu_logic.settings_menu();

    mock_input.char_input.push(ASCII_3); // sound
    mock_input.char_input.push(ASCII_1); // on
    menu_logic.settings_menu();

    mock_input.char_input.push(ASCII_4); // number of entries
    mock_input.char_input.push(ASCII_1); // 3
    menu_logic.settings_menu();

    mock_input.char_input.push(ASCII_5); // continuous mode
    mock_input.char_input.push(ASCII_2); // off
    menu_logic.settings_menu();

    mock_input.char_input.push(ASCII_6); // save settings
    menu_logic.settings_menu();

    decorator_type = menu_logic.get_decorator_type();
    text_position = menu_logic.get_text_position();
    sound = menu_logic.get_sound();
    number_of_entries = menu_logic.get_number_of_entries();
    continuous_mode = menu_logic.get_continuous_mode();

    ASSERT_EQ(decorator_type, '+');
    ASSERT_EQ(text_position, 0);
    ASSERT_EQ(sound, 1);
    ASSERT_EQ(number_of_entries, 3);
    ASSERT_EQ(continuous_mode, 0);
}

TEST_F(MenuLogicTest, test_enter_manage_databases){
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    mock_input.char_input.push(ASCII_1); // Manage Databases
    int next_menu_id = menu_logic.main_menu();
    ASSERT_EQ(next_menu_id, MANAGE_DATABASES_MENU_ID);
    
    mock_input.char_input.push(ASCII_ENTER); // Back
    next_menu_id = menu_logic.manage_databases_menu();
    ASSERT_EQ(next_menu_id, MAIN_MENU_ID);
}

TEST_F(MenuLogicTest, test_main_menu_invalid_input_and_exit) {
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    mock_input.char_input.push('h'); // do nothing
    int next_menu_id = menu_logic.main_menu();
    ASSERT_EQ(next_menu_id, MAIN_MENU_ID);

    mock_input.char_input.push('5');
    next_menu_id = menu_logic.main_menu();
    ASSERT_EQ(next_menu_id, MAIN_MENU_ID);

    mock_input.char_input.push(ASCII_3);
    next_menu_id = menu_logic.main_menu();
    ASSERT_EQ(next_menu_id, EXIT_PROGRAM);
}

TEST_F(MenuLogicTest, test_create_database_successfully) {
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    mock_input.char_input.push(ASCII_1);           // Manage Datbasases
    int next_menu_id = menu_logic.main_menu();
    int current_menu_id = menu_logic.get_current_menu_id();
    ASSERT_EQ(current_menu_id, MAIN_MENU_ID);
    ASSERT_EQ(next_menu_id, MANAGE_DATABASES_MENU_ID);
    
    mock_input.char_input.push(ASCII_1);           // Create Database
    next_menu_id = menu_logic.manage_databases_menu();
    current_menu_id = menu_logic.get_current_menu_id();
    ASSERT_EQ(current_menu_id, MANAGE_DATABASES_MENU_ID);
    ASSERT_EQ(next_menu_id, CREATE_DATABASE_MENU_ID);
    
    const vector<Database>& test_vector_before = menu_logic.get_database_vector();
    ASSERT_EQ(test_vector_before.size(), 0);

    mock_input.string_input.push("test_database"); // Database name
    mock_input.char_input.push(ASCII_ENTER);       // Confirm database name
    mock_input.char_input.push(ASCII_ENTER);       // Continue
    next_menu_id = menu_logic.create_database_menu();
    current_menu_id = menu_logic.get_current_menu_id();
    ASSERT_EQ(current_menu_id, CREATE_DATABASE_MENU_ID);
    ASSERT_EQ(next_menu_id, MANAGE_DATABASES_MENU_ID);

    const vector<Database>& test_vector_after = menu_logic.get_database_vector();
    ASSERT_EQ(test_vector_after.size(), 1);
    ASSERT_EQ(test_vector_after[0].get_database_name(), "test_database");
}

TEST_F(MenuLogicTest, test_cancel_create_database){
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    mock_input.char_input.push(ASCII_1);     // Manage Datbasases
    int next_menu_id = menu_logic.main_menu();
    int current_menu_id = menu_logic.get_current_menu_id();
    ASSERT_EQ(current_menu_id, MAIN_MENU_ID);
    ASSERT_EQ(next_menu_id, MANAGE_DATABASES_MENU_ID);

    ASSERT_EQ(menu_logic.get_database_vector().size(), 0);

    mock_input.char_input.push(ASCII_1);     // Create Database
    mock_input.string_input.push("");        // Empty Database name
    mock_input.char_input.push(ASCII_ENTER); // Confirm database name
    next_menu_id = menu_logic.create_database_menu();
    current_menu_id = menu_logic.get_current_menu_id();
    ASSERT_EQ(current_menu_id, CREATE_DATABASE_MENU_ID);
    ASSERT_EQ(next_menu_id, MANAGE_DATABASES_MENU_ID);

    ASSERT_EQ(menu_logic.get_database_vector().size(), 0);
}

TEST_F(MenuLogicTest, test_edit_database_options){
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);
   
    mock_input.string_input.push("test_database"); // Database name
    mock_input.char_input.push(ASCII_ENTER);       // Confirm database name
    mock_input.char_input.push(ASCII_ENTER);       // Continue
    menu_logic.create_database_menu();
    ASSERT_EQ(menu_logic.get_database_vector().size(), 1);

    const vector<Database>& test_vector_before = menu_logic.get_database_vector();
    ASSERT_EQ(menu_logic.get_database_vector().size(), 1);
    
    mock_input.char_input.push(ASCII_ENTER);          // Edit
    mock_input.char_input.push(ASCII_ENTER);          // Edit
    mock_input.string_input.push("0");                // First Database
    mock_input.string_input.push("editted_database"); // First Database
    menu_logic.edit_database();

    const vector<Database>& test_vector_after = menu_logic.get_database_vector();
    ASSERT_EQ(test_vector_after[0].get_database_name(), "editted_database");
}

TEST_F(MenuLogicTest, test_delete_database){
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    mock_input.string_input.push("test_database_1"); // Database name
    mock_input.char_input.push(ASCII_ENTER);         // Confirm database name
    // mock_input.char_input.push(ASCII_ENTER);         // Continue
    menu_logic.create_database_menu();

    const vector<Database>& test_vector_before = menu_logic.get_database_vector();
    ASSERT_EQ(menu_logic.get_database_vector().size(), 1);

    mock_input.char_input.push(ASCII_2);             // Enter Delete Database Menu
    mock_input.string_input.push("0");               // Select first database
    mock_input.char_input.push(ASCII_ENTER);         // Enter Delete Database Menu
    mock_input.string_input.push("delete");          // Wrong input
    // mock_input.char_input.push(ASCII_ENTER);         // Confirm
    // mock_input.char_input.push(ASCII_ENTER);         // Continue
    menu_logic.delete_database_menu();

    mock_input.char_input.push(ASCII_2);             // Enter Delete Database Menu
    mock_input.string_input.push("0");
    mock_input.char_input.push(ASCII_ENTER);         // Enter Delete Database Menu
    mock_input.string_input.push("DELETE");
    mock_input.char_input.push(ASCII_ENTER);         // Confirm
    mock_input.char_input.push(ASCII_ENTER);         // Continue
    menu_logic.delete_database_menu();

    const vector<Database>& test_vector_after = menu_logic.get_database_vector();
    ASSERT_EQ(test_vector_after.size(), 0);
}

TEST_F(MenuLogicTest, test_increment_page_in_all_available_databases) {
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    // create 6 databases to get pages
    mock_input.string_input.push("test_database_1"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_2"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_3"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_4"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_5"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_6"); // Database name
    menu_logic.create_database_menu();

    const vector<Database>& test_vector = menu_logic.get_database_vector();
    ASSERT_EQ(menu_logic.get_database_vector().size(), 6);

    int page_number = menu_logic.get_page_number();
    ASSERT_EQ(page_number, 0);

    mock_input.char_input.push(ASCII_3);
    menu_logic.available_databases_menu();

    page_number = menu_logic.get_page_number();
    ASSERT_EQ(page_number, 1);
    
    mock_input.char_input.push(ASCII_3);
    menu_logic.available_databases_menu();
    page_number = menu_logic.get_page_number();
    ASSERT_EQ(page_number, 0);
}

TEST_F(MenuLogicTest, test_decrement_page_in_all_available_databases) {
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    // create 6 databases to get pages
    mock_input.string_input.push("test_database_1"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_2"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_3"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_4"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_5"); // Database name
    menu_logic.create_database_menu();

    mock_input.string_input.push("test_database_6"); // Database name
    menu_logic.create_database_menu();

    const vector<Database>& test_vector = menu_logic.get_database_vector();
    ASSERT_EQ(menu_logic.get_database_vector().size(), 6);

    int page_number = menu_logic.get_page_number();
    ASSERT_EQ(page_number, 0);

    mock_input.char_input.push(ASCII_1);
    menu_logic.available_databases_menu();

    page_number = menu_logic.get_page_number();
    ASSERT_EQ(page_number, 1);
    
    mock_input.char_input.push(ASCII_1);
    menu_logic.available_databases_menu();
    page_number = menu_logic.get_page_number();
    ASSERT_EQ(page_number, 0);
}

TEST_F(MenuLogicTest, test_create_database_and_add_delete_and_edit_objects){
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    mock_input.string_input.push("test_database_1"); // Database name
    menu_logic.create_database_menu();

    mock_input.char_input.push(ASCII_2); // choose database
    mock_input.string_input.push("0"); // first database
    menu_logic.load_database_menu();

    Database test_database = menu_logic.get_current_database();
    int test_number_of_objects = test_database.get_number_of_objects();
    ASSERT_EQ(test_number_of_objects, 0);

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_1 test_type_1 123");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_2 test_type_2 123");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_3 test_type_3 123");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_1 test_type_1 123");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_2 test_type_2 123");
    menu_logic.current_database_menu();

    test_database = menu_logic.get_current_database();
    test_number_of_objects = test_database.get_number_of_objects();
    ASSERT_EQ(test_number_of_objects, 5);

    // delete by id
    mock_input.char_input.push(ASCII_3); // delete
    mock_input.char_input.push(ASCII_1); // by ID
    mock_input.string_input.push("2"); // test_item_3's id
    menu_logic.current_database_menu();

    test_database = menu_logic.get_current_database();
    test_number_of_objects = test_database.get_number_of_objects();
    ASSERT_EQ(test_number_of_objects, 4);

    // delete by name
    mock_input.char_input.push(ASCII_3); // delete
    mock_input.char_input.push(ASCII_2); // by ID
    mock_input.string_input.push("test_item_1"); // test_item_3's id
    menu_logic.current_database_menu();

    test_database = menu_logic.get_current_database();
    test_number_of_objects = test_database.get_number_of_objects();
    ASSERT_EQ(test_number_of_objects, 2);

    // delete by type
    mock_input.char_input.push(ASCII_3); // delete
    mock_input.char_input.push(ASCII_3); // by ID
    mock_input.string_input.push("test_type_2"); // test_item_3's id
    menu_logic.current_database_menu();

    test_database = menu_logic.get_current_database();
    test_number_of_objects = test_database.get_number_of_objects();
    ASSERT_EQ(test_number_of_objects, 0);
}

TEST_F(MenuLogicTest, test_edit_database_object){
    MockInputSource mock_input;
    MockMenuUI mock_ui;
    MenuLogic menu_logic(mock_input, mock_ui);

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_1 test_type_1 1");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_2 test_type_2 2");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_3 test_type_3 3");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_4 test_type_4 4");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_5 test_type_5 5");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_6 test_type_6 6");
    menu_logic.current_database_menu();

    mock_input.char_input.push(ASCII_2); // add item
    mock_input.string_input.push("test_item_7 test_type_7 7");
    menu_logic.current_database_menu();
    
    Database test_database = menu_logic.get_current_database();
    vector<Object> test_object_vector = test_database.get_database_objects();

    ASSERT_EQ(test_object_vector[0].get_id(), 0);
    ASSERT_EQ(test_object_vector[0].get_name(), "test_item_1");
    ASSERT_EQ(test_object_vector[0].get_type(), "test_type_1");
    ASSERT_EQ(test_object_vector[0].get_quantity(), 1);

    ASSERT_EQ(test_object_vector[1].get_id(), 1);
    ASSERT_EQ(test_object_vector[1].get_name(), "test_item_2");
    ASSERT_EQ(test_object_vector[1].get_type(), "test_type_2");
    ASSERT_EQ(test_object_vector[1].get_quantity(), 2);

    ASSERT_EQ(test_object_vector[2].get_id(), 2);
    ASSERT_EQ(test_object_vector[2].get_name(), "test_item_3");
    ASSERT_EQ(test_object_vector[2].get_type(), "test_type_3");
    ASSERT_EQ(test_object_vector[2].get_quantity(), 3);

    ASSERT_EQ(test_object_vector[3].get_id(), 3);
    ASSERT_EQ(test_object_vector[3].get_name(), "test_item_4");
    ASSERT_EQ(test_object_vector[3].get_type(), "test_type_4");
    ASSERT_EQ(test_object_vector[3].get_quantity(), 4);

    ASSERT_EQ(test_object_vector[4].get_id(), 4);
    ASSERT_EQ(test_object_vector[4].get_name(), "test_item_5");
    ASSERT_EQ(test_object_vector[4].get_type(), "test_type_5");
    ASSERT_EQ(test_object_vector[4].get_quantity(), 5);

    ASSERT_EQ(test_object_vector[5].get_id(), 5);
    ASSERT_EQ(test_object_vector[5].get_name(), "test_item_6");
    ASSERT_EQ(test_object_vector[5].get_type(), "test_type_6");
    ASSERT_EQ(test_object_vector[5].get_quantity(), 6);

    ASSERT_EQ(test_object_vector[6].get_id(), 6);
    ASSERT_EQ(test_object_vector[6].get_name(), "test_item_7");
    ASSERT_EQ(test_object_vector[6].get_type(), "test_type_7");
    ASSERT_EQ(test_object_vector[6].get_quantity(), 7);

    mock_input.string_input.push("0"); 
    mock_input.string_input.push("  101");
    menu_logic.edit_object_menu();

    mock_input.string_input.push("1"); 
    mock_input.string_input.push(" edited_type_1 ");
    menu_logic.edit_object_menu();

    mock_input.string_input.push("2"); 
    mock_input.string_input.push(" edited_type_3 103");
    menu_logic.edit_object_menu();

    mock_input.string_input.push("3"); 
    mock_input.string_input.push("edited_name_4  ");
    menu_logic.edit_object_menu();

    mock_input.string_input.push("4"); 
    mock_input.string_input.push("edited_name_5  105");
    menu_logic.edit_object_menu();

    mock_input.string_input.push("5");
    mock_input.string_input.push("edited_name_6 edited_type_6 ");
    menu_logic.edit_object_menu();

    mock_input.string_input.push("6"); 
    mock_input.string_input.push("edited_name_7 edited_type_7 107");
    menu_logic.edit_object_menu();

    test_object_vector.clear();
    test_database = menu_logic.get_current_database();
    test_object_vector = test_database.get_database_objects();

    ASSERT_EQ(test_object_vector[0].get_id(), 0);
    ASSERT_EQ(test_object_vector[0].get_name(), "test_item_1");
    ASSERT_EQ(test_object_vector[0].get_type(), "test_type_1");
    ASSERT_EQ(test_object_vector[0].get_quantity(), 101);

    ASSERT_EQ(test_object_vector[1].get_id(), 1);
    ASSERT_EQ(test_object_vector[1].get_name(), "test_item_2");
    ASSERT_EQ(test_object_vector[1].get_type(), "edited_type_1");
    ASSERT_EQ(test_object_vector[1].get_quantity(), 2);

    ASSERT_EQ(test_object_vector[2].get_id(), 2);
    ASSERT_EQ(test_object_vector[2].get_name(), "test_item_3");
    ASSERT_EQ(test_object_vector[2].get_type(), "edited_type_3");
    ASSERT_EQ(test_object_vector[2].get_quantity(), 103);

    ASSERT_EQ(test_object_vector[3].get_id(), 3);
    ASSERT_EQ(test_object_vector[3].get_name(), "edited_name_4");
    ASSERT_EQ(test_object_vector[3].get_type(), "test_type_4");
    ASSERT_EQ(test_object_vector[3].get_quantity(), 4);

    ASSERT_EQ(test_object_vector[4].get_id(), 4);
    ASSERT_EQ(test_object_vector[4].get_name(), "edited_name_5");
    ASSERT_EQ(test_object_vector[4].get_type(), "test_type_5");
    ASSERT_EQ(test_object_vector[4].get_quantity(), 105);

    ASSERT_EQ(test_object_vector[5].get_id(), 5);
    ASSERT_EQ(test_object_vector[5].get_name(), "edited_name_6");
    ASSERT_EQ(test_object_vector[5].get_type(), "edited_type_6");
    ASSERT_EQ(test_object_vector[5].get_quantity(), 6);

    ASSERT_EQ(test_object_vector[6].get_id(), 6);
    ASSERT_EQ(test_object_vector[6].get_name(), "edited_name_7");
    ASSERT_EQ(test_object_vector[6].get_type(), "edited_type_7");
    ASSERT_EQ(test_object_vector[6].get_quantity(), 107);
}