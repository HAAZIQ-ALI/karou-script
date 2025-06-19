#include <iostream>
#include <string>

enum class TokenType {
    NUMBER,
    IDENTIFIER,
    STRING,
    PRINT,
    EQUALS,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    OPEN_PAREN,
    CLOSE_PAREN,
    LET,
    SEMICOLON,
    ILLEGAL,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string literal;
    Token(TokenType t, std::string l) : type(t), literal(l) {}
};

class Lexer {
private:
    std::string input;
    size_t position;
    size_t readPosition;
    char ch;

    void readChar() {
        if (readPosition >= input.length()) {
            ch = 0;
        } else {
            ch = input[readPosition];
        }
        position = readPosition;
        readPosition++;
    }

    void skipWhitespace() {
        while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            readChar();
        }
    }

    std::string readNumber() {
        size_t startPos = position;
        while (isdigit(ch)) {
            readChar();
        }
        return input.substr(startPos, position - startPos);
    }

    std::string readIdentifier() {
        size_t startPos = position;
        while (isalpha(ch) || ch == '_') {
            readChar();
        }
        return input.substr(startPos, position - startPos);
    }
     
    std::string readString() {
        readChar();
        
        size_t startPos = position;
        while (ch != '"' && ch != 0) {
            readChar();
        }
        
        std::string str = input.substr(startPos, position - startPos);
        
        if (ch == '"') {
            readChar();
        }
        
        return str;
    }

public:
    Lexer(std::string input) : input(input), position(0), readPosition(0) {
        readChar();
    }

    Token nextToken() {
        skipWhitespace();
        
        Token tok(TokenType::ILLEGAL, std::string(1, ch));
        
        switch (ch) {
        case '=':
            tok = Token(TokenType::EQUALS, "=");
            break;
        case '+':
            tok = Token(TokenType::PLUS, "+");
            break;
        case '-':
            tok = Token(TokenType::MINUS, "-");
            break;
        case '*':
            tok = Token(TokenType::STAR, "*");
            break;
        case '/':
            tok = Token(TokenType::SLASH, "/");
            break;
        case '(':
            tok = Token(TokenType::OPEN_PAREN, "(");
            break;
        case ')':
            tok = Token(TokenType::CLOSE_PAREN, ")");
            break;
        case ';':
            tok = Token(TokenType::SEMICOLON, ";");
            break;
        case '"':
            return Token(TokenType::STRING, readString());
        case 0:
            tok = Token(TokenType::END_OF_FILE, "");
            break;
        default:
            if (isdigit(ch)) {
                std::string num = readNumber();
                return Token(TokenType::NUMBER, num);
            } else if (isalpha(ch) || ch == '_') {
                std::string ident = readIdentifier();
                if (ident == "let") {
                    return Token(TokenType::LET, ident);
                } else if (ident == "print") {
                    return Token(TokenType::PRINT, ident);
                } else {
                    return Token(TokenType::IDENTIFIER, ident);
                }
            } else {
                return Token(TokenType::ILLEGAL, std::string(1, ch));
            }
        }
        
        readChar();
        return tok;
    }
};

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::PRINT: return "PRINT";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::OPEN_PAREN: return "OPEN_PAREN";
        case TokenType::CLOSE_PAREN: return "CLOSE_PAREN";
        case TokenType::LET: return "LET";
        case TokenType::ILLEGAL: return "ILLEGAL";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::STRING: return "STRING";
        case TokenType::SEMICOLON: return "SEMICOLON";
        default: return "UNKNOWN";
    }
}

int main() {
    std::string input = "pr1int(\"hi\");";

    Lexer lexer(input);
    
    Token tok = lexer.nextToken();

    while (tok.type != TokenType::END_OF_FILE) {
        std::cout << "Type: " << tokenTypeToString(tok.type)
                  << ", Literal: '" << tok.literal << "'" << std::endl;
        
        tok = lexer.nextToken();
    }
    return 0;
}

