#include "ast.h"
#include "eval.h"

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
    auto right = expr_->eval();
    if(isError(right)) { 
        return right;
    } 
    return const_cast<Object*>(evalPrefixExpression(operator_, right));
}

Object* InfixExpression::eval() {
    Object* left_val = left_->eval();
    Object* right_val = right_->eval();

    if(isError(left_val)) {
        return left_val;
    }

    if(isError(right_val)) {
        return right_val;
    }

    if(left_val->type() != right_val->type()) { 
        return new Error({"type mismatch: ", left_val->type(), " ", op_, " ", right_val->type()});
    }
    return const_cast<Object*>(evalInprefixExpression(left_val, right_val, op_));
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
    Object* cond = cond_->eval();
    if(isError(cond)) {
        return cond;
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
