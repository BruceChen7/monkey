#include "lex.h"
#include <cctype>
using namespace std;


unordered_map<string, TokenType> Lex::sReserveWord = { 
    {"fn", TokenType::FUNCTION},
    {"let", TokenType::LET},
    {"return", TokenType::RETURN},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
};

Token Lex::getNextToken() {
    skipWhitespace();
    char ch = getChar();
    Token s;
    switch(ch) {
        case '=': 
            if(peekChar() == '=') { 
                readChar();
                s.set("==",TokenType::EQUAL); 
            }  else {
                s.set("=", TokenType::ASSIGN);
            }
            break;
        case ';':
            s.set(";", TokenType::SEMICOLON);
            break;
        case ',':
            s.set(",", TokenType::COMMA);
            break;
        case '+':
            s.set("+",TokenType::PLUS);
            break;
        case '-':
            s.set("-",TokenType::MINUES);
            break;
        case '*':
            s.set("*",TokenType::PRODUCT);
            break;
        case '/':
            s.set("/",TokenType::DIVIDE);
            break; 
        case '{':
            s.set("{",TokenType::LBRACE);
            break;
        case '}':
            s.set("}",TokenType::RBRACE);
            break;
        case '(':
            s.set("(",TokenType::LPAREN);
            break;
        case ')':
            s.set(")",TokenType::RPAREN);
            break;
        case '!':
            if(peekChar() == '=') { 
                readChar();
                s.set("!=", TokenType::NOTEQUAL);
            } else { 
                s.set("!", TokenType::BANG);
            }
            break; 
        case '>':
            if(peekChar() == '=') {
                readChar();
                s.set(">=", TokenType::GREATEQUAL);
            } else {
                s.set(">", TokenType::ASSIGN);
            }
        case '<':
            if(peekChar() == '=') { 
                s.set("<=", TokenType::LESSEQUAL);
            } else { 
                s.set("<", TokenType::LESS);
            } 
            break;
        case '\0':
            s.set("", TokenType::EndOfLine);
            break;
        default: 
            if(isLetter(ch)) { 
                auto value  = readIdentifier();
                s.set(value, lookupIdent(value));
            } else if(::isdigit(ch)) {
                auto num = readNum(); 
                s.set(num, TokenType::INT);
            } else { 
                s.set(string(1, ch), TokenType::INVALID); 
            }
            return s;
    } 
    readChar();
    return s;
} 
