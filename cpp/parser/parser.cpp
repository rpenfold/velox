#include "xl-formula/parser.h"
#include <algorithm>

namespace xl_formula {

ParseResult Parser::parse(const std::string& input) {
    // Reset parser state
    current_token_index_ = 0;
    errors_.clear();

    // Tokenize input
    Lexer lexer(input);
    tokens_ = lexer.tokenize();

    if (tokens_.empty() || tokens_[0].type == TokenType::EOF_TOKEN) {
        return ParseResult(std::vector<ParseError>{ParseError("Empty input", 0)});
    }

    try {
        auto ast = parseExpression();

        // Check if we consumed all tokens (except EOF)
        if (currentToken().type != TokenType::EOF_TOKEN) {
            error("Unexpected token after expression: " + currentToken().value);
        }

        if (!errors_.empty()) {
            return ParseResult(errors_);
        }

        return ParseResult(std::move(ast));
    } catch (const std::exception& e) {
        errors_.push_back(ParseError(e.what(), currentToken().position));
        return ParseResult(errors_);
    }
}

const Token& Parser::currentToken() const {
    if (current_token_index_ >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", 0, 0);
        return eof_token;
    }
    return tokens_[current_token_index_];
}

const Token& Parser::peekToken(size_t offset) const {
    size_t peek_index = current_token_index_ + offset;
    if (peek_index >= tokens_.size()) {
        static Token eof_token(TokenType::EOF_TOKEN, "", 0, 0);
        return eof_token;
    }
    return tokens_[peek_index];
}

void Parser::advance() {
    if (current_token_index_ < tokens_.size()) {
        current_token_index_++;
    }
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    return currentToken().type == type;
}

void Parser::error(const std::string& message) {
    errors_.push_back(ParseError(message, currentToken().position, currentToken().length));
}

void Parser::synchronize() {
    advance();

    while (currentToken().type != TokenType::EOF_TOKEN) {
        switch (currentToken().type) {
            case TokenType::COMMA:
            case TokenType::RIGHT_PAREN:
            case TokenType::SEMICOLON:
                return;
            default:
                advance();
        }
    }
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseComparison();
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    auto expr = parseConcatenation();

    while (true) {
        BinaryOpNode::Operator op;
        if (match(TokenType::EQUAL)) {
            op = BinaryOpNode::Operator::EQUAL;
        } else if (match(TokenType::NOT_EQUAL)) {
            op = BinaryOpNode::Operator::NOT_EQUAL;
        } else if (match(TokenType::LESS_THAN)) {
            op = BinaryOpNode::Operator::LESS_THAN;
        } else if (match(TokenType::LESS_EQUAL)) {
            op = BinaryOpNode::Operator::LESS_EQUAL;
        } else if (match(TokenType::GREATER_THAN)) {
            op = BinaryOpNode::Operator::GREATER_THAN;
        } else if (match(TokenType::GREATER_EQUAL)) {
            op = BinaryOpNode::Operator::GREATER_EQUAL;
        } else {
            break;
        }

        auto right = parseConcatenation();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseConcatenation() {
    auto expr = parseAddition();

    while (match(TokenType::CONCAT)) {
        auto right = parseAddition();
        expr = std::make_unique<BinaryOpNode>(BinaryOpNode::Operator::CONCAT, std::move(expr),
                                              std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseAddition() {
    auto expr = parseMultiplication();

    while (true) {
        BinaryOpNode::Operator op;
        if (match(TokenType::PLUS)) {
            op = BinaryOpNode::Operator::ADD;
        } else if (match(TokenType::MINUS)) {
            op = BinaryOpNode::Operator::SUBTRACT;
        } else {
            break;
        }

        auto right = parseMultiplication();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseMultiplication() {
    auto expr = parsePower();

    while (true) {
        BinaryOpNode::Operator op;
        if (match(TokenType::MULTIPLY)) {
            op = BinaryOpNode::Operator::MULTIPLY;
        } else if (match(TokenType::DIVIDE)) {
            op = BinaryOpNode::Operator::DIVIDE;
        } else {
            break;
        }

        auto right = parsePower();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parsePower() {
    auto expr = parseUnary();

    // Right-associative
    if (match(TokenType::POWER)) {
        auto right = parsePower();  // Recursive for right-associativity
        expr = std::make_unique<BinaryOpNode>(BinaryOpNode::Operator::POWER, std::move(expr),
                                              std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    if (match(TokenType::MINUS)) {
        auto operand = parseUnary();
        return std::make_unique<UnaryOpNode>(UnaryOpNode::Operator::MINUS, std::move(operand));
    }

    if (match(TokenType::PLUS)) {
        auto operand = parseUnary();
        return std::make_unique<UnaryOpNode>(UnaryOpNode::Operator::PLUS, std::move(operand));
    }

    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    // Numbers
    if (check(TokenType::NUMBER)) {
        std::string number_str = currentToken().value;
        advance();
        double value = std::stod(number_str);
        return std::make_unique<LiteralNode>(Value(value));
    }

    // Strings
    if (check(TokenType::STRING)) {
        std::string text = currentToken().value;
        advance();
        return std::make_unique<LiteralNode>(Value(text));
    }

    // Booleans
    if (check(TokenType::BOOLEAN)) {
        std::string bool_str = currentToken().value;
        advance();
        std::transform(bool_str.begin(), bool_str.end(), bool_str.begin(), ::toupper);
        bool value = (bool_str == "TRUE");
        return std::make_unique<LiteralNode>(Value(value));
    }

    // Identifiers (variables or function calls)
    if (check(TokenType::IDENTIFIER)) {
        std::string name = currentToken().value;
        advance();

        // Check if it's a function call
        if (check(TokenType::LEFT_PAREN)) {
            return parseFunctionCall(name);
        } else {
            return std::make_unique<VariableNode>(name);
        }
    }

    // Parenthesized expressions
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        if (!match(TokenType::RIGHT_PAREN)) {
            error("Expected ')' after expression");
            return nullptr;
        }
        return expr;
    }

    // Array literals {value1, value2, value3}
    if (match(TokenType::LEFT_BRACE)) {
        return parseArrayLiteral();
    }

    error("Expected expression");
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseFunctionCall(const std::string& name) {
    if (!match(TokenType::LEFT_PAREN)) {
        error("Expected '(' after function name");
        return nullptr;
    }

    auto arguments = parseArgumentList();

    if (!match(TokenType::RIGHT_PAREN)) {
        error("Expected ')' after function arguments");
        return nullptr;
    }

    return std::make_unique<FunctionCallNode>(name, std::move(arguments));
}

std::vector<std::unique_ptr<ASTNode>> Parser::parseArgumentList() {
    std::vector<std::unique_ptr<ASTNode>> arguments;

    // Handle empty argument list
    if (check(TokenType::RIGHT_PAREN)) {
        return arguments;
    }

    // Parse first argument
    arguments.push_back(parseExpression());

    // Parse remaining arguments
    while (match(TokenType::COMMA)) {
        arguments.push_back(parseExpression());
    }

    return arguments;
}

std::unique_ptr<ASTNode> Parser::parseArrayLiteral() {
    std::vector<std::unique_ptr<ASTNode>> elements;

    // Handle empty array {}
    if (check(TokenType::RIGHT_BRACE)) {
        advance();  // consume '}'
        return std::make_unique<ArrayNode>(std::move(elements));
    }

    // Parse first element
    elements.push_back(parseExpression());

    // Parse remaining elements (comma for horizontal, semicolon for vertical)
    // For now, we'll support both separators but treat them the same
    while (match(TokenType::COMMA) || match(TokenType::SEMICOLON)) {
        elements.push_back(parseExpression());
    }

    if (!match(TokenType::RIGHT_BRACE)) {
        error("Expected '}' after array elements");
        return nullptr;
    }

    return std::make_unique<ArrayNode>(std::move(elements));
}

}  // namespace xl_formula