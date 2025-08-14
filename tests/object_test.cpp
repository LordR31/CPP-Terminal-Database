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
    ASSERT_EQ(object_instance->get_id(), 1);                                                  // check object id
    ASSERT_EQ(object_instance->get_name(), "test_object");                             // check object name
    ASSERT_EQ(object_instance->get_type(), "test_type");                               // check object type
    ASSERT_EQ(object_instance->get_quantity(), 100);                                   // check object quantity
}
