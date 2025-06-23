#include "token.h"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::PRINT: return "PRINT";
        case TokenType::LET: return "LET";
        case TokenType::FUNCTION: return "FUNCTION";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::ONCLICK: return "ONCLICK";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::OPEN_PAREN: return "OPEN_PAREN";
        case TokenType::CLOSE_PAREN: return "CLOSE_PAREN";
        case TokenType::OPEN_BRACE: return "OPEN_BRACE";
        case TokenType::CLOSE_BRACE: return "CLOSE_BRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::ILLEGAL: return "ILLEGAL";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}