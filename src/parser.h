#ifndef __PARSER_H__
#define __PARSER_H__

#include "lex.h"
#include "ast.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

using namespace std;


class Parser {
    public: 
        using prefixParserFn = std::function<void(int)>;
        using infixParserFn = std::function<void(int)>;
        Parser(const string& s):l_(std::move(unique_ptr<Lex>(new Lex(s)))) { 
            nextToken();
            nextToken();
        }

        Token nextToken() { 
            cur_token_ = peek_token_;
            peek_token_ = l_->getNextToken();
            return cur_token_;
        } 
    public:
        bool currentTokenIs(TokenType t) {
            return cur_token_.type == t;
        }
        bool peekTokenIs(TokenType t) {
            return peek_token_.type == t;
        }

        bool expectPeek(TokenType t) {
            if(peekTokenIs(t)) {
                nextToken();
                return true;
            } else {
                peekError(t);
                return false;
            }
        }

        void peekError(TokenType t) { 
            char buf[1024];
            ::snprintf(buf, sizeof(buf), "expect next token to be %s, got %s instead\n", 
                    convertTokenType(t).c_str(), convertTokenType(peek_token_.type).c_str()); 
            errors_.push_back(string(buf));
        }

        const vector<string>& getErrors() {
            return errors_;
        }

    public:
        Program* parseProgram();
        Statement* parseStatement();
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement(); 

    private:
        unique_ptr<Lex> l_;
        vector<string> errors_;
        std::unordered_map<int, prefixParserFn>pre_parser_fn_;
        std::unordered_map<int, infixParserFn>in_parser_fn_; 
        Token cur_token_;
        Token peek_token_;
}; 

#endif
