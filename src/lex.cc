#include "lex.h"

Token Lex::getNextToken() {
    skipWhitespace();
    char ch = getChar();
    Token s;
    switch(ch) {
        case '=': 
            if(peekChar() == '=') { 
                getChar();
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
            break;
            
    } 
    return s;
}


