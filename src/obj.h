#ifndef __OBJ___
#define __OBJ___

#include <string>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <iostream>

using namespace std;

struct Object {
    virtual string inspect() = 0; 
    virtual string type() = 0;
    virtual bool isTrue()  {
        return true;
    }
    virtual ~Object() {}
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

struct ReturnValue: public Object { 
    string inspect() override { 
        return val->inspect();
    }
    ReturnValue(Object* o) {
        cout << "set " << endl;
        val.reset(o);
    }
    string type() override {
        return "RETURN_VALUE";
    }
    ~ReturnValue() = default;

    ReturnValue(const ReturnValue& ) = delete;
    ReturnValue& operator=(const ReturnValue&) = delete;
    unique_ptr<Object> val;
};

struct Error: public Object { 

    string type() override {
        return "ERROR";
    }

    string inspect() override {
        return msg;
    }
    bool isTrue() override {
        return false;
    }
    Error(const std::initializer_list<string> para) {
        stringstream ss; 
        for(const auto& p : para) {
            ss << p;
        }
        msg = ss.str();
    }
    string msg; 
};
#endif
