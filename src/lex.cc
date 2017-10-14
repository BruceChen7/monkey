#include "lex.h"
#include <cctype>
using namespace std;


const unordered_map<string, TokenType> Lex::sReserveWord = { 
    {"fn", TokenType::FUNCTION},
    {"let", TokenType::LET},
    {"return", TokenType::RETURN},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
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
            if(peekChar() != '/') {
                s.set("/",TokenType::DIVIDE);
            } else {
                readChar();
                s.set("", TokenType::EndOfLine);
                readUntilEndOfLine();
            }
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
                s.set(">", TokenType::GREAT);
            }
            break;
        case '<':
            if(peekChar() == '=') { 
                readChar();
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

void Lex::readUntilEndOfLine() {
    while(cur_char_ != '\n' && cur_char_ != '\0') {
        readChar();
    }
    readChar();
}
