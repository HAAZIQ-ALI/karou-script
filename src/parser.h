#pragma once
#include "lexer.h"
#include "ast.h"
#include <memory>

class Parser {
private:
    Lexer lexer;
    Token currentToken;
    Token peekToken;
    
    void nextToken();
    bool expectPeek(TokenType type);
    
    // Parsing methods
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<LetStatement> parseLetStatement();
    std::unique_ptr<FunctionDeclaration> parseFunctionDeclaration();
    std::unique_ptr<OnClickStatement> parseOnClickStatement();
    std::unique_ptr<ExpressionStatement> parseExpressionStatement();
    std::unique_ptr<BlockStatement> parseBlockStatement();
    
    std::unique_ptr<Expression> parseExpression(int precedence = 0);
    std::unique_ptr<Expression> parsePrimaryExpression();
    std::unique_ptr<Expression> parseCallExpression(std::unique_ptr<Expression> function);
    
    int getOperatorPrecedence(TokenType type);
    
public:
    Parser(const std::string& input);
    std::unique_ptr<Program> parseProgram();
    std::vector<std::string> getErrors() const { return errors; }
    
private:
    std::vector<std::string> errors;
    void addError(const std::string& message);
};