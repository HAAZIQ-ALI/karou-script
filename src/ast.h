#pragma once
#include <memory>
#include <vector>
#include <string>

// Forward declarations
class ASTVisitor;

// Base AST Node
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual std::string toString() const = 0;
};

// Expression nodes
class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
};

class NumberLiteral : public Expression {
public:
    double value;
    NumberLiteral(double val) : value(val) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

class StringLiteral : public Expression {
public:
    std::string value;
    StringLiteral(const std::string& val) : value(val) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

class Identifier : public Expression {
public:
    std::string name;
    Identifier(const std::string& n) : name(n) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::string operator_;
    std::unique_ptr<Expression> right;
    
    BinaryExpression(std::unique_ptr<Expression> l, const std::string& op, std::unique_ptr<Expression> r)
        : left(std::move(l)), operator_(op), right(std::move(r)) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

class CallExpression : public Expression {
public:
    std::unique_ptr<Expression> function;
    std::vector<std::unique_ptr<Expression>> arguments;
    
    CallExpression(std::unique_ptr<Expression> func) : function(std::move(func)) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

// Statement nodes
class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
    ExpressionStatement(std::unique_ptr<Expression> expr) : expression(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

class LetStatement : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> value;
    
    LetStatement(const std::string& n, std::unique_ptr<Expression> val)
        : name(n), value(std::move(val)) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

class BlockStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

class FunctionDeclaration : public Statement {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<BlockStatement> body;
    
    FunctionDeclaration(const std::string& n) : name(n) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

class OnClickStatement : public Statement {
public:
    std::string elementId;
    std::unique_ptr<BlockStatement> body;
    
    OnClickStatement(const std::string& id, std::unique_ptr<BlockStatement> b)
        : elementId(id), body(std::move(b)) {}
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

// Program (root node)
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

// Visitor pattern for AST traversal
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(NumberLiteral& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(CallExpression& node) = 0;
    virtual void visit(ExpressionStatement& node) = 0;
    virtual void visit(LetStatement& node) = 0;
    virtual void visit(BlockStatement& node) = 0;
    virtual void visit(FunctionDeclaration& node) = 0;
    virtual void visit(OnClickStatement& node) = 0;
    virtual void visit(Program& node) = 0;
};