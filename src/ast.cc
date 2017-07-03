#include "ast.h"
#include "eval.h"
#include "env.h"
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

shared_ptr<Object> Program::eval(Env* env) {
    shared_ptr<Object> res;
    for(const auto& s : statements_) {
        res = s->eval(env); 
        ReturnValue* rv = dynamic_cast<ReturnValue*>(res.get()); 

        if(rv != nullptr) {
            return res;
        }
        
        Error* ev = dynamic_cast<Error*>(res.get());
        if(ev != nullptr) {
            return res;
        } 
    }
    return res;
}
shared_ptr<Object> LetStatement::eval(Env* env) { 
    auto val = value_->eval(env);
    if(isError(val.get())) {
        return val;
    }
    env->set(name_->getValue(), val);
    return val;
}

shared_ptr<Object> ReturnStatement::eval(Env* env) { 
    auto val = expression_->eval(env);
    if(isError(val.get())) {
        return val;
    }
    return  make_shared<ReturnValue>(val);
}

shared_ptr<Object> ExpressionStatement::eval(Env* env) {
    return expr_->eval(env);
}


shared_ptr<Object> PrefixExpression::eval(Env* env) {
    auto right = expr_->eval(env);

    if(isError(right.get())) { 
        return expr_->eval(env);
    } 
    return shared_ptr<Object>(const_cast<Object*>(evalPrefixExpression(operator_, right.get())));
}

shared_ptr<Object> InfixExpression::eval(Env* env) {
    auto left_val = left_->eval(env);
    auto right_val = right_->eval(env);

    if(isError(left_val.get())) { 
        return left_->eval(env);
    }

    if(isError(right_val.get())) {
        return right_->eval(env);
    }

    if(left_val->type() != right_val->type()) { 
        return shared_ptr<Error>(new Error{"type mismatch: ", left_val->type(), " ", op_, " ", right_val->type()});
    } else { 
        return shared_ptr<Object>(const_cast<Object*>(evalInprefixExpression(left_val.get(), right_val.get(), op_)));

    }
}

shared_ptr<Object> IdentifierNode::eval(Env* env) {
    auto val_pair = env->get(value_);

    if(val_pair.first != true) {
        return shared_ptr<Error>(new Error{"identifer not found: ", value_});
    }
    return val_pair.second;
}

shared_ptr<Object> IntegerLiteral::eval(Env* env) {
    auto int_val = make_shared<IntegerObject>(value_);
    return int_val;
}

shared_ptr<Object> Boolean::eval(Env* env) { 
    return make_shared<BooleanObj>(value_);
} 

shared_ptr<Object> BlockStatement::eval(Env* env) {
    shared_ptr<Object> res;
    for(const auto& s : statements_) { 
        res = s->eval(env);

        ReturnValue* rv = dynamic_cast<ReturnValue*>(res.get());
        
        if(rv != nullptr) {
            return res;
        } 

        if(res->type() == "ERROR") {
            return res;
        }

    }
    return res;

}

shared_ptr<Object> IfExpression::eval(Env* env) { 
    shared_ptr<Object> cond = cond_->eval(env);
    if(isError(cond.get())) {
        return cond_->eval(env);
    }

    if(cond->isTrue()) {
        return consequence_->eval(env);
    } else if (alternative_ != nullptr ) {
        return alternative_->eval(env);
    } else {
        return shared_ptr<NullObj>(new NullObj());
    }
}

shared_ptr<Object> FunctionLiteral::eval(Env* env) { 
    return nullptr;
}

shared_ptr<Object> CallExpression::eval(Env* env) { 
    return nullptr;
} 
