#include "parser.h"
#include "lex.h"
#include "ast.h"

Program* Parser::parseProgram() { 
    auto t = cur_token_; 
    Program* p = new Program();
    while(t.type != TokenType::EndOfLine) {

        auto s = parseStatement();
        
        if (s != nullptr) {
            p->addStatements(s);
        }
        nextToken(); 
        t = cur_token_;
    }
    return p;
}

Statement* Parser::parseStatement() { 
    auto type = cur_token_.type;
    switch(type) {
        case TokenType::LET:
            return parseLetStatement();
        case TokenType::RETURN:
            return parseReturnStatement();
        default:
            return nullptr; 
    }
}



LetStatement* Parser::parseLetStatement() { 
    auto ls = new LetStatement(cur_token_);

    if (!expectPeek(TokenType::IDENTIFIER)) {
        return nullptr;
    } 
    auto name = new IdentifierNode(cur_token_, cur_token_.value);
    ls->setName(name);

    // FixMe parse Expression
    while(!currentTokenIs(TokenType::SEMICOLON) && !currentTokenIs(TokenType::EndOfLine)) {
        nextToken();
    } 
    return ls;
}


ReturnStatement* Parser::parseReturnStatement() { 
    auto rs = new ReturnStatement(cur_token_);
    // FixMe parse expression
    while(!currentTokenIs(TokenType::SEMICOLON)) {
        nextToken();
    } 

    return rs;
}



