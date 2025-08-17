#include <gtest/gtest.h>
#include <velox/formulas/parser.h>

using namespace xl_formula;

class LexerTest : public ::testing::Test {
  protected:
    void tokenizeAndCheck(const std::string& input, const std::vector<TokenType>& expected_types) {
        Lexer lexer(input);
        auto tokens = lexer.tokenize();

        ASSERT_EQ(expected_types.size() + 1, tokens.size());  // +1 for EOF

        for (size_t i = 0; i < expected_types.size(); ++i) {
            EXPECT_EQ(expected_types[i], tokens[i].type) << "Token " << i << " in input: " << input;
        }

        EXPECT_EQ(TokenType::EOF_TOKEN, tokens.back().type);
    }
};

TEST_F(LexerTest, Numbers) {
    tokenizeAndCheck("123", {TokenType::NUMBER});
    tokenizeAndCheck("123.45", {TokenType::NUMBER});
    tokenizeAndCheck("0.5", {TokenType::NUMBER});
    tokenizeAndCheck("1e10", {TokenType::NUMBER});
    tokenizeAndCheck("1.5e-3", {TokenType::NUMBER});
}

TEST_F(LexerTest, Strings) {
    tokenizeAndCheck("\"hello\"", {TokenType::STRING});
    tokenizeAndCheck("\"hello world\"", {TokenType::STRING});
    tokenizeAndCheck("\"\"", {TokenType::STRING});
}

TEST_F(LexerTest, Booleans) {
    tokenizeAndCheck("TRUE", {TokenType::BOOLEAN});
    tokenizeAndCheck("FALSE", {TokenType::BOOLEAN});
    tokenizeAndCheck("true", {TokenType::BOOLEAN});
    tokenizeAndCheck("false", {TokenType::BOOLEAN});
}

TEST_F(LexerTest, Identifiers) {
    tokenizeAndCheck("A1", {TokenType::IDENTIFIER});
    tokenizeAndCheck("SUM", {TokenType::IDENTIFIER});
    tokenizeAndCheck("my_var", {TokenType::IDENTIFIER});
}

TEST_F(LexerTest, Operators) {
    tokenizeAndCheck("+", {TokenType::PLUS});
    tokenizeAndCheck("-", {TokenType::MINUS});
    tokenizeAndCheck("*", {TokenType::MULTIPLY});
    tokenizeAndCheck("/", {TokenType::DIVIDE});
    tokenizeAndCheck("^", {TokenType::POWER});
    tokenizeAndCheck("&", {TokenType::CONCAT});
    tokenizeAndCheck("=", {TokenType::EQUAL});
    tokenizeAndCheck("<>", {TokenType::NOT_EQUAL});
    tokenizeAndCheck("!=", {TokenType::NOT_EQUAL});
    tokenizeAndCheck("<", {TokenType::LESS_THAN});
    tokenizeAndCheck("<=", {TokenType::LESS_EQUAL});
    tokenizeAndCheck(">", {TokenType::GREATER_THAN});
    tokenizeAndCheck(">=", {TokenType::GREATER_EQUAL});
}

TEST_F(LexerTest, Delimiters) {
    tokenizeAndCheck("(", {TokenType::LEFT_PAREN});
    tokenizeAndCheck(")", {TokenType::RIGHT_PAREN});
    tokenizeAndCheck(",", {TokenType::COMMA});
    tokenizeAndCheck(";", {TokenType::SEMICOLON});
}

TEST_F(LexerTest, ComplexExpression) {
    tokenizeAndCheck("SUM(A1, B2) + 5", {
                                                TokenType::IDENTIFIER,   // SUM
                                                TokenType::LEFT_PAREN,   // (
                                                TokenType::IDENTIFIER,   // A1
                                                TokenType::COMMA,        // ,
                                                TokenType::IDENTIFIER,   // B2
                                                TokenType::RIGHT_PAREN,  // )
                                                TokenType::PLUS,         // +
                                                TokenType::NUMBER        // 5
                                        });
}

class ParserTest : public ::testing::Test {
  protected:
    Parser parser;

    void parseAndCheckSuccess(const std::string& input) {
        auto result = parser.parse(input);
        EXPECT_TRUE(result.isSuccess()) << "Failed to parse: " << input;
        EXPECT_FALSE(result.hasErrors()) << "Parse errors for: " << input;
        EXPECT_NE(nullptr, result.getAST()) << "No AST generated for: " << input;
    }

    void parseAndCheckError(const std::string& input) {
        auto result = parser.parse(input);
        EXPECT_FALSE(result.isSuccess()) << "Expected parse error for: " << input;
        EXPECT_TRUE(result.hasErrors()) << "No errors reported for: " << input;
    }
};

TEST_F(ParserTest, Literals) {
    parseAndCheckSuccess("123");
    parseAndCheckSuccess("123.45");
    parseAndCheckSuccess("\"hello\"");
    parseAndCheckSuccess("TRUE");
    parseAndCheckSuccess("FALSE");
}

TEST_F(ParserTest, Variables) {
    parseAndCheckSuccess("A1");
    parseAndCheckSuccess("my_variable");
    parseAndCheckSuccess("_test123");
}

TEST_F(ParserTest, BasicArithmetic) {
    parseAndCheckSuccess("1 + 2");
    parseAndCheckSuccess("10 - 5");
    parseAndCheckSuccess("3 * 4");
    parseAndCheckSuccess("15 / 3");
    parseAndCheckSuccess("2 ^ 3");
}

TEST_F(ParserTest, ArithmeticPrecedence) {
    parseAndCheckSuccess("1 + 2 * 3");  // Should be 1 + (2 * 3)
    parseAndCheckSuccess("2 * 3 + 4");  // Should be (2 * 3) + 4
    parseAndCheckSuccess("2 ^ 3 ^ 2");  // Should be 2 ^ (3 ^ 2) (right associative)
}

TEST_F(ParserTest, Parentheses) {
    parseAndCheckSuccess("(1 + 2)");
    parseAndCheckSuccess("(1 + 2) * 3");
    parseAndCheckSuccess("1 + (2 * 3)");
    parseAndCheckSuccess("((1 + 2) * 3)");
}

TEST_F(ParserTest, UnaryOperators) {
    parseAndCheckSuccess("-5");
    parseAndCheckSuccess("+10");
    parseAndCheckSuccess("-(1 + 2)");
    parseAndCheckSuccess("-A1");
}

TEST_F(ParserTest, Comparison) {
    parseAndCheckSuccess("1 = 2");
    parseAndCheckSuccess("1 <> 2");
    parseAndCheckSuccess("1 < 2");
    parseAndCheckSuccess("1 <= 2");
    parseAndCheckSuccess("1 > 2");
    parseAndCheckSuccess("1 >= 2");
}

TEST_F(ParserTest, TextConcatenation) {
    parseAndCheckSuccess("\"hello\" & \" world\"");
    parseAndCheckSuccess("A1 & B1");
    parseAndCheckSuccess("\"Value: \" & A1");
}

TEST_F(ParserTest, FunctionCalls) {
    parseAndCheckSuccess("SUM()");
    parseAndCheckSuccess("SUM(1)");
    parseAndCheckSuccess("SUM(1, 2)");
    parseAndCheckSuccess("SUM(1, 2, 3)");
    parseAndCheckSuccess("MAX(A1, A2, A3)");
    parseAndCheckSuccess("CONCATENATE(\"hello\", \" \", \"world\")");
}

TEST_F(ParserTest, NestedFunctionCalls) {
    parseAndCheckSuccess("SUM(MAX(1, 2), MIN(3, 4))");
    parseAndCheckSuccess("IF(A1 > 0, SUM(A1, A2), 0)");
}

TEST_F(ParserTest, ComplexExpressions) {
    parseAndCheckSuccess("SUM(A1:A10) / COUNT(A1:A10)");
    parseAndCheckSuccess("IF(A1 > 0, A1 * 2, A1 / 2)");
    parseAndCheckSuccess("(A1 + A2) * (B1 - B2) / 100");
    parseAndCheckSuccess("\"Result: \" & (A1 + B1)");
}

TEST_F(ParserTest, ErrorCases) {
    parseAndCheckError("");         // Empty input
    parseAndCheckError("1 +");      // Incomplete expression
    parseAndCheckError("(1 + 2");   // Missing closing parenthesis
    parseAndCheckError("1 + 2)");   // Extra closing parenthesis
    parseAndCheckError("SUM(1,)");  // Trailing comma
    parseAndCheckError("SUM(,1)");  // Leading comma
    // Excel treats '1 ++ 2' as '1 + +2', which is valid
    parseAndCheckSuccess("1 ++ 2");
    parseAndCheckError("1 2");  // Missing operator
}

TEST_F(ParserTest, WhitespaceHandling) {
    parseAndCheckSuccess("  1  +  2  ");
    parseAndCheckSuccess("\t1\n+\r2\r\n");
    parseAndCheckSuccess("SUM( 1 , 2 , 3 )");
}