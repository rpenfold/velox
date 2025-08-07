#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <xl-formula/xl-formula.h>
#include <string>
#include <vector>

using namespace emscripten;
using namespace xl_formula;

/**
 * @brief JavaScript-friendly wrapper for the Value class
 */
class JSValue {
private:
    Value value_;

public:
    JSValue() : value_(Value::empty()) {}
    JSValue(const Value& value) : value_(value) {}
    JSValue(double number) : value_(Value(number)) {}
    JSValue(const std::string& text) : value_(Value(text)) {}
    JSValue(bool boolean) : value_(Value(boolean)) {}

    // Type checking
    bool isNumber() const { return value_.getType() == ValueType::NUMBER; }
    bool isText() const { return value_.getType() == ValueType::TEXT; }
    bool isBoolean() const { return value_.getType() == ValueType::BOOLEAN; }
    bool isError() const { return value_.getType() == ValueType::ERROR; }
    bool isEmpty() const { return value_.getType() == ValueType::EMPTY; }
    bool isDate() const { return value_.getType() == ValueType::DATE; }

    // Value getters
    double asNumber() const {
        if (value_.canConvertToNumber()) {
            return value_.toNumber();
        }
        return 0.0;
    }

    std::string asText() const {
        return value_.toString();
    }

    bool asBoolean() const {
        if (value_.isBoolean()) {
            return value_.asBoolean();
        }
        return false;
    }

    std::string getErrorText() const {
        if (value_.getType() == ValueType::ERROR) {
            switch (value_.asError()) {
                case ErrorType::DIV_ZERO: return "#DIV/0!";
                case ErrorType::VALUE_ERROR: return "#VALUE!";
                case ErrorType::REF_ERROR: return "#REF!";
                case ErrorType::NAME_ERROR: return "#NAME?";
                case ErrorType::NUM_ERROR: return "#NUM!";
                case ErrorType::NA_ERROR: return "#N/A";
                case ErrorType::PARSE_ERROR: return "#PARSE!";
                default: return "#ERROR!";
            }
        }
        return "";
    }

    std::string getTypeName() const {
        switch (value_.getType()) {
            case ValueType::NUMBER: return "number";
            case ValueType::TEXT: return "text";
            case ValueType::BOOLEAN: return "boolean";
            case ValueType::DATE: return "date";
            case ValueType::ERROR: return "error";
            case ValueType::ARRAY: return "array";
            case ValueType::EMPTY: return "empty";
            default: return "unknown";
        }
    }

    // Get the underlying Value for internal use
    const Value& getValue() const { return value_; }
    
    // Static factory methods for Emscripten bindings
    static JSValue fromNumber(double value) {
        return JSValue(Value(value));
    }
    
    static JSValue fromText(const std::string& value) {
        return JSValue(Value(value));
    }
    
    static JSValue fromBoolean(bool value) {
        return JSValue(Value(value));
    }
    
    static JSValue createEmpty() {
        return JSValue(Value::empty());
    }
};

/**
 * @brief JavaScript-friendly wrapper for evaluation results
 */
class JSEvaluationResult {
private:
    EvaluationResult result_;

public:
    JSEvaluationResult(const EvaluationResult& result) : result_(result) {}

    bool isSuccess() const { return result_.isSuccess(); }
    bool hasError() const { return !result_.isSuccess(); }

    JSValue getValue() const {
        if (result_.isSuccess()) {
            return JSValue(result_.getValue());
        }
        return JSValue(); // empty value
    }

    std::string getErrorMessage() const {
        if (!result_.isSuccess()) {
            const auto& value = result_.getValue();
            if (value.isError()) {
                switch (value.asError()) {
                    case ErrorType::DIV_ZERO: return "#DIV/0!";
                    case ErrorType::VALUE_ERROR: return "#VALUE!";
                    case ErrorType::REF_ERROR: return "#REF!";
                    case ErrorType::NAME_ERROR: return "#NAME?";
                    case ErrorType::NUM_ERROR: return "#NUM!";
                    case ErrorType::NA_ERROR: return "#N/A";
                    case ErrorType::PARSE_ERROR: return "#PARSE!";
                    default: return "#ERROR!";
                }
            }
        }
        return "";
    }

    std::vector<std::string> getErrors() const {
        return result_.getWarnings();
    }
};

/**
 * @brief JavaScript-friendly wrapper for the FormulaEngine
 */
class JSFormulaEngine {
private:
    FormulaEngine engine_;

public:
    JSFormulaEngine() = default;

    // Variable management
    void setVariable(const std::string& name, const JSValue& value) {
        engine_.setVariable(name, value.getValue());
    }

    void setNumberVariable(const std::string& name, double value) {
        engine_.setVariable(name, Value(value));
    }

    void setTextVariable(const std::string& name, const std::string& value) {
        engine_.setVariable(name, Value(value));
    }

    void setBooleanVariable(const std::string& name, bool value) {
        engine_.setVariable(name, Value(value));
    }

    JSValue getVariable(const std::string& name) const {
        auto context = engine_.getContext();
        if (context.hasVariable(name)) {
            return JSValue(context.getVariable(name));
        }
        return JSValue(); // empty value
    }

    bool hasVariable(const std::string& name) const {
        return engine_.getContext().hasVariable(name);
    }

    void removeVariable(const std::string& name) {
        engine_.getContext().removeVariable(name);
    }

    void clearVariables() {
        engine_.getContext().clear();
    }

    // Formula evaluation
    JSEvaluationResult evaluate(const std::string& formula) {
        return JSEvaluationResult(engine_.evaluate(formula));
    }

    // Quick evaluation functions for convenience
    double evaluateNumber(const std::string& formula) {
        auto result = engine_.evaluate(formula);
        if (result.isSuccess() && result.getValue().canConvertToNumber()) {
            return result.getValue().toNumber();
        }
        return 0.0;
    }

    std::string evaluateText(const std::string& formula) {
        auto result = engine_.evaluate(formula);
        if (result.isSuccess()) {
            return result.getValue().toString();
        }
        return "";
    }

    bool evaluateBoolean(const std::string& formula) {
        auto result = engine_.evaluate(formula);
        if (result.isSuccess() && result.getValue().isBoolean()) {
            return result.getValue().asBoolean();
        }
        return false;
    }
};

/**
 * @brief Standalone evaluation functions for simple use cases
 */
JSEvaluationResult quickEvaluate(const std::string& formula) {
    return JSEvaluationResult(xl_formula::evaluate(formula));
}

double quickEvaluateNumber(const std::string& formula) {
    auto result = xl_formula::evaluate(formula);
    if (result.isSuccess() && result.getValue().canConvertToNumber()) {
        return result.getValue().toNumber();
    }
    return 0.0;
}

std::string quickEvaluateText(const std::string& formula) {
    auto result = xl_formula::evaluate(formula);
    if (result.isSuccess()) {
        return result.getValue().toString();
    }
    return "";
}

bool quickEvaluateBoolean(const std::string& formula) {
    auto result = xl_formula::evaluate(formula);
    if (result.isSuccess() && result.getValue().isBoolean()) {
        return result.getValue().asBoolean();
    }
    return false;
}

std::string getVersion() {
    return Version::toString();
}

// Emscripten bindings
EMSCRIPTEN_BINDINGS(xl_formula) {
    // JSValue class
    class_<JSValue>("Value")
        .constructor<>()
        .class_function("fromNumber", &JSValue::fromNumber)
        .class_function("fromText", &JSValue::fromText)
        .class_function("fromBoolean", &JSValue::fromBoolean)
        .class_function("empty", &JSValue::createEmpty)
        .function("isNumber", &JSValue::isNumber)
        .function("isText", &JSValue::isText)
        .function("isBoolean", &JSValue::isBoolean)
        .function("isError", &JSValue::isError)
        .function("isEmpty", &JSValue::isEmpty)
        .function("isDate", &JSValue::isDate)
        .function("asNumber", &JSValue::asNumber)
        .function("asText", &JSValue::asText)
        .function("asBoolean", &JSValue::asBoolean)
        .function("getErrorText", &JSValue::getErrorText)
        .function("getTypeName", &JSValue::getTypeName);

    // JSEvaluationResult class
    class_<JSEvaluationResult>("EvaluationResult")
        .function("isSuccess", &JSEvaluationResult::isSuccess)
        .function("hasError", &JSEvaluationResult::hasError)
        .function("getValue", &JSEvaluationResult::getValue)
        .function("getErrorMessage", &JSEvaluationResult::getErrorMessage)
        .function("getErrors", &JSEvaluationResult::getErrors);

    // JSFormulaEngine class
    class_<JSFormulaEngine>("FormulaEngine")
        .constructor<>()
        .function("setVariable", &JSFormulaEngine::setVariable)
        .function("setNumberVariable", &JSFormulaEngine::setNumberVariable)
        .function("setTextVariable", &JSFormulaEngine::setTextVariable)
        .function("setBooleanVariable", &JSFormulaEngine::setBooleanVariable)
        .function("getVariable", &JSFormulaEngine::getVariable)
        .function("hasVariable", &JSFormulaEngine::hasVariable)
        .function("removeVariable", &JSFormulaEngine::removeVariable)
        .function("clearVariables", &JSFormulaEngine::clearVariables)
        .function("evaluate", &JSFormulaEngine::evaluate)
        .function("evaluateNumber", &JSFormulaEngine::evaluateNumber)
        .function("evaluateText", &JSFormulaEngine::evaluateText)
        .function("evaluateBoolean", &JSFormulaEngine::evaluateBoolean);

    // Standalone functions
    function("evaluate", &quickEvaluate);
    function("evaluateNumber", &quickEvaluateNumber);
    function("evaluateText", &quickEvaluateText);
    function("evaluateBoolean", &quickEvaluateBoolean);
    function("getVersion", &getVersion);

    // Vector bindings for arrays
    register_vector<std::string>("StringVector");
}