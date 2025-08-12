#include "xl-formula/evaluator.h"
#include "xl-formula/parser.h"

namespace xl_formula {

// FormulaEngine implementation
FormulaEngine::FormulaEngine() {
    function_registry_ = FunctionRegistry::createDefault();
}

FormulaEngine::~FormulaEngine() = default;

EvaluationResult FormulaEngine::evaluate(const std::string& formula) {
    Parser parser;
    auto parse_result = parser.parse(formula);

    if (!parse_result.isSuccess()) {
        return EvaluationResult::error(ErrorType::PARSE_ERROR);
    }

    return evaluate(*parse_result.getAST());
}

EvaluationResult FormulaEngine::evaluate(const ASTNode& ast) {
    Evaluator evaluator(context_, function_registry_.get());
    return evaluator.evaluate(ast);
}

EvaluationResult FormulaEngine::evaluate(const std::string& formula,
                                         const std::unordered_map<std::string, Value>& overrides) {
    // Parse first
    Parser parser;
    auto parse_result = parser.parse(formula);
    if (!parse_result.isSuccess()) {
        return EvaluationResult::error(ErrorType::PARSE_ERROR);
    }

    // Snapshot keys that will be overridden and whether they existed
    std::vector<std::pair<std::string, Value>> original_values;
    original_values.reserve(overrides.size());
    std::vector<std::string> keys_to_clear;
    keys_to_clear.reserve(overrides.size());

    // Apply overrides into the engine context
    for (const auto& [name, val] : overrides) {
        Value prior = context_.getVariable(name);
        if (prior.isEmpty()) {
            keys_to_clear.push_back(name);
        } else {
            original_values.emplace_back(name, prior);
        }
        context_.setVariable(name, val);
    }

    // Evaluate with temporary context
    Evaluator evaluator(context_, function_registry_.get());
    auto result = evaluator.evaluate(*parse_result.getAST());

    // Restore original context
    for (const auto& [name, prior] : original_values) {
        context_.setVariable(name, prior);
    }
    for (const auto& name : keys_to_clear) {
        context_.removeVariable(name);
    }

    return result;
}

EvaluationResult FormulaEngine::evaluateWithTrace(const std::string& formula,
                                                  std::unique_ptr<TraceNode>& out_trace_root) {
    Parser parser;
    auto parse_result = parser.parse(formula);

    if (!parse_result.isSuccess()) {
        out_trace_root.reset();
        return EvaluationResult::error(ErrorType::PARSE_ERROR);
    }

    Evaluator evaluator(context_, function_registry_.get());
    return evaluator.evaluateWithTrace(*parse_result.getAST(), out_trace_root);
}

void FormulaEngine::setVariable(const std::string& name, const Value& value) {
    context_.setVariable(name, value);
}

Value FormulaEngine::getVariable(const std::string& name) const {
    return context_.getVariable(name);
}

void FormulaEngine::registerFunction(const std::string& name, const FunctionImpl& impl) {
    function_registry_->registerFunction(name, impl);
}

}  // namespace xl_formula