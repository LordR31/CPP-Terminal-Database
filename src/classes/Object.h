// Object.h
#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object{
public: 
    Object(int id, std::string name, std::string type, int quantity);   // Constructor

    int get_id() const;
    std::string get_name() const;
    std::string get_type() const;
    int get_quantity() const;

private:
    int object_id;
    std::string name;
    std::string type;
    int quantity;
};

#endif // OBJECT