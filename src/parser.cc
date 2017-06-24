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
    {TokenType::PLUS, Priority::SUM},
    {TokenType::MINUES, Priority::SUM},
    {TokenType::BANG, Priority::PREFIX},
    {TokenType::LESSEQUAL, Priority::LESSGREATER},
    {TokenType::GREATEQUAL, Priority::LESSGREATER},
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
          auto int_p  = static_cast<int>(p);
          auto int_peek_p = static_cast<int>(peekPriority());

          while(!peekTokenIs(TokenType::SEMICOLON) && int_p < int_peek_p) { 
              auto in_fn_iter = in_parser_fn_.find(static_cast<int>(peek_token_.type));

              if(in_fn_iter == in_parser_fn_.end()) { 
                  return left_expr;
              }
              nextToken();
              auto infix_fn = in_fn_iter->second;
              left_expr = infix_fn(left_expr);
              int_peek_p = static_cast<int>(peekPriority());
          } 
          return left_expr;
    } 
} 

Priority Parser::peekPriority() const {
    auto it = sPriority.find(peek_token_.type);

    if (it != sPriority.end()) {
        return it->second;
    } else {
        return Priority::LOWEST;
    }
}

Priority Parser::currentPrioriy() const {
    auto it = sPriority.find(cur_token_.type);

    if (it != sPriority.end()) {
        return it->second;
    } else {
        return Priority::LOWEST;
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
    auto t = cur_token_;
    nextToken();
    auto right = parseExpression(Priority::PREFIX); 
    
    return new PrefixExpression(t,t.value, right);
}
   

Expression* Parser::parseInprefixExpression(Expression* left) { 
    auto t = cur_token_;
    Priority curr_pri = currentPrioriy();
    nextToken();
    Expression* right = parseExpression(curr_pri);
    return new InfixExpression(t, t.value, left, right);
}

