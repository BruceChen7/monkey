#include "eval.h"
#include "obj.h" 

const Object* evalPrefixExpression(const string& op, Object* right) {
    if(op == "!") {
        return evalBangOperatorExpression(right);
    } else if(op == "-") { 
        return evalMinusPrefixExpression(right);
    } else { 
        return new Error({"unknown operator: ", right->type()});
    }

}
const Object* evalBangOperatorExpression(Object* right) { 
    if(right->isTrue()) { 
        return  new BooleanObj(false);
    } else  {
        return new BooleanObj(true);
    }

}

const Object* evalMinusPrefixExpression(Object* right) { 
    if (right->type() != "INTEGER") {
        return new Error({"unknown operator: -", right->type()});

    } else {
        auto value = dynamic_cast<IntegerObject*>(right)->val; 
        return new IntegerObject(-1 * value);
    
    }
}

const Object* evalInprefixExpression(Object* left, Object* right, const string& op) { 
    if(left->type() == "INTEGER" && right->type() == "INTEGER") { 
        auto int_left = dynamic_cast<IntegerObject*>(left);
        auto int_right = dynamic_cast<IntegerObject*>(right);
        return int_left->binaryOperation(op, int_right);
    } else {
        return new Error({"unknown operator: ",  left->type(), " ", op, " ", right->type()});
    }
    
}

bool isError(Object* obj) {
    if(obj != nullptr) {
        return obj->type() == "ERROR";
    }
    return false;
} 

