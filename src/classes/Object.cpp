// Object.cpp
#include "Object.hpp" // Include the corresponding header file

using namespace std;

Object::Object(int id, string name, string type, int quantity) : object_id(id), name(name), type(type), quantity(quantity){}

int Object::get_id() const{
    return object_id;
}

const string& Object::get_name() const{
    return name; 
}

const string& Object::get_type() const{
    return type;
}

int Object::get_quantity() const{
    return quantity;
}

bool Object::set_object_name(const std::string& new_name){
    if(new_name.empty())
        return false;    
    name = new_name;
    return true;
}

bool Object::set_object_type(const std::string& new_type){  
    if(new_type.empty())
        return false;
    type = new_type;
    return true;
}

void Object::set_object_quantity(int new_quantity){
    quantity = new_quantity;
}

bool Object::operator<(const Object& object_to_compare){
    return object_id < object_to_compare.get_id();
}

bool Object::operator==(const Object& object_to_compare){
    return object_id == object_to_compare.get_id();
}   
