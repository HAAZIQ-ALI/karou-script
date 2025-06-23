#pragma once
#include "ast.h"
#include <unordered_map>
#include <variant>
#include <functional>

// Value types that our interpreter can handle
using Value = std::variant<double, std::string, bool>;

class Environment {
private:
    std::unordered_map<std::string, Value> variables;
    std::shared_ptr<Environment> parent;

public:
    Environment(std::shared_ptr<Environment> parent = nullptr) : parent(parent) {}
    
    void define(const std::string& name, const Value& value) {
        variables[name] = value;
    }
    
    Value get(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return it->second;
        }
        
        if (parent) {
            return parent->get(name);
        }
        
        throw std::runtime_error("Undefined variable: " + name);
    }
    
    void set(const std::string& name, const Value& value) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            it->second = value;
            return;
        }
        
        if (parent) {
            parent->set(name, value);
            return;
        }
        
        throw std::runtime_error("Undefined variable: " + name);
    }
};

class Interpreter : public ASTVisitor {
private:
    std::shared_ptr<Environment> environment;
    Value lastValue;
    std::unordered_map<std::string, std::function<void()>> eventHandlers;

public:
    Interpreter();
    
    void interpret(Program& program);
    Value getLastValue() const { return lastValue; }
    
    // Built-in functions
    void registerBuiltins();
    void print(const Value& value);
    
    // Event handling
    void registerEventHandler(const std::string& elementId, std::function<void()> handler);
    void triggerEvent(const std::string& elementId);
    
    // Visitor methods
    void visit(NumberLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(Identifier& node) override;
    void visit(BinaryExpression& node) override;
    void visit(CallExpression& node) override;
    void visit(ExpressionStatement& node) override;
    void visit(LetStatement& node) override;
    void visit(BlockStatement& node) override;
    void visit(FunctionDeclaration& node) override;
    void visit(OnClickStatement& node) override;
    void visit(Program& node) override;
    
private:
    std::string valueToString(const Value& value);
    double valueToNumber(const Value& value);
    bool valueToBoolean(const Value& value);
};