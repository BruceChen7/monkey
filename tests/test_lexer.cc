#include "lex.h"
#include <vector>
#include <memory>
#include <iostream>

using namespace std; 

vector<string> tests = { 
    "let x = 1;",
    "if(x < y) x = 1; else { y = 1; }",
    "let add = fn(x, y) { return x + y }; " ,
    "-a * b;",
    "-a * b",
    "// ab is a variable ",
    "a * b; // a multiply b"
};


vector<vector<Token>> res =  {
    {
        { "let", TokenType::LET },
        { "x", TokenType::IDENTIFIER }, 
        { "=", TokenType::ASSIGN },
        { "1", TokenType::INT }, 
        { ";", TokenType::SEMICOLON },
        { "", TokenType::EndOfLine }
    }, 

    {
        { "if", TokenType::IF },
        { "(", TokenType::LPAREN }, 
        { "x", TokenType::IDENTIFIER }, 
        { "<", TokenType::LESS },
        { "y", TokenType::IDENTIFIER },
        { ")", TokenType::RPAREN },
        { "x", TokenType::IDENTIFIER },
        { "=", TokenType::ASSIGN},
        { "1", TokenType::INT},
        { ";", TokenType::SEMICOLON },
        { "else", TokenType::ELSE},
        { "{", TokenType::LBRACE },
        { "y", TokenType::IDENTIFIER },
        { "=", TokenType::ASSIGN },
        { "1", TokenType::INT},
        { ";", TokenType::SEMICOLON },
        { "}", TokenType::RBRACE },
        { "", TokenType::EndOfLine },

    },

    {
        { "let", TokenType::LET },
        { "add", TokenType::IDENTIFIER }, 
        { "=", TokenType::ASSIGN },
        { "fn", TokenType::FUNCTION }, 
        { "(", TokenType::LPAREN },
        { "x", TokenType::IDENTIFIER },
        { ",", TokenType::COMMA },
        { "y", TokenType::IDENTIFIER }, 
        { ")", TokenType::RPAREN },
        { "{", TokenType::LBRACE }, 
        { "return", TokenType::RETURN },
        { "x", TokenType::IDENTIFIER }, 
        { "+", TokenType::PLUS },
        { "y", TokenType::IDENTIFIER },
        { "}", TokenType::RBRACE },
        { ";", TokenType::SEMICOLON },
        { "", TokenType::EndOfLine }
    },

    {
        {"-", TokenType::MINUES },
        {"a", TokenType::IDENTIFIER }, 
        {"*", TokenType::PRODUCT }, 
        {"b", TokenType::IDENTIFIER }, 
        {";", TokenType::SEMICOLON }, 
        {"", TokenType::EndOfLine }, 
    },

    {
        {"-", TokenType::MINUES },
        {"a", TokenType::IDENTIFIER }, 
        {"*", TokenType::PRODUCT }, 
        {"b", TokenType::IDENTIFIER }, 
        {"", TokenType::EndOfLine }, 
    },

    {
        {"", TokenType::EndOfLine}
    },

    {
        {"a", TokenType::IDENTIFIER},
        {"*", TokenType::PRODUCT},
        {"b", TokenType::IDENTIFIER},
        {";", TokenType::SEMICOLON},
        {"", TokenType::EndOfLine},
    }
};

static bool verify(const vector<Token>& tokens, int i) { 
    auto r = res[i];
    return tokens == r; 
}

int main() {

    auto i = 0;
    for(auto t : tests) { 
        vector<Token> tokens;

        auto l  = unique_ptr<Lex>(new Lex(t)); 

        while(true) {
            auto token = l->getNextToken();
            tokens.push_back(token);

            if(token.type == TokenType::EndOfLine) { 
                break; 
            }
        }

        if(verify(tokens, i)) {
            std::cout << "Test Case " << i << " Succeed" << std::endl;
        } else {
            std::cout << "Test Case " << i << " Failed" << std::endl;
        }
        i++;
       
    }
    return 0;
}
