#ifndef __OBJ___
#define __OBJ___

#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

struct Object {
    virtual string inspect() = 0; 
    virtual string type() = 0;
    virtual bool isTrue() = 0;
};

struct IntegerObject: public Object {
    string inspect()  override {
        stringstream ss;
        ss << val;
        return ss.str();
    } 
    IntegerObject(int64_t v): val(v) { } 
    
    string type() override { 
        return "INTEGER";
    }
    bool isTrue() override {
        if(val == 0) {
            return false;
        } else {
            return true;
        }
    }

    Object* binaryOperation(const string& op,  IntegerObject* right);
    int64_t val;
};

struct BooleanObj: public Object { 
    string inspect()  override { 
        if(val) {
            return "true";
        } else {
            return "false";
        }
    } 
    BooleanObj(bool v):val(v) { }
    
    string type() override { 
        return "BOOLEAN";
    } 
    
    bool isTrue() override {
        if(val) {
            return true;
        } else {
            return false;
        }
    } 
    bool val;
};

struct NullObj: public Object {
    string inspect()  override {
        return "null";
    } 
    string type() override { 
        return "NULL";
    } 
    bool isTrue() override {
        return false;
    }
};

#endif
