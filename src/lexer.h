#pragma once
#include "token.h"
#include <string>

class Lexer {
private:
    std::string input;
    size_t position;
    size_t readPosition;
    char ch;
    int line;
    int column;

    void readChar();
    void skipWhitespace();
    std::string readNumber();
    std::string readIdentifier();
    std::string readString();
    char peekChar();

public:
    Lexer(const std::string& input);
    Token nextToken();
};