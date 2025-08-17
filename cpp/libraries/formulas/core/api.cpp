#include "velox/formulas/xl-formula.h"

namespace xl_formula {

EvaluationResult evaluate(const std::string& formula, const Context& context) {
    FormulaEngine engine;

    // Copy context variables to engine
    for (const auto& name : context.getVariableNames()) {
        engine.setVariable(name, context.getVariable(name));
    }

    return engine.evaluate(formula);
}

EvaluationResult evaluate(const std::string& formula,
                          const std::unordered_map<std::string, Value>& variables) {
    FormulaEngine engine;
    return engine.evaluate(formula, variables);
}

ParseResult parse(const std::string& formula) {
    Parser parser;
    return parser.parse(formula);
}

}  // namespace xl_formula