#include "ast.h"
#include <sstream>

// NumberLiteral
void NumberLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string NumberLiteral::toString() const {
    return std::to_string(value);
}

// StringLiteral
void StringLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string StringLiteral::toString() const {
    return "\"" + value + "\"";
}

// Identifier
void Identifier::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string Identifier::toString() const {
    return name;
}

// BinaryExpression
void BinaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BinaryExpression::toString() const {
    return "(" + left->toString() + " " + operator_ + " " + right->toString() + ")";
}

// CallExpression
void CallExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string CallExpression::toString() const {
    std::string result = function->toString() + "(";
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) result += ", ";
        result += arguments[i]->toString();
    }
    result += ")";
    return result;
}

// ExpressionStatement
void ExpressionStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string ExpressionStatement::toString() const {
    return expression->toString() + ";";
}

// LetStatement
void LetStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string LetStatement::toString() const {
    return "let " + name + " = " + value->toString() + ";";
}

// BlockStatement
void BlockStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BlockStatement::toString() const {
    std::string result = "{\n";
    for (const auto& stmt : statements) {
        result += "  " + stmt->toString() + "\n";
    }
    result += "}";
    return result;
}

// FunctionDeclaration
void FunctionDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string FunctionDeclaration::toString() const {
    std::string result = "function " + name + "(";
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) result += ", ";
        result += parameters[i];
    }
    result += ") " + body->toString();
    return result;
}

// OnClickStatement
void OnClickStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string OnClickStatement::toString() const {
    return "onClick(\"" + elementId + "\") " + body->toString();
}

// Program
void Program::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string Program::toString() const {
    std::string result;
    for (const auto& stmt : statements) {
        result += stmt->toString() + "\n";
    }
    return result;
}