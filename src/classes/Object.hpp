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

    bool set_object_name(const std::string& new_name);
    bool set_object_type(const std::string& new_type);
    void set_object_quantity(int new_quantity);

    bool operator<(const Object& object_to_compare);
    bool operator==(const Object& object_to_compare);



private:
    int object_id;    // cppcheck-suppress unusedStructMember
    std::string name; // cppcheck-suppress unusedStructMember
    std::string type; // cppcheck-suppress unusedStructMember
    int quantity;
};

#endif // OBJECT