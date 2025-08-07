// Object.cpp
#include "Object.h" // Include the corresponding header file

using namespace std;

Object::Object(int id, string name, string type, int quantity) : object_id(id), name(name), type(type), quantity(quantity){
    // cout << "A new object named " << mName << " has been created!" << endl;
}

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