#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ast.h"
#include "types.h"

namespace xl_formula {

/**
 * @brief Token types for lexical analysis
 */
enum class TokenType {
    // Literals
    NUMBER,   // 123, 45.67
    STRING,   // "hello"
    BOOLEAN,  // TRUE, FALSE

    // Identifiers
    IDENTIFIER,  // variable names, function names

    // Operators
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    POWER,          // ^
    CONCAT,         // &
    EQUAL,          // =
    NOT_EQUAL,      // <> or !=
    LESS_THAN,      // <
    LESS_EQUAL,     // <=
    GREATER_THAN,   // >
    GREATER_EQUAL,  // >=

    // Delimiters
    LEFT_PAREN,   // (
    RIGHT_PAREN,  // )
    COMMA,        // ,
    SEMICOLON,    // ;

    // Special
    EOF_TOKEN,  // End of input
    INVALID     // Invalid token
};

/**
 * @brief Represents a lexical token
 */
struct Token {
    TokenType type;
    std::string value;
    size_t position;
    size_t length;

    Token(TokenType t, const std::string& v, size_t pos, size_t len)
        : type(t), value(v), position(pos), length(len) {}
};

/**
 * @brief Parse error information
 */
struct ParseError {
    std::string message;
    size_t position;
    size_t length;

    ParseError(const std::string& msg, size_t pos, size_t len = 1)
        : message(msg), position(pos), length(len) {}
};

/**
 * @brief Result of parsing operation
 */
class ParseResult {
  private:
    std::unique_ptr<ASTNode> ast_;
    std::vector<ParseError> errors_;
    bool success_;

  public:
    ParseResult() : success_(false) {}
    ParseResult(std::unique_ptr<ASTNode> ast) : ast_(std::move(ast)), success_(true) {}
    ParseResult(const std::vector<ParseError>& errors) : errors_(errors), success_(false) {}

    bool isSuccess() const {
        return success_;
    }
    bool hasErrors() const {
        return !errors_.empty();
    }

    const ASTNode* getAST() const {
        return ast_.get();
    }
    std::unique_ptr<ASTNode> takeAST() {
        return std::move(ast_);
    }

    const std::vector<ParseError>& getErrors() const {
        return errors_;
    }

    void addError(const ParseError& error) {
        errors_.push_back(error);
        success_ = false;
    }
};

/**
 * @brief Lexical analyzer for formula text
 */
class Lexer {
  private:
    std::string input_;
    size_t position_;
    size_t current_char_pos_;
    char current_char_;

    void advance();
    void skipWhitespace();
    char peek(size_t offset = 1) const;

    Token makeNumber();
    Token makeString();
    Token makeIdentifier();
    Token makeTwoCharToken(TokenType type, char second_char);

  public:
    explicit Lexer(const std::string& input);

    /**
     * @brief Get the next token from input
     * @return Next token
     */
    Token nextToken();

    /**
     * @brief Tokenize the entire input
     * @return Vector of all tokens
     */
    std::vector<Token> tokenize();

    /**
     * @brief Get current position in input
     * @return Current position
     */
    size_t getPosition() const {
        return position_;
    }
};

/**
 * @brief Recursive descent parser for formula expressions
 */
class Parser {
  private:
    std::vector<Token> tokens_;
    size_t current_token_index_;
    std::vector<ParseError> errors_;

    const Token& currentToken() const;
    const Token& peekToken(size_t offset = 1) const;
    void advance();
    bool match(TokenType type);
    bool check(TokenType type) const;

    void error(const std::string& message);
    void synchronize();

    // Parsing methods (precedence from lowest to highest)
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseConcatenation();
    std::unique_ptr<ASTNode> parseAddition();
    std::unique_ptr<ASTNode> parseMultiplication();
    std::unique_ptr<ASTNode> parsePower();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ASTNode> parseFunctionCall(const std::string& name);

    std::vector<std::unique_ptr<ASTNode>> parseArgumentList();

  public:
    /**
     * @brief Parse formula text into AST
     * @param input Formula text to parse
     * @return Parse result containing AST or errors
     */
    ParseResult parse(const std::string& input);
};

}  // namespace xl_formula