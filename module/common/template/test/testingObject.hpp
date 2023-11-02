#pragma once
#include <iostream>

class TestingObject
{
public:
    TestingObject(std::string& in_str) : str(in_str) {
        str += "(constructor)";
    };
    TestingObject(TestingObject&& other) : str(other.str) {
        str += "(move constructor)";
    }
    ~TestingObject() {
        str += "(destructor)";
    }
protected:
    std::string& str;
};