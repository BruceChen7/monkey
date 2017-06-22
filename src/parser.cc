#include "parser.h"
#include "lex.h"
#include "ast.h"


const unordered_map<TokenType, Priority, EnumClassHash> Parser::sPriority = {
    {TokenType::EQUAL, Priority::EQUALS},
    {TokenType::NOTEQUAL, Priority::EQUALS},
    {TokenType::LESS, Priority::LESSGREATER},
    {TokenType::GREAT, Priority::LESSGREATER},
    {TokenType::DIVIDE, Priority::PRODUCT},
    {TokenType::PRODUCT, Priority::PRODUCT},
};

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
            return parseExpressionStatement();
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

ExpressionStatement* Parser::parseExpressionStatement() {
    auto es = new ExpressionStatement(cur_token_);
    es->setExpression(parseExpression(Priority::LOWEST));

    if(peekTokenIs(TokenType::SEMICOLON)) {
        nextToken();
    }
    return es;
}

Expression* Parser::parseExpression(Priority p) { 
    auto pre_it = pre_parser_fn_.find(static_cast<int>(cur_token_.type));

    if(pre_it == pre_parser_fn_.end()) {
        return nullptr;
    } else { 
          auto pre_fn = pre_it->second; 
          auto left_expr = pre_fn();

          // while(peekTokenIs(token.SEMICOLON) && ) { 
          // }

    } 
} 
Priority Parser::peekPriority() const {
    auto it = sPriority.find(peek_token_.type);

    if (it != sPriority.end()) {
        return it->second;
    } else {
        Priority::LOWEST;
    }
}

Priority Parser::currentPrioriy() const {
    auto it = sPriority.find(cur_token_.type);

    if (it != sPriority.end()) {
        return it->second;
    } else {
        Priority::LOWEST;
    } 
}

Expression* Parser::parseIdenifier() { 
    return new IdentifierNode(cur_token_, cur_token_.value);
}

Expression* Parser::parseIntegerLiteral() { 
    auto v = std::atol(cur_token_.value.c_str());
    return new IntegerLiteral(cur_token_, v); 
}

Expression* Parser::parsePrefixExpression() { 
    auto right = parseExpression(Priority::PREFIX); 
    return new PrefixExpression(cur_token_, cur_token_.value, right);
}
   

Expression* Parser::parseInprefixExpression(Expression* ) { 

}

