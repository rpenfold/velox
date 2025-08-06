#include "xl-formula/ast.h"
#include <sstream>

namespace xl_formula {

// LiteralNode implementation
void LiteralNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string LiteralNode::toString() const {
    return "Literal(" + value_.toString() + ")";
}

// VariableNode implementation
void VariableNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string VariableNode::toString() const {
    return "Variable(" + name_ + ")";
}

// BinaryOpNode implementation
void BinaryOpNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string BinaryOpNode::toString() const {
    return "BinaryOp(" + operatorToString(operator_) + ", " + left_->toString() + ", " +
           right_->toString() + ")";
}

std::string BinaryOpNode::operatorToString(Operator op) {
    switch (op) {
        case Operator::ADD:
            return "+";
        case Operator::SUBTRACT:
            return "-";
        case Operator::MULTIPLY:
            return "*";
        case Operator::DIVIDE:
            return "/";
        case Operator::POWER:
            return "^";
        case Operator::CONCAT:
            return "&";
        case Operator::EQUAL:
            return "=";
        case Operator::NOT_EQUAL:
            return "<>";
        case Operator::LESS_THAN:
            return "<";
        case Operator::LESS_EQUAL:
            return "<=";
        case Operator::GREATER_THAN:
            return ">";
        case Operator::GREATER_EQUAL:
            return ">=";
        default:
            return "?";
    }
}

// UnaryOpNode implementation
void UnaryOpNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string UnaryOpNode::toString() const {
    std::string op = (operator_ == Operator::PLUS) ? "+" : "-";
    return "UnaryOp(" + op + ", " + operand_->toString() + ")";
}

// FunctionCallNode implementation
void FunctionCallNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

std::string FunctionCallNode::toString() const {
    std::ostringstream oss;
    oss << "FunctionCall(" << name_ << ", [";
    for (size_t i = 0; i < arguments_.size(); ++i) {
        if (i > 0)
            oss << ", ";
        oss << arguments_[i]->toString();
    }
    oss << "])";
    return oss.str();
}

}  // namespace xl_formula