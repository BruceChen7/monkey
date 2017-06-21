#ifndef __LEX__
#define __LEX__
#include <string>
using namespace std;

enum class TokenType { 
    EndOfLine,
    INT, //123
    ASSIGN, // 123
    IDENTIFIER, 
    PLUS,  // +
    MINUES, // -
    PRODUCT, // * 
    DIVIDE, // / 
    GREAT, // >
    LESS, // <
    GREATEQUAL, // >=
    LESSEQUAL, // <= 
    EQUAL,    //  ==
    NOTEQUAL, // != 
    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }
    COMMA, // ,
    SEMICOLON, // ;
    LET, // "let"
    FUNCTION, // function
    BANG, // !
    TRUE,  // true
    FALSE, // false
    IF, // if
    ELSE, // else
    RETURN,  // return
};


static string convertTokenType(TokenType type) { 
    switch(type) {
        case TokenType::EndOfLine:
            return "eof";
        case TokenType::INT:
            return "number";
        case TokenType::ASSIGN:
            return "=";
        case TokenType::IDENTIFIER:
            return "ident";
        case TokenType::PLUS:
            return "+";
        case TokenType::MINUES:
            return "-";
        case TokenType::PRODUCT:
            return "*";
        case TokenType::DIVIDE:
            return "/";
        case TokenType::GREAT:
            return ">";
        case TokenType::GREATEQUAL:
            return ">=";
        case TokenType::LESS:
            return "<";
        case TokenType::LESSEQUAL:
            return ">=";
        case TokenType::EQUAL:
            return "=="; 
        case TokenType::NOTEQUAL:
            return "!="; 
        case TokenType::LPAREN:
            return "("; 
        case TokenType::RPAREN:
            return ")"; 
        case TokenType::LBRACE:
            return "{"; 
        case TokenType::RBRACE:
            return "}"; 
        case TokenType::COMMA:
            return ",";
        case TokenType::SEMICOLON:
            return ";";
        case TokenType::LET:
            return "let"; 
        case TokenType::FUNCTION:
            return "fn"; 
        case TokenType::BANG:
            return "!";
        case TokenType::IF:
            return "if";
        case TokenType::ELSE:
            return "else";
        case TokenType::RETURN:
            return "return";
        default:
            return "unknown";
    }

}

struct Token { 
    public:
        TokenType type;
        std::string value; 

        Token(): type(TokenType::EndOfLine), value("\0") { 
        } 

        Token(const string& s, TokenType t):value{s}, type{t} { 
        }

        void set(const string& s, TokenType t) { 
            type = t;
            value = s;
        } 

        string strings() const { 
            return "{Type:" + convertTokenType(type) + " Value: " + value + " }";
        }
        Token(const Token& ) = default;
        Token& operator=(const Token& ) = default;
};

class Lex { 
    public:
        Lex(const std::string& input): in_{input}, reading_pos_{0}, 
                                       cur_input_pos_{-1} { 
            readChar();
        }
        void readChar() { 
            if (reading_pos_ >= in_.size()) { 
                cur_char_ = '\0';
                return;
            } else {
                cur_char_ = in_[reading_pos_]; 
                cur_input_pos_ = reading_pos_;
                reading_pos_++; 
            }
        }

        void skipWhitespace() { 
            while (cur_char_ == ' ' || cur_char_ == '\t' || cur_char_ =='\n' || cur_char_ == '\r') { 
                readChar();
            }
        }

        char peekChar() { 
            if (reading_pos_ >= in_.size()) { 
                return '\0';
            } else { 
               return in_[reading_pos_];
            } 
        }

        char getChar() {
            return cur_char_;
        }
    public:
        Token getNextToken(); 

    public:
        Lex(const Lex& ) = delete;
        Lex& operator=(const Lex&) = delete;

    private: 
        const std::string in_; 
        int cur_input_pos_; 
        int reading_pos_; // current reading input after current char
        char cur_char_; // current examming character
}; 
#endif
