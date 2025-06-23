#pragma once
#include <string>

/**
 * TokenType enum defines all the different types of tokens our lexer can recognize.
 */
enum class TokenType {
    // Literals
    NUMBER,
    IDENTIFIER,
    STRING,
    
    // Keywords
    PRINT,
    LET,
    FUNCTION,
    IF,
    ELSE,
    WHILE,
    RETURN,
    ONCLICK,
    
    // Operators
    EQUALS,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    
    // Delimiters
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACE,
    CLOSE_BRACE,
    SEMICOLON,
    COMMA,
    
    // Special
    ILLEGAL,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string literal;
    int line;
    int column;
    
    Token(TokenType t, std::string l, int ln = 1, int col = 1) 
        : type(t), literal(l), line(ln), column(col) {}
};

std::string tokenTypeToString(TokenType type);