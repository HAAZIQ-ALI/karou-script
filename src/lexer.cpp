#include "lexer.h"
#include <cctype>
#include <unordered_map>

Lexer::Lexer(const std::string& input) 
    : input(input), position(0), readPosition(0), line(1), column(1) {
    readChar();
}

void Lexer::readChar() {
    if (readPosition >= input.length()) {
        ch = 0;
    } else {
        ch = input[readPosition];
    }
    position = readPosition;
    readPosition++;
    
    if (ch == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
}

char Lexer::peekChar() {
    if (readPosition >= input.length()) {
        return 0;
    }
    return input[readPosition];
}

void Lexer::skipWhitespace() {
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        readChar();
    }
}

std::string Lexer::readNumber() {
    size_t startPos = position;
    while (isdigit(ch) || ch == '.') {
        readChar();
    }
    return input.substr(startPos, position - startPos);
}

std::string Lexer::readIdentifier() {
    size_t startPos = position;
    while (isalnum(ch) || ch == '_') {
        readChar();
    }
    return input.substr(startPos, position - startPos);
}

std::string Lexer::readString() {
    readChar(); // Skip opening quote
    size_t startPos = position;
    
    while (ch != '"' && ch != 0) {
        readChar();
    }
    
    std::string str = input.substr(startPos, position - startPos);
    
    if (ch == '"') {
        readChar(); // Skip closing quote
    }
    
    return str;
}

Token Lexer::nextToken() {
    skipWhitespace();
    
    Token tok(TokenType::ILLEGAL, std::string(1, ch), line, column);
    
    switch (ch) {
        case '=':
            tok = Token(TokenType::EQUALS, "=", line, column);
            break;
        case '+':
            tok = Token(TokenType::PLUS, "+", line, column);
            break;
        case '-':
            tok = Token(TokenType::MINUS, "-", line, column);
            break;
        case '*':
            tok = Token(TokenType::STAR, "*", line, column);
            break;
        case '/':
            tok = Token(TokenType::SLASH, "/", line, column);
            break;
        case '(':
            tok = Token(TokenType::OPEN_PAREN, "(", line, column);
            break;
        case ')':
            tok = Token(TokenType::CLOSE_PAREN, ")", line, column);
            break;
        case '{':
            tok = Token(TokenType::OPEN_BRACE, "{", line, column);
            break;
        case '}':
            tok = Token(TokenType::CLOSE_BRACE, "}", line, column);
            break;
        case ';':
            tok = Token(TokenType::SEMICOLON, ";", line, column);
            break;
        case ',':
            tok = Token(TokenType::COMMA, ",", line, column);
            break;
        case '"':
            return Token(TokenType::STRING, readString(), line, column);
        case 0:
            tok = Token(TokenType::END_OF_FILE, "", line, column);
            break;
        default:
            if (isdigit(ch)) {
                return Token(TokenType::NUMBER, readNumber(), line, column);
            } else if (isalpha(ch) || ch == '_') {
                std::string ident = readIdentifier();
                
                // Keywords mapping
                static std::unordered_map<std::string, TokenType> keywords = {
                    {"let", TokenType::LET},
                    {"print", TokenType::PRINT},
                    {"function", TokenType::FUNCTION},
                    {"if", TokenType::IF},
                    {"else", TokenType::ELSE},
                    {"while", TokenType::WHILE},
                    {"return", TokenType::RETURN},
                    {"onClick", TokenType::ONCLICK}
                };
                
                auto it = keywords.find(ident);
                if (it != keywords.end()) {
                    return Token(it->second, ident, line, column);
                } else {
                    return Token(TokenType::IDENTIFIER, ident, line, column);
                }
            }
            break;
    }
    
    readChar();
    return tok;
}