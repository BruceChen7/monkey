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
    {TokenType::LPAREN, Priority::CALL},
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

    if(!expectPeek(TokenType::ASSIGN)) { 
        return nullptr;
    } 
    nextToken();
    auto value = parseExpression(Priority::LOWEST);
    ls->setExpression(value);

    if(peekTokenIs(TokenType::SEMICOLON)) {
        nextToken();
    } 
    return ls;
}


ReturnStatement* Parser::parseReturnStatement() { 
    auto t = cur_token_;
    nextToken();
    auto v = parseExpression(Priority::LOWEST);

    auto rs = new ReturnStatement(t, v);
    if(peekTokenIs(TokenType::SEMICOLON)) {
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

void Parser::noPrefixParserFnError(TokenType t) { 
    stringstream ss;
    ss << "no prefix parse function for " << convertTokenType(t) << " found" << endl;
    errors_.push_back(ss.str());
}

Expression* Parser::parseExpression(Priority p) { 
    auto pre_it = pre_parser_fn_.find(static_cast<int>(cur_token_.type));

    if(pre_it == pre_parser_fn_.end()) {
        noPrefixParserFnError(cur_token_.type);
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
    return new PrefixExpression(t, t.value, right);
}
   

Expression* Parser::parseInprefixExpression(Expression* left) { 
    auto t = cur_token_;
    Priority curr_pri = currentPrioriy();
    nextToken();
    Expression* right = parseExpression(curr_pri);
    return new InfixExpression(t, t.value, left, right);
}

Expression* Parser::parseBooleanExpression() { 
    bool v = (cur_token_.value == "true" ? true : false);
    return new Boolean(cur_token_, v);
}
   
Expression* Parser::parseGroupedExpression() { 
    nextToken(); // skip (
    auto expr = parseExpression(Priority::LOWEST);

    if (!expectPeek(TokenType::RPAREN)) {
        return nullptr;
    }
    return expr;
}

Expression* Parser::parseIfExpression() {
    auto if_token = cur_token_;

    if(!expectPeek(TokenType::LPAREN)) {
        return nullptr;
    }
    nextToken(); 
    auto condition_expr = parseExpression(Priority::LOWEST);

    if(!expectPeek(TokenType::RPAREN)) { 
        return nullptr;
    }

    if(!expectPeek(TokenType::LBRACE)) { 
        return nullptr;
    }

    auto cons = parseBlockStatement(); 
    BlockStatement* alter = nullptr;

    if(peekTokenIs(TokenType::ELSE)) {
        nextToken(); 
        if(!expectPeek(TokenType::LBRACE)) {
            return nullptr;
        } 
        alter = parseBlockStatement();

    }
    return new IfExpression(if_token, condition_expr, cons, alter);
}

BlockStatement* Parser::parseBlockStatement() { 
    auto t = cur_token_;
    nextToken(); 
    auto bs = new BlockStatement(t);


    while(!currentTokenIs(TokenType::RBRACE)) { 
        bs->addStatement(parseStatement());
        nextToken();
    }
    return bs;
} 

Expression* Parser::parseFunctionLiteral() { 
    auto t = cur_token_; 

    if(!expectPeek(TokenType::LPAREN)) { 
        return nullptr;
    } 
    auto param = parseFunctionParams(); 

    if(!expectPeek(TokenType::LBRACE)) { 
        return nullptr;
    }

    auto blocks = parseBlockStatement();
    auto func = new FunctionLiteral(t, param, blocks); 
    return func;
}

vector<IdentifierNode*> Parser::parseFunctionParams() {
    vector<IdentifierNode*> param;
    // no params
    if(peekTokenIs(TokenType::RPAREN)) { 
        nextToken();
        return param;
    } 
    // skip "("
    nextToken(); 
    IdentifierNode* ident = new IdentifierNode(cur_token_, cur_token_.value); 
    param.push_back(ident);

    while(peekTokenIs(TokenType::COMMA)) {
        // skip current param
        nextToken();
        // skip ","
        nextToken();
        IdentifierNode* i= new IdentifierNode(cur_token_, cur_token_.value); 
        param.push_back(i); 
    }

    if(!expectPeek(TokenType::RPAREN)) {
        return vector<IdentifierNode*>{};
    } 
    return param;
} 

Expression* Parser::parseCallExpression(Expression* func) { 
    Token t = cur_token_; 
    vector<Expression*> arguments = parseCallArguments(); 
    return new CallExpression(t, arguments, func); 
}


vector<Expression*> Parser::parseCallArguments() { 
    vector<Expression*> argument;

    if(peekTokenIs(TokenType::RPAREN)) {
        nextToken();
        return argument;
    }
    nextToken();
    argument.push_back(parseExpression(Priority::LOWEST));

    while(peekTokenIs(TokenType::COMMA)) {
        nextToken(); // skip ','
        nextToken();
        argument.push_back(parseExpression(Priority::LOWEST));
    }

    if(!expectPeek(TokenType::RPAREN)) {
        return vector<Expression*> {};
    }
    return argument; 
}
