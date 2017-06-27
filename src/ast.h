#ifndef __AST_H__
#define __AST_H__
#include "lex.h"
#include "obj.h"


#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdint>
#include <algorithm>
using namespace std;

class IdentifierNode;

class ASTNode { 
    public:
        virtual string tokenLiteral() = 0;
        virtual string toString() = 0;
        virtual ~ASTNode() {};
        virtual Object* eval() = 0;
};

class Statement : public ASTNode {
    public:
        virtual  void statementNode() = 0;
        virtual string toString() = 0;
        virtual ~Statement() = default;
        virtual Object* eval() = 0;
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

        const vector<unique_ptr<Statement>>* getStatements() const {
            return &statements_;
        }

        virtual Object* eval();

    public:
        virtual ~Program() = default;
    private:
        vector<unique_ptr<Statement>> statements_;
}; 

class LetStatement: public Statement { 
    public: 
        LetStatement(const Token& t): token_(t) { 

        }
        string getName();
        string getValue();
        void setName(IdentifierNode* name) {
            name_ = std::move(unique_ptr<IdentifierNode>(name)); 
        }
        void setExpression(Expression* e)  { 
            value_.reset(e);
        }
        virtual Object* eval();

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
        virtual Object* eval();

    public:
        ReturnStatement(Token t, Expression* val): token_(t) { 
            expression_.reset(val);
        }

        ReturnStatement(const ReturnStatement& ) = delete;
        ReturnStatement& operator=(const ReturnStatement&) = delete;

    private: 
        Token token_; // "let" token
        unique_ptr<Expression> expression_;
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
        virtual Object* eval();
    public:
        ExpressionStatement(const Token& t):token_(t) {
        } 
        void setExpression(Expression* e) { 
            expr_ = std::move(unique_ptr<Expression>(e));
        }
        Expression* getExpression() const {
            return expr_.get();
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
        virtual Object* eval();
    public:
        PrefixExpression(const Token& t, string op, Expression* e):token_(t), operator_(op) {
            expr_.reset(e); 
        }
        const string& getOP() const {
            return operator_;
        }
        Expression* getExpression() const {
            return expr_.get();
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
        virtual Object* eval();
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
        virtual Object* eval();

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
        int64_t getValue() const {
            return value_;
        }
        virtual Object* eval();
    public:
        IntegerLiteral(const Token& t, long long int v): token_(t), value_(v) { }
    private:
        Token token_;
        int64_t value_;
};

class Boolean: public Expression { 
    public:
        virtual void expressionNode() override {}
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual string toString() override { 
            return token_.value;
        } 
        bool getValue() {
            return value_; 
        }

        virtual Object* eval();

        Boolean(const Token& t, bool v): token_(t), value_(v) { }

    private:
        Token token_;
        bool value_;
};

class BlockStatement: public Statement {
    public:
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual string toString() override { 
            stringstream ss;

            for(const auto& it : statements_) {
                ss << it->toString();
            }
            return ss.str();
        } 
        virtual void statementNode() override { }
        virtual Object* eval();
        
    public:
        BlockStatement(const Token& t): token_(t) { }
        void addStatement(Statement* ns) {
            statements_.emplace_back(unique_ptr<Statement>(ns));
        }
    private: 
        Token token_; // "{" token 
        vector<unique_ptr<Statement>> statements_;

};

class IfExpression: public Expression {
    public:
        virtual void expressionNode() override {}
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual string toString() override { 
            stringstream ss;
            ss << "if " << cond_->toString() << " " << consequence_->toString() ;

            if (alternative_ != nullptr) {
                ss << alternative_->toString(); 
            } 
            return ss.str(); 
        } 
        virtual Object* eval();
    public:
        IfExpression(const Token& t, Expression* c, BlockStatement* cons, BlockStatement* alter = nullptr): token_(t) {
            cond_.reset(c);
            consequence_.reset(cons);
            alternative_.reset(alter);
        }
        Token getToken() {
            return token_;
        }

        Expression* getCondition() { 
            return cond_.get();
        } 

        BlockStatement* getConsequence() {
            return consequence_.get();
        }

        BlockStatement* getAlternative() {
            if(alternative_ != nullptr ) {
                return alternative_.get();
            } else {
                return nullptr;
            }
        }
           
   private:
        Token token_; 
        unique_ptr<Expression> cond_; 
        unique_ptr<BlockStatement> consequence_;
        unique_ptr<BlockStatement> alternative_;

};

class FunctionLiteral: public Expression {
    public:
        virtual void expressionNode() override {}
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual string toString() override { 
            stringstream ss;
            ss << token_.value << "(";

            for(int i = 0; i < para_.size(); i++) {
                ss << para_[i]->toString();

                if ( i != para_.size() - 1) {
                    ss << ", ";
                }
            }
            ss << ")"; 
            ss << body_->toString();
        } 

        virtual Object* eval();
    public:
        FunctionLiteral(const Token& t, const vector<IdentifierNode*>& param, BlockStatement* body):token_(t) {
            body_.reset(body);
            std::transform(param.begin(), param.end(), std::back_inserter(para_), [](IdentifierNode* i) { 
                    return std::move(unique_ptr<IdentifierNode>(i));
            });
        } 
        FunctionLiteral(const FunctionLiteral&) = delete;
        FunctionLiteral&operator=(const FunctionLiteral&) = delete;
        const vector<unique_ptr<IdentifierNode>>& getParam() {
            return para_;
        }
    private:
        vector<unique_ptr<IdentifierNode>> para_;
        Token token_;
        unique_ptr<BlockStatement> body_;
};

class CallExpression: public Expression {
    public:
        virtual void expressionNode() override {}
        virtual string tokenLiteral() override { 
            return token_.value;
        }
        virtual string toString() override { 
            stringstream ss;
            ss << function_->toString() << "(";

            for(int i = 0; i < arguments_.size(); i++) {
                ss << arguments_[i]->toString();

                if ( i != arguments_.size() - 1) {
                    ss << ", ";
                }
            }
            ss << ")"; 
            return ss.str();
        } 
        virtual Object* eval();
    public:
        CallExpression(const CallExpression& o) = delete;
        CallExpression& operator=(const CallExpression&) = delete;
        CallExpression(const Token& t, const vector<Expression*> argu, const Expression* func): token_(t) {
            function_.reset(const_cast<Expression*>(func));

            transform(argu.begin(), argu.end(), back_inserter(arguments_), [](Expression* e) { 
                    return std::move(unique_ptr<Expression>(e));
            });
        }
    private:
        Token token_; // "(" token
        unique_ptr<Expression> function_; // function name
        vector<unique_ptr<Expression>> arguments_;
};
#endif 
