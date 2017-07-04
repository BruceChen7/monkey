#include "obj.h" 
#include "ast.h"

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
    return new Error({"unknown operator: ",  type(), " ", op, " ", right->type()});
};



string FunctionObj::inspect() { 
    stringstream ss;
    ss << "fn(";
    for(auto i = 0; i < params.size(); i++) {
        ss << params[i]->toString();

        if(i != params.size() - 1) {
            ss << params[i] << " , ";
        }
    }

    ss << ") {\n";
    ss << body->toString();
    ss << "\n}"; 
    return ss.str();
}
