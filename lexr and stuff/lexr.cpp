#include <iostream>
#include <string>

/**
 * TokenType enum defines all the different types of tokens our lexer can recognize.
 * This forms the vocabulary of our language.
 */
enum class TokenType {
    NUMBER,      // Numeric literals like 42, 3.14
    IDENTIFIER,  // Variable names, function names
    EQUALS,      // = sign
    PLUS,        // + sign
    MINUS,       // - sign
    STAR,        // * sign (multiplication)
    SLASH,       // / sign (division)
    OPEN_PAREN,  // Opening parenthesis (
    CLOSE_PAREN, // Closing parenthesis )
    LET,         // 'let' keyword
    ILLEGAL,     // Characters we don't recognize
    END_OF_FILE  // Indicates we've reached the end of input
};

/**
 * Token struct represents a single token extracted from the input.
 * It contains both the type of token and the literal string from input.
 */
struct Token {
    TokenType type;
    std::string literal;
    Token(TokenType t, std::string l) : type(t), literal(l) {}
};

/**
 * Lexer class handles the lexical analysis - breaking input text into tokens.
 * This is the first stage of a compiler pipeline.
 */
class Lexer {
private:
    std::string input;     // The source code to be tokenized
    size_t position;       // Current position in input (points to current char)
    size_t readPosition;   // Current reading position (after current char)
    char ch;               // Current character under examination

    /**
     * readChar advances the pointers and sets the current character.
     * This is our way of "moving forward" in the input text.
     */
    void readChar() {
        if (readPosition >= input.length()) {
            ch = 0;  // ASCII NUL, indicating "end of input"
        } else {
            ch = input[readPosition];
        }
        position = readPosition;  // Current position becomes where we were reading
        readPosition++;           // Advance the reading position
    }

    /**
     * skipWhitespace ignores all whitespace characters.
     * This lets the lexer focus on meaningful tokens only.
     */
    void skipWhitespace() {
        while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            readChar();
        }
    }

    /**
     * readNumber consumes a sequence of digit characters and returns them as a string.
     * This handles numeric literals in the source code.
     */
    std::string readNumber() {
        size_t startPos = position;  // Remember where the number started
        while (isdigit(ch)) {        // Keep reading as long as we see digits
            readChar();
        }
        // Extract the substring from the input that represents this number
        return input.substr(startPos, position - startPos);
    }

    /**
     * readIdentifier consumes a sequence of letter/underscore characters.
     * This handles identifiers (variable names) and keywords.
     */
    std::string readIdentifier() {
        size_t startPos = position;  // Remember where the identifier started
        while (isalpha(ch) || ch == '_') {  // Letters or underscores are valid
            readChar();
        }
        // Extract the substring from the input that forms this identifier
        return input.substr(startPos, position - startPos);
    }

public:
    /**
     * Lexer constructor initializes the lexer with input text.
     * It sets up the initial state and reads the first character.
     */
    Lexer(std::string input) : input(input), position(0), readPosition(0) {
        readChar();  // Prime the pump - read the first character
    }

    /**
     * nextToken examines the current character and returns the appropriate token.
     * This is the main method that clients will call repeatedly to get all tokens.
     */
    Token nextToken() {
        skipWhitespace();  // First skip any whitespace
        
        // Default to ILLEGAL token using current character
        Token tok(TokenType::ILLEGAL, std::string(1, ch));
        
        // Recognize single-character tokens directly
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
            case 0:  // End of file reached
                tok = Token(TokenType::END_OF_FILE, "");
                break;
            default:
                // Handle multi-character tokens
                if (isdigit(ch)) {
                    // For numbers, use a helper method
                    std::string num = readNumber();
                    return Token(TokenType::NUMBER, num);
                } else if (isalpha(ch) || ch == '_') {
                    // For identifiers, use a helper method
                    std::string ident = readIdentifier();
                    // Check if this identifier is actually a keyword
                    if (ident == "let") {
                        return Token(TokenType::LET, ident);
                    }
                    return Token(TokenType::IDENTIFIER, ident);
                }
                // If we don't recognize the character, it remains an ILLEGAL token
        }
        
        // For single-character tokens, advance to the next character
        readChar();
        return tok;
    }
};

/**
 * tokenTypeToString converts a TokenType enum value to a human-readable string.
 * This is useful for debugging and displaying token information.
 */
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
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
        default: return "UNKNOWN";
    }
}

/**
 * main function demonstrates the lexer in action.
 * It tokenizes a sample input and prints each token.
 */
int main() {
    // Sample input to tokenize
    std::string input = "let yourWeight = 10 Ton";
    
    // Create a lexer with our input
    Lexer lexer(input);
    
    // Get the first token
    Token tok = lexer.nextToken();
    
    // Process all tokens until we reach the end
    while (tok.type != TokenType::END_OF_FILE) {
        // Print each token's type and literal value
        std::cout << "Type: " << tokenTypeToString(tok.type)
                  << ", Literal: '" << tok.literal << "'" << std::endl;
        
        // Get the next token
        tok = lexer.nextToken();
    }
    return 0;
}
