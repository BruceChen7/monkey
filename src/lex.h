#ifndef __LEX__
#define __LEX__
#include <string>
#include <unordered_map>

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
    INVALID,
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
            return "identifier";
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
        case TokenType::INVALID:
            return "invalid";
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

        bool operator==(const Token& t) const {
            return t.value == value && t.type == type;
        
        }

        string strings() const { 
            return "{Type:" + convertTokenType(type) + " Value: " + value + " }";
        }
        Token(const Token& ) = default;
        Token& operator=(const Token& ) = default;
};

class Lex { 
    public:
        static unordered_map<string, TokenType> sReserveWord;

        Lex(const string& input): in_{input}, reading_pos_{0}, 
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

        string readIdentifier() { 
            auto pos = cur_input_pos_;

            while(isLetter(cur_char_)) { 
                readChar();
            }
            return in_.substr(pos, cur_input_pos_ - pos);
        }

        char getChar() {
            return cur_char_;
        }
        
        bool isLetter(char ch) const {
            return ::isalpha(ch) || ch == '_';
        }

        TokenType lookupIdent(const string& s) {
            auto it = sReserveWord.find(s);

            if(it != sReserveWord.end()) { 
                return it->second;
            } else { 
                return TokenType::IDENTIFIER;
            }
        }


        string readNum() {
            auto pos = cur_input_pos_; 

            while(::isdigit(cur_char_)) { 
                readChar();
            }
            return in_.substr(pos, cur_input_pos_ - pos);
        }
        

    public:
        Token getNextToken(); 

    public:
        Lex(const Lex& ) = delete;
        Lex& operator=(const Lex&) = delete;

    public:
        const std::string in_; 
        int cur_input_pos_; 
        int reading_pos_; // current reading input after current char
        char cur_char_; // current examming character
}; 
#endif
