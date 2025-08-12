#include <algorithm>
#include <cctype>
#include "xl-formula/parser.h"

namespace xl_formula {

Lexer::Lexer(const std::string& input) : input_(input), position_(0), current_char_pos_(0) {
    if (!input_.empty()) {
        current_char_ = input_[0];
    } else {
        current_char_ = '\0';
    }
}

void Lexer::advance() {
    position_++;
    if (position_ >= input_.length()) {
        current_char_ = '\0';
    } else {
        current_char_ = input_[position_];
    }
    current_char_pos_ = position_;
}

void Lexer::skipWhitespace() {
    while (current_char_ != '\0' && std::isspace(current_char_)) {
        advance();
    }
}

char Lexer::peek(size_t offset) const {
    size_t peek_pos = position_ + offset;
    if (peek_pos >= input_.length()) {
        return '\0';
    }
    return input_[peek_pos];
}

Token Lexer::makeNumber() {
    size_t start_pos = position_;
    std::string number_str;

    // Handle digits before decimal point
    while (current_char_ != '\0' && std::isdigit(current_char_)) {
        number_str += current_char_;
        advance();
    }

    // Handle decimal point and digits after
    if (current_char_ == '.') {
        number_str += current_char_;
        advance();

        while (current_char_ != '\0' && std::isdigit(current_char_)) {
            number_str += current_char_;
            advance();
        }
    }

    // Handle scientific notation
    if (current_char_ == 'e' || current_char_ == 'E') {
        number_str += current_char_;
        advance();

        if (current_char_ == '+' || current_char_ == '-') {
            number_str += current_char_;
            advance();
        }

        while (current_char_ != '\0' && std::isdigit(current_char_)) {
            number_str += current_char_;
            advance();
        }
    }

    return Token(TokenType::NUMBER, number_str, start_pos, position_ - start_pos);
}

Token Lexer::makeString() {
    size_t start_pos = position_;
    std::string string_value;

    advance();  // Skip opening quote

    while (current_char_ != '\0' && current_char_ != '"') {
        if (current_char_ == '\\') {
            advance();
            switch (current_char_) {
                case 'n':
                    string_value += '\n';
                    break;
                case 't':
                    string_value += '\t';
                    break;
                case 'r':
                    string_value += '\r';
                    break;
                case '\\':
                    string_value += '\\';
                    break;
                case '"':
                    string_value += '"';
                    break;
                default:
                    string_value += '\\';
                    string_value += current_char_;
                    break;
            }
        } else {
            string_value += current_char_;
        }
        advance();
    }

    if (current_char_ == '"') {
        advance();  // Skip closing quote
    }

    return Token(TokenType::STRING, string_value, start_pos, position_ - start_pos);
}

Token Lexer::makeIdentifier() {
    size_t start_pos = position_;
    std::string identifier;

    while (current_char_ != '\0' && (std::isalnum(current_char_) || current_char_ == '_' || current_char_ == ':')) {
        identifier += current_char_;
        advance();
    }

    // Convert to uppercase for case-insensitive comparison
    std::string upper_identifier = identifier;
    std::transform(upper_identifier.begin(), upper_identifier.end(), upper_identifier.begin(),
                   ::toupper);

    // Check for boolean literals
    TokenType type = TokenType::IDENTIFIER;
    if (upper_identifier == "TRUE" || upper_identifier == "FALSE") {
        // Only treat as boolean literal if not followed by '(' (so TRUE() parses as function)
        if (current_char_ != '(') {
            type = TokenType::BOOLEAN;
        }
    }

    return Token(type, identifier, start_pos, position_ - start_pos);
}

Token Lexer::makeTwoCharToken(TokenType type, char second_char) {
    size_t start_pos = position_;
    advance();  // Skip first character
    if (current_char_ == second_char) {
        advance();  // Skip second character
        return Token(type, std::string(2, second_char), start_pos, 2);
    } else {
        // Backtrack - this shouldn't happen with proper usage
        position_ = start_pos + 1;
        current_char_ = input_.length() > position_ ? input_[position_] : '\0';
        return Token(TokenType::INVALID, "", start_pos, 1);
    }
}

Token Lexer::nextToken() {
    while (current_char_ != '\0') {
        size_t start_pos = position_;

        if (std::isspace(current_char_)) {
            skipWhitespace();
            continue;
        }

        if (std::isdigit(current_char_)) {
            return makeNumber();
        }

        if (current_char_ == '"') {
            return makeString();
        }

        if (std::isalpha(current_char_) || current_char_ == '_') {
            return makeIdentifier();
        }

        // Single character tokens
        switch (current_char_) {
            case '+':
                // Excel treats '1 ++ 2' the same as '1 + + 2', so lex each '+' as PLUS
                advance();
                return Token(TokenType::PLUS, "+", start_pos, 1);
            case '-':
                advance();
                return Token(TokenType::MINUS, "-", start_pos, 1);
            case '*':
                advance();
                return Token(TokenType::MULTIPLY, "*", start_pos, 1);
            case '/':
                advance();
                return Token(TokenType::DIVIDE, "/", start_pos, 1);
            case '^':
                advance();
                return Token(TokenType::POWER, "^", start_pos, 1);
            case '&':
                advance();
                return Token(TokenType::CONCAT, "&", start_pos, 1);
            case '=':
                advance();
                return Token(TokenType::EQUAL, "=", start_pos, 1);
            case '(':
                advance();
                return Token(TokenType::LEFT_PAREN, "(", start_pos, 1);
            case ')':
                advance();
                return Token(TokenType::RIGHT_PAREN, ")", start_pos, 1);
            case '{':
                advance();
                return Token(TokenType::LEFT_BRACE, "{", start_pos, 1);
            case '}':
                advance();
                return Token(TokenType::RIGHT_BRACE, "}", start_pos, 1);
            case ',':
                advance();
                return Token(TokenType::COMMA, ",", start_pos, 1);
            case ';':
                advance();
                return Token(TokenType::SEMICOLON, ";", start_pos, 1);
            case '<':
                if (peek() == '=') {
                    advance();
                    advance();
                    return Token(TokenType::LESS_EQUAL, "<=", start_pos, 2);
                } else if (peek() == '>') {
                    advance();
                    advance();
                    return Token(TokenType::NOT_EQUAL, "<>", start_pos, 2);
                } else {
                    advance();
                    return Token(TokenType::LESS_THAN, "<", start_pos, 1);
                }
            case '>':
                if (peek() == '=') {
                    advance();
                    advance();
                    return Token(TokenType::GREATER_EQUAL, ">=", start_pos, 2);
                } else {
                    advance();
                    return Token(TokenType::GREATER_THAN, ">", start_pos, 1);
                }
            case '!':
                if (peek() == '=') {
                    advance();
                    advance();
                    return Token(TokenType::NOT_EQUAL, "!=", start_pos, 2);
                } else {
                    advance();
                    return Token(TokenType::INVALID, "!", start_pos, 1);
                }
            default:
                advance();
                return Token(TokenType::INVALID, std::string(1, input_[start_pos]), start_pos, 1);
        }
    }

    return Token(TokenType::EOF_TOKEN, "", position_, 0);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = nextToken();

    while (token.type != TokenType::EOF_TOKEN) {
        tokens.push_back(token);
        token = nextToken();
    }

    tokens.push_back(token);  // Add EOF token
    return tokens;
}

}  // namespace xl_formula