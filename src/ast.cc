#include "ast.h"

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
    Object* res;
    for(const auto& s : statements_) {
        res = s->eval(); 
    }
    return res;
}
Object* LetStatement::eval() {
    return nullptr;
}

Object* ReturnStatement::eval() {
    return nullptr;
}

Object* ExpressionStatement::eval() {
    return expr_->eval();
}


Object* PrefixExpression::eval() {
    return nullptr;
}

Object* InfixExpression::eval() {
    return nullptr;
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
    return nullptr;
}

Object* IfExpression::eval() {
    return nullptr;

}

Object* FunctionLiteral::eval() { 
    return nullptr;
}

Object* CallExpression::eval() { 
    return nullptr;
} 
