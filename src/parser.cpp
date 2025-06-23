#include "parser.h"
#include <iostream>

Parser::Parser(const std::string& input) : lexer(input), currentToken(TokenType::ILLEGAL, "", 1, 1), peekToken(TokenType::ILLEGAL, "", 1, 1) {
    nextToken();
    nextToken();
}

void Parser::nextToken() {
    currentToken = peekToken;
    peekToken = lexer.nextToken();
}

bool Parser::expectPeek(TokenType type) {
    if (peekToken.type == type) {
        nextToken();
        return true;
    } else {
        addError("Expected " + tokenTypeToString(type) + ", got " + tokenTypeToString(peekToken.type));
        return false;
    }
}

void Parser::addError(const std::string& message) {
    errors.push_back("Line " + std::to_string(currentToken.line) + ": " + message);
}

std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    
    while (currentToken.type != TokenType::END_OF_FILE) {
        auto stmt = parseStatement();
        if (stmt) {
            program->statements.push_back(std::move(stmt));
        }
        nextToken();
    }
    
    return program;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    switch (currentToken.type) {
        case TokenType::LET:
            return parseLetStatement();
        case TokenType::FUNCTION:
            return parseFunctionDeclaration();
        case TokenType::ONCLICK:
            return parseOnClickStatement();
        default:
            return parseExpressionStatement();
    }
}

std::unique_ptr<LetStatement> Parser::parseLetStatement() {
    if (!expectPeek(TokenType::IDENTIFIER)) {
        return nullptr;
    }
    
    std::string name = currentToken.literal;
    
    if (!expectPeek(TokenType::EQUALS)) {
        return nullptr;
    }
    
    nextToken();
    auto value = parseExpression();
    
    if (peekToken.type == TokenType::SEMICOLON) {
        nextToken();
    }
    
    return std::make_unique<LetStatement>(name, std::move(value));
}

std::unique_ptr<FunctionDeclaration> Parser::parseFunctionDeclaration() {
    if (!expectPeek(TokenType::IDENTIFIER)) {
        return nullptr;
    }
    
    auto func = std::make_unique<FunctionDeclaration>(currentToken.literal);
    
    if (!expectPeek(TokenType::OPEN_PAREN)) {
        return nullptr;
    }
    
    // Parse parameters
    if (peekToken.type != TokenType::CLOSE_PAREN) {
        nextToken();
        func->parameters.push_back(currentToken.literal);
        
        while (peekToken.type == TokenType::COMMA) {
            nextToken();
            nextToken();
            func->parameters.push_back(currentToken.literal);
        }
    }
    
    if (!expectPeek(TokenType::CLOSE_PAREN)) {
        return nullptr;
    }
    
    if (!expectPeek(TokenType::OPEN_BRACE)) {
        return nullptr;
    }
    
    func->body = parseBlockStatement();
    
    return func;
}

std::unique_ptr<OnClickStatement> Parser::parseOnClickStatement() {
    if (!expectPeek(TokenType::OPEN_PAREN)) {
        return nullptr;
    }
    
    if (!expectPeek(TokenType::STRING)) {
        return nullptr;
    }
    
    std::string elementId = currentToken.literal;
    
    if (!expectPeek(TokenType::CLOSE_PAREN)) {
        return nullptr;
    }
    
    if (!expectPeek(TokenType::OPEN_BRACE)) {
        return nullptr;
    }
    
    auto body = parseBlockStatement();
    
    return std::make_unique<OnClickStatement>(elementId, std::move(body));
}

std::unique_ptr<ExpressionStatement> Parser::parseExpressionStatement() {
    auto expr = parseExpression();
    
    if (peekToken.type == TokenType::SEMICOLON) {
        nextToken();
    }
    
    return std::make_unique<ExpressionStatement>(std::move(expr));
}

std::unique_ptr<BlockStatement> Parser::parseBlockStatement() {
    auto block = std::make_unique<BlockStatement>();
    
    nextToken();
    
    while (currentToken.type != TokenType::CLOSE_BRACE && currentToken.type != TokenType::END_OF_FILE) {
        auto stmt = parseStatement();
        if (stmt) {
            block->statements.push_back(std::move(stmt));
        }
        nextToken();
    }
    
    return block;
}

std::unique_ptr<Expression> Parser::parseExpression(int precedence) {
    auto left = parsePrimaryExpression();
    
    while (peekToken.type != TokenType::SEMICOLON && getOperatorPrecedence(peekToken.type) > precedence) {
        TokenType op = peekToken.type;
        nextToken();
        
        std::string operator_ = currentToken.literal;
        int rightPrec = getOperatorPrecedence(op);
        
        nextToken();
        auto right = parseExpression(rightPrec);
        
        left = std::make_unique<BinaryExpression>(std::move(left), operator_, std::move(right));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parsePrimaryExpression() {
    switch (currentToken.type) {
        case TokenType::NUMBER: {
            double value = std::stod(currentToken.literal);
            return std::make_unique<NumberLiteral>(value);
        }
        case TokenType::STRING: {
            return std::make_unique<StringLiteral>(currentToken.literal);
        }
        case TokenType::IDENTIFIER: {
            auto ident = std::make_unique<Identifier>(currentToken.literal);
            
            if (peekToken.type == TokenType::OPEN_PAREN) {
                return parseCallExpression(std::move(ident));
            }
            
            return ident;
        }
        case TokenType::OPEN_PAREN: {
            nextToken();
            auto expr = parseExpression();
            
            if (!expectPeek(TokenType::CLOSE_PAREN)) {
                return nullptr;
            }
            
            return expr;
        }
        default:
            addError("Unexpected token: " + currentToken.literal);
            return nullptr;
    }
}

std::unique_ptr<Expression> Parser::parseCallExpression(std::unique_ptr<Expression> function) {
    auto call = std::make_unique<CallExpression>(std::move(function));
    
    nextToken(); // consume '('
    
    if (currentToken.type != TokenType::CLOSE_PAREN) {
        call->arguments.push_back(parseExpression());
        
        while (peekToken.type == TokenType::COMMA) {
            nextToken();
            nextToken();
            call->arguments.push_back(parseExpression());
        }
    }
    
    if (!expectPeek(TokenType::CLOSE_PAREN)) {
        return nullptr;
    }
    
    return call;
}

int Parser::getOperatorPrecedence(TokenType type) {
    switch (type) {
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 1;
        case TokenType::STAR:
        case TokenType::SLASH:
            return 2;
        default:
            return 0;
    }
}