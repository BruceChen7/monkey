#ifndef __OBJ___
#define __OBJ___

#include <string>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <vector>


class  IdentifierNode;
class BlockStatement;
class Env;
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
    ReturnValue(shared_ptr<Object> o) { 
        val = o;
    }
    string type() override {
        return "RETURN_VALUE";
    }
    ~ReturnValue() = default;

    ReturnValue(const ReturnValue& ) = delete;
    ReturnValue& operator=(const ReturnValue&) = delete;
    shared_ptr<Object> val;
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

struct FunctionObj: public Object {
    vector<shared_ptr<IdentifierNode>> params;
    shared_ptr<BlockStatement> body; 
    shared_ptr<Env> env;
    FunctionObj(const vector<shared_ptr<IdentifierNode>>& p, const shared_ptr<BlockStatement>& b, const shared_ptr<Env>& e):
        params(p), body(b), env(e) {  }
    string type() override {
        return "FUNCTION";
    }
    // Implemented in obj.cc
    string inspect();
};
#endif
