#ifndef __AST_H__
#define __AST_H__
#include "lex.h"

#include <string>
#include <vector>
#include <memory>
using namespace std;

class ASTNode { 
    public:
        virtual string tokenLiteral() = 0;
};

class Statement : public ASTNode {
    public:
        virtual  void statementNode() = 0;
};

class Expression: public ASTNode {
    public:
        virtual void expressionNode() = 0;
};


class IdentifierNode : public Expression {
    public:
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual void expressionNode() override { }
    public:
        IdentifierNode(Token t, string v = ""): token_(t), value_(v) { 
        }
            
    private: 
        Token token_;
        string value_;
};

class Program : public ASTNode {
    public:
        void addStatements(Statement* s) { 
            auto ns = unique_ptr<Statement>(s);
            statements_.push_back(std::move(ns));
        }

        virtual string tokenLiteral() override { 
            if(!statements_.empty())  {
                return statements_[0]->tokenLiteral();
            }
        }
    private:
        vector<unique_ptr<Statement>> statements_;
}; 

class  LetStatement: public Statement { 
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
    public:
        ReturnStatement(Token t): token_(t) { 
        }
        ReturnStatement(const ReturnStatement& ) = delete;
        ReturnStatement& operator=(const ReturnStatement&) = delete;

    private: 
        Token token_; // "let" token
        Expression* expression_;
};

#endif
