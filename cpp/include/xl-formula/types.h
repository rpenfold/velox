#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace xl_formula {

/**
 * @brief Supported value types in the formula system
 */
enum class ValueType {
    NUMBER,   ///< Numeric value (double)
    TEXT,     ///< Text/string value
    BOOLEAN,  ///< Boolean value (true/false)
    DATE,     ///< Date value
    ERROR,    ///< Error value
    ARRAY,    ///< Array value (vector of Values)
    EMPTY     ///< Empty/null value
};

/**
 * @brief Error types that can occur during formula evaluation
 */
enum class ErrorType {
    NONE,         ///< No error
    DIV_ZERO,     ///< Division by zero (#DIV/0!)
    VALUE_ERROR,  ///< Value error (#VALUE!)
    REF_ERROR,    ///< Reference error (#REF!)
    NAME_ERROR,   ///< Name error (#NAME?)
    NUM_ERROR,    ///< Number error (#NUM!)
    NA_ERROR,     ///< Not available error (#N/A)
    PARSE_ERROR   ///< Parse error
};

/**
 * @brief Represents a value in the formula system
 */
class Value {
  public:
    using DateType = std::chrono::system_clock::time_point;
    using ArrayType = std::shared_ptr<std::vector<Value>>;
    using VariantType = std::variant<double, std::string, bool, DateType, ErrorType, ArrayType>;

  private:
    VariantType data_;
    ValueType type_;

  public:
    // Constructors
    Value() : data_(ErrorType::NONE), type_(ValueType::EMPTY) {}
    Value(double num) : data_(num), type_(ValueType::NUMBER) {}
    Value(const std::string& text) : data_(text), type_(ValueType::TEXT) {}
    Value(const char* text) : data_(std::string(text)), type_(ValueType::TEXT) {}
    Value(bool boolean) : data_(boolean), type_(ValueType::BOOLEAN) {}
    Value(const DateType& date) : data_(date), type_(ValueType::DATE) {}
    Value(ErrorType error) : data_(error), type_(ValueType::ERROR) {}

    // Type checking
    ValueType getType() const {
        return type_;
    }
    bool isNumber() const {
        return type_ == ValueType::NUMBER;
    }
    bool isText() const {
        return type_ == ValueType::TEXT;
    }
    bool isBoolean() const {
        return type_ == ValueType::BOOLEAN;
    }
    bool isDate() const {
        return type_ == ValueType::DATE;
    }
    bool isError() const {
        return type_ == ValueType::ERROR;
    }
    bool isArray() const {
        return type_ == ValueType::ARRAY;
    }
    bool isEmpty() const {
        return type_ == ValueType::EMPTY;
    }

    // Value accessors
    double asNumber() const;
    std::string asText() const;
    bool asBoolean() const;
    DateType asDate() const;
    ErrorType asError() const;
    const std::vector<Value>& asArray() const;

    // Conversion utilities
    bool canConvertToNumber() const;
    double toNumber() const;
    std::string toString() const;

    // Comparison operators
    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const {
        return !(*this == other);
    }
    bool operator<(const Value& other) const;
    bool operator<=(const Value& other) const;
    bool operator>(const Value& other) const;
    bool operator>=(const Value& other) const;

    // Array constructor
    Value(const std::vector<Value>& array)
        : data_(std::make_shared<std::vector<Value>>(array)), type_(ValueType::ARRAY) {}

    // Static factory methods
    static Value error(ErrorType type) {
        return Value(type);
    }
    static Value empty() {
        return Value();
    }
    static Value array(const std::vector<Value>& elements) {
        return Value(elements);
    }
};

/**
 * @brief Context for formula evaluation containing variable bindings
 */
class Context {
  private:
    std::unordered_map<std::string, Value> variables_;

  public:
    /**
     * @brief Set a variable value in the context
     * @param name Variable name
     * @param value Variable value
     */
    void setVariable(const std::string& name, const Value& value);

    /**
     * @brief Get a variable value from the context
     * @param name Variable name
     * @return Value if found, empty value otherwise
     */
    Value getVariable(const std::string& name) const;

    /**
     * @brief Check if a variable exists in the context
     * @param name Variable name
     * @return true if variable exists, false otherwise
     */
    bool hasVariable(const std::string& name) const;

    /**
     * @brief Remove a variable from the context
     * @param name Variable name
     */
    void removeVariable(const std::string& name);

    /**
     * @brief Clear all variables from the context
     */
    void clear();

    /**
     * @brief Get all variable names
     * @return Vector of variable names
     */
    std::vector<std::string> getVariableNames() const;
};

}  // namespace xl_formula