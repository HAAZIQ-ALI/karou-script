#include "interpreter.h"
#include <iostream>
#include <sstream>

Interpreter::Interpreter() {
    environment = std::make_shared<Environment>();
    registerBuiltins();
}

void Interpreter::registerBuiltins() {
    // Built-in functions are handled in CallExpression visitor
}

void Interpreter::interpret(Program& program) {
    program.accept(*this);
}

void Interpreter::print(const Value& value) {
    std::cout << valueToString(value) << std::endl;
}

void Interpreter::registerEventHandler(const std::string& elementId, std::function<void()> handler) {
    eventHandlers[elementId] = handler;
}

void Interpreter::triggerEvent(const std::string& elementId) {
    auto it = eventHandlers.find(elementId);
    if (it != eventHandlers.end()) {
        it->second();
    }
}

std::string Interpreter::valueToString(const Value& value) {
    return std::visit([](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::string>) {
            return v;
        } else if constexpr (std::is_same_v<T, double>) {
            // Remove trailing zeros for cleaner output
            std::string str = std::to_string(v);
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            str.erase(str.find_last_not_of('.') + 1, std::string::npos);
            return str;
        } else if constexpr (std::is_same_v<T, bool>) {
            return v ? "true" : "false";
        }
        return "";
    }, value);
}

double Interpreter::valueToNumber(const Value& value) {
    return std::visit([](const auto& v) -> double {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, double>) {
            return v;
        } else if constexpr (std::is_same_v<T, std::string>) {
            try {
                return std::stod(v);
            } catch (...) {
                return 0.0;
            }
        } else if constexpr (std::is_same_v<T, bool>) {
            return v ? 1.0 : 0.0;
        }
        return 0.0;
    }, value);
}

bool Interpreter::valueToBoolean(const Value& value) {
    return std::visit([](const auto& v) -> bool {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, bool>) {
            return v;
        } else if constexpr (std::is_same_v<T, double>) {
            return v != 0.0;
        } else if constexpr (std::is_same_v<T, std::string>) {
            return !v.empty();
        }
        return false;
    }, value);
}

void Interpreter::visit(NumberLiteral& node) {
    lastValue = node.value;
}

void Interpreter::visit(StringLiteral& node) {
    lastValue = node.value;
}

void Interpreter::visit(Identifier& node) {
    try {
        lastValue = environment->get(node.name);
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        lastValue = 0.0;
    }
}

void Interpreter::visit(BinaryExpression& node) {
    node.left->accept(*this);
    Value leftVal = lastValue;
    
    node.right->accept(*this);
    Value rightVal = lastValue;
    
    if (node.operator_ == "+") {
        // Handle string concatenation
        if (std::holds_alternative<std::string>(leftVal) || std::holds_alternative<std::string>(rightVal)) {
            lastValue = valueToString(leftVal) + valueToString(rightVal);
        } else {
            lastValue = valueToNumber(leftVal) + valueToNumber(rightVal);
        }
    } else if (node.operator_ == "-") {
        lastValue = valueToNumber(leftVal) - valueToNumber(rightVal);
    } else if (node.operator_ == "*") {
        lastValue = valueToNumber(leftVal) * valueToNumber(rightVal);
    } else if (node.operator_ == "/") {
        double rightNum = valueToNumber(rightVal);
        if (rightNum == 0.0) {
            std::cerr << "Runtime error: Division by zero" << std::endl;
            lastValue = 0.0;
        } else {
            lastValue = valueToNumber(leftVal) / rightNum;
        }
    }
}

void Interpreter::visit(CallExpression& node) {
    // Check if it's a built-in function
    if (auto ident = dynamic_cast<Identifier*>(node.function.get())) {
        if (ident->name == "print") {
            if (!node.arguments.empty()) {
                node.arguments[0]->accept(*this);
                print(lastValue);
            }
            lastValue = 0.0; // print returns nothing
            return;
        }
    }
    
    // For now, we don't support user-defined functions
    std::cerr << "Runtime error: Function calls not yet supported" << std::endl;
    lastValue = 0.0;
}

void Interpreter::visit(ExpressionStatement& node) {
    node.expression->accept(*this);
}

void Interpreter::visit(LetStatement& node) {
    node.value->accept(*this);
    environment->define(node.name, lastValue);
}

void Interpreter::visit(BlockStatement& node) {
    // Create new scope
    auto previousEnv = environment;
    environment = std::make_shared<Environment>(environment);
    
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
    
    // Restore previous scope
    environment = previousEnv;
}

void Interpreter::visit(FunctionDeclaration& node) {
    // For now, we'll just store function declarations
    // Full function support would require more complex implementation
    std::cout << "Function '" << node.name << "' declared (not yet executable)" << std::endl;
}

void Interpreter::visit(OnClickStatement& node) {
    // Register the event handler
    registerEventHandler(node.elementId, [this, &node]() {
        node.body->accept(*this);
    });
    
    std::cout << "Event handler registered for element: " << node.elementId << std::endl;
}

void Interpreter::visit(Program& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}