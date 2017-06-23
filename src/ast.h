#ifndef __AST_H__
#define __AST_H__
#include "lex.h"

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdint>
using namespace std;

class IdentifierNode;
class IdentifierNode;
class ExpressionStatement;

class ASTNode { 
    public:
        virtual string tokenLiteral() = 0;
        virtual string toString() = 0;
        virtual ~ASTNode() {};
};

class Statement : public ASTNode {
    public:
        virtual  void statementNode() = 0;
        virtual string toString() = 0;
        virtual ~Statement() = default;
};

class Expression: public ASTNode {
    public:
        virtual void expressionNode() = 0;
        virtual ~Expression() = default;
}; 

class Program : public ASTNode {
    public:
        void addStatements(Statement* s) { 
            auto ns = unique_ptr<Statement>(s);
            statements_.push_back(std::move(ns));
        }

        virtual string toString() override { 
            stringstream ss;
            for(auto& s : statements_) {
                ss << s->toString();
            }
            return ss.str();
        } 
        virtual string tokenLiteral() override { 
            if(!statements_.empty())  {
                return statements_[0]->tokenLiteral();
            }
        }
    public:
        virtual ~Program() = default;
    private:
        vector<unique_ptr<Statement>> statements_;
}; 

class LetStatement: public Statement { 
    public: 
        LetStatement(const Token& t): token_(t) { 

        }
        void setName(IdentifierNode* name) {
            name_ = std::move(unique_ptr<IdentifierNode>(name)); 
        }

    public: 
        virtual  void statementNode() override {}
        virtual string tokenLiteral() override {
            return token_.value;
        }
        virtual string toString();
    private: 
        Token token_; // "let" token
        unique_ptr<IdentifierNode> name_;
        unique_ptr<Expression> value_;
};


class ReturnStatement: public Statement {
    public: 
        virtual  void statementNode() override {}
        virtual string tokenLiteral() override {
            return token_.value;
        }

        virtual string toString() {
            stringstream ss;
            ss << token_.value << " ";

            if (expression_ != nullptr) {
                ss << expression_->toString();
            }
            ss << "; ";
            return ss.str();
        }

    public:
        ReturnStatement(Token t): token_(t) { 
        }
        ReturnStatement(const ReturnStatement& ) = delete;
        ReturnStatement& operator=(const ReturnStatement&) = delete;

    private: 
        Token token_; // "let" token
        Expression* expression_;
};


class ExpressionStatement: public Statement { 
    public: 
        virtual  void statementNode() override {}
        virtual string tokenLiteral() override {
            return token_.value;
        }
        virtual string toString() override {
            if(expr_ != nullptr) {
                return expr_->toString();
            }
            return "";
        }
    public:
        ExpressionStatement(const Token& t):token_(t) {
        } 
        void setExpression(Expression* e) { 
            expr_ = std::move(unique_ptr<Expression>(e));
        }
    private:
        Token token_; 
        std::unique_ptr<Expression> expr_;
};

class PrefixExpression: public Expression {
    public:
        virtual void expressionNode() override {}
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual string toString() override { 
            stringstream ss;
            ss << "(" << operator_ << expr_->toString() << ")" ;
            return ss.str(); 
        }
    public:
        PrefixExpression(const Token& t, string op, Expression* e):token_(t), operator_(op) {
            expr_.reset(e); 
        }
        
    private:
        Token token_;
        string operator_;
        unique_ptr<Expression> expr_;
};

class InfixExpression: public Expression { 
    public:
        virtual void expressionNode() override {}
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual string toString() override { 
            stringstream ss;
            ss << "(";
            ss << left_->toString() << " " ;
            ss << op_ << " "<< right_->toString() << ")" ;
            
            return ss.str(); 
        }
    public: 
        InfixExpression(const Token& t, const string& op, Expression* l, Expression* r):token_(t),op_(op) {
            left_.reset(l);
            right_.reset(r);
        }
    private:
        Token token_; 
        unique_ptr<Expression> left_;
        string op_;
        unique_ptr<Expression> right_;
};

class IdentifierNode : public Expression {
    public:
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual void expressionNode() override { }

        virtual string toString()  override { 
            return value_;
        }

    public:
        IdentifierNode(Token t, string v = ""): token_(t), value_(v) { 
        }
            
    private: 
        Token token_;
        string value_;
};


class IntegerLiteral: public Expression {
    public:
        virtual void expressionNode() override {}
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual string toString() override { 
            return token_.value;
        } 
    public:
        IntegerLiteral(const Token& t, long long int v): token_(t), value_(v) {
        }
    private:
        Token token_;
        int64_t value_;
};

#endif
