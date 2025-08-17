#pragma once

#include <memory>
#include <string>
#include <vector>
#include "types.h"

namespace xl_formula {

/**
 * @brief Base class for all AST nodes
 */
class ASTNode {
  public:
    virtual ~ASTNode() = default;

    /**
     * @brief Accept method for visitor pattern
     * @param visitor The visitor to accept
     */
    virtual void accept(class ASTVisitor& visitor) = 0;

    /**
     * @brief Get string representation of the node (for debugging)
     * @return String representation
     */
    virtual std::string toString() const = 0;
};

/**
 * @brief Literal value node (numbers, strings, booleans)
 */
class LiteralNode : public ASTNode {
  private:
    Value value_;

  public:
    explicit LiteralNode(const Value& value) : value_(value) {}

    const Value& getValue() const {
        return value_;
    }

    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Variable reference node
 */
class VariableNode : public ASTNode {
  private:
    std::string name_;

  public:
    explicit VariableNode(const std::string& name) : name_(name) {}

    const std::string& getName() const {
        return name_;
    }

    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Binary operation node (+, -, *, /, etc.)
 */
class BinaryOpNode : public ASTNode {
  public:
    enum class Operator {
        ADD,           // +
        SUBTRACT,      // -
        MULTIPLY,      // *
        DIVIDE,        // /
        POWER,         // ^
        CONCAT,        // &
        EQUAL,         // =
        NOT_EQUAL,     // <>
        LESS_THAN,     // <
        LESS_EQUAL,    // <=
        GREATER_THAN,  // >
        GREATER_EQUAL  // >=
    };

  private:
    Operator operator_;
    std::unique_ptr<ASTNode> left_;
    std::unique_ptr<ASTNode> right_;

  public:
    BinaryOpNode(Operator op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : operator_(op), left_(std::move(left)), right_(std::move(right)) {}

    Operator getOperator() const {
        return operator_;
    }
    const ASTNode& getLeft() const {
        return *left_;
    }
    const ASTNode& getRight() const {
        return *right_;
    }

    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;

    static std::string operatorToString(Operator op);
};

/**
 * @brief Unary operation node (-, +)
 */
class UnaryOpNode : public ASTNode {
  public:
    enum class Operator {
        PLUS,  // +
        MINUS  // -
    };

  private:
    Operator operator_;
    std::unique_ptr<ASTNode> operand_;

  public:
    UnaryOpNode(Operator op, std::unique_ptr<ASTNode> operand)
        : operator_(op), operand_(std::move(operand)) {}

    Operator getOperator() const {
        return operator_;
    }
    const ASTNode& getOperand() const {
        return *operand_;
    }

    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Array literal node [value1, value2, value3]
 */
class ArrayNode : public ASTNode {
  private:
    std::vector<std::unique_ptr<ASTNode>> elements_;

  public:
    explicit ArrayNode(std::vector<std::unique_ptr<ASTNode>> elements)
        : elements_(std::move(elements)) {}

    const std::vector<std::unique_ptr<ASTNode>>& getElements() const {
        return elements_;
    }

    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Function call node
 */
class FunctionCallNode : public ASTNode {
  private:
    std::string name_;
    std::vector<std::unique_ptr<ASTNode>> arguments_;

  public:
    FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> arguments)
        : name_(name), arguments_(std::move(arguments)) {}

    const std::string& getName() const {
        return name_;
    }
    const std::vector<std::unique_ptr<ASTNode>>& getArguments() const {
        return arguments_;
    }

    void accept(ASTVisitor& visitor) override;
    std::string toString() const override;
};

/**
 * @brief Visitor interface for AST traversal
 */
class ASTVisitor {
  public:
    virtual ~ASTVisitor() = default;

    virtual void visit(const LiteralNode& node) = 0;
    virtual void visit(const VariableNode& node) = 0;
    virtual void visit(const BinaryOpNode& node) = 0;
    virtual void visit(const UnaryOpNode& node) = 0;
    virtual void visit(const ArrayNode& node) = 0;
    virtual void visit(const FunctionCallNode& node) = 0;
};

}  // namespace xl_formula