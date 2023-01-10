#pragma once

class EmptyClass {
};

class SimpleClass {
public:
    inline SimpleClass(int field) : field(field) {};

    inline int setField(int newValue) { field = newValue; };
    inline int getField() const { return field; };
private:
    int field;
};