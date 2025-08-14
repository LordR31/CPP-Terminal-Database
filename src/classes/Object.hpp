// Object.h
#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object{
public: 
    Object(int id, std::string name, std::string type, int quantity);

    int get_id() const;
    const std::string& get_name() const;
    const std::string& get_type() const;
    int get_quantity() const;

private:
    int object_id;    // cppcheck-suppress unusedStructMember
    std::string name; // cppcheck-suppress unusedStructMember
    std::string type; // cppcheck-suppress unusedStructMember
    int quantity;
};

#endif // OBJECT