#include "gtest/gtest.h"
#include "../src/classes/Object.hpp"
#include <string>

using namespace std;

// create test fixture class
class ObjectTest : public ::testing::Test {
protected:
    void SetUp() override {object_instance = new Object(1, "test_object", "test_type", 100);} // create an object

    void TearDown() override {delete object_instance;}                                        // delete the object

    // OBJECT
    Object* object_instance;
};
// test for the constructor and the getters
TEST_F(ObjectTest, test_main_constructor_and_getters) {
    ASSERT_EQ(object_instance->get_id(), 1);                                           // check object id
    ASSERT_EQ(object_instance->get_name(), "test_object");                             // check object name
    ASSERT_EQ(object_instance->get_type(), "test_type");                               // check object type
    ASSERT_EQ(object_instance->get_quantity(), 100);                                   // check object quantity
}

TEST_F(ObjectTest, test_constructor_with_empty_strings) {
    Object negative_quantity_object(2, "negative_item", "item_type", -10);
    ASSERT_EQ(negative_quantity_object.get_quantity(), -10);

    Object empty_name_object(3, "", "no_type", 50);                                 // should never be the case
    ASSERT_TRUE(empty_name_object.get_name().empty());

    Object empty_type_object(4, "test_name", "", 50);                               // should never be the case
    ASSERT_TRUE(empty_type_object.get_type().empty());
}

TEST_F(ObjectTest, test_setters){
    object_instance->set_object_name("edited_name");
    object_instance->set_object_type("edited_type");
    object_instance->set_object_quantity(31);

    ASSERT_EQ(object_instance->get_name(), "edited_name");                         
    ASSERT_EQ(object_instance->get_type(), "edited_type");                         
    ASSERT_EQ(object_instance->get_quantity(), 31);   
}

TEST_F(ObjectTest, test_setters_with_emtpy_strings) {
    object_instance->set_object_name("");
    ASSERT_FALSE(object_instance->get_name().empty());
    
    object_instance->set_object_type("");
    ASSERT_FALSE(object_instance->get_type().empty());
}

TEST_F(ObjectTest, test_overloaded_operators){
    Object* new_object = new Object(2, "new_object", "new_type", 5);
    ASSERT_EQ(*object_instance < *new_object, true);
    ASSERT_EQ(*object_instance == *new_object, false);

    new_object = new Object(1, "new_object", "new_type", 5);
    ASSERT_EQ(*object_instance < *new_object, false);
    ASSERT_EQ(*object_instance == *new_object, true);
}