#include "obj.h" 

Object* IntegerObject::binaryOperation(const string& op,  IntegerObject* right) { 
    if(op == "+") {
        return new IntegerObject{val + right->val};
    }

    if(op == "-") {
        return new IntegerObject(val - right->val);
    }


    if(op == "*") {
        return new IntegerObject(val * right->val);
    }

    if(op == "/") {
        // Fix Me: divide 0
        return new IntegerObject(val / right->val);
    }

    if(op == ">") { 
        return new BooleanObj( val > right->val );
    }

    if(op == "<") {
        return new BooleanObj( val < right->val);
    }

    if(op == ">=") {
        return new BooleanObj(val >= right->val);
    }

    if(op == "==") {
        return new BooleanObj(val == right->val);
    }

    if(op == "<=") {
        return new BooleanObj(val == right->val);
    }
    return new NullObj{};
}


