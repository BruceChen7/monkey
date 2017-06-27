#ifndef __EVAL_H__
#define __EVAL_H__

#include <string>
#include "ast.h"
#include <unordered_map>
#include <sstream>

struct Object {
    virtual string inspect() = 0; 
    virtual string type() = 0;
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
    int64_t val;
};

struct BooleanObj: public Object { 
    string inspect()  override { 
        stringstream ss;
        ss << val;
        return ss.str();
    } 
    BooleanObj(bool v):val(v) { }
    
    string type() override { 
        return "BOOLEAN";
    } 
    bool val;
};

struct NullObj: public Object {
    string inspect()  override {
        return "null";
    } 
    string type() override { 
        return "null";
    } 
};

#endif
