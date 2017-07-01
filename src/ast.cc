#include "ast.h"
#include "eval.h"
#include <iostream>
using namespace std;

string LetStatement::toString() {
    stringstream ss; 
    ss << token_.value << " "; 
    ss << name_->toString();
    ss << " = " ;
    if (value_ != nullptr) { 
        ss << value_->toString();
    }
    ss << ";";
    return ss.str();
}

string LetStatement::getName() { 
    return name_->toString(); 
}

string LetStatement::getValue() {
    return value_->toString();
}

Object* Program::eval() {
    Object* res = nullptr;
    Object* previous_res = nullptr;
    for(const auto& s : statements_) {
        previous_res = res;
        res = s->eval(); 

        if(previous_res) {
            cout << "delete previous res " << endl;
            delete previous_res;
        }

        ReturnValue* rv = dynamic_cast<ReturnValue*>(res); 

        if(rv != nullptr) {
            return res;
        }
        
        Error* ev = dynamic_cast<Error*>(res);
        if(ev != nullptr) {
            return res;
        } 
    }
    return res;
}
Object* LetStatement::eval() {
    return nullptr;
}

Object* ReturnStatement::eval() { 
    auto val = expression_->eval();
    if(isError(val)) {
        return val;
    }
    return  new ReturnValue(val);
}

Object* ExpressionStatement::eval() {
    return expr_->eval();
}


Object* PrefixExpression::eval() {
    auto right = unique_ptr<Object>(expr_->eval());

    if(isError(right.get())) { 
        return expr_->eval();
    } 
    return const_cast<Object*>(evalPrefixExpression(operator_, right.get()));
}

Object* InfixExpression::eval() {
    auto left_val = unique_ptr<Object>(left_->eval());
    auto right_val = unique_ptr<Object>(right_->eval());

    if(isError(left_val.get())) { 
        return left_->eval();
    }

    if(isError(right_val.get())) {
        return right_->eval();
    }

    if(left_val->type() != right_val->type()) { 
        return new Error({"type mismatch: ", left_val->type(), " ", op_, " ", right_val->type()});
    } else { 
        return const_cast<Object*>(evalInprefixExpression(left_val.get(), right_val.get(), op_));

    }
}

Object* IdentifierNode::eval() {
    return nullptr;
}

Object* IntegerLiteral::eval() {
    Object* int_val = new IntegerObject(value_);
    return int_val;
}

Object* Boolean::eval() { 
    return new BooleanObj(value_);
} 

Object* BlockStatement::eval() {
    Object* res;
    for(const auto& s : statements_) { 
        res = s->eval();

        ReturnValue* rv = dynamic_cast<ReturnValue*>(res);
        
        if(rv != nullptr) {
            return res;
        } 

        if(res->type() == "ERROR") {
            return res;
        }

    }
    return res;

}

Object* IfExpression::eval() { 
    unique_ptr<Object> cond(cond_->eval());
    if(isError(cond.get())) {
        return cond_->eval();
    }

    if(cond->isTrue()) {
        return consequence_->eval();
    } else if (alternative_ != nullptr ) {
        return alternative_->eval();
    } else {
        return new NullObj();
    }
}

Object* FunctionLiteral::eval() { 
    return nullptr;
}

Object* CallExpression::eval() { 
    return nullptr;
} 
