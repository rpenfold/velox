#include "xl-formula/xl-formula.h"

namespace xl_formula {

EvaluationResult evaluate(const std::string& formula, const Context& context) {
    FormulaEngine engine;

    // Copy context variables to engine
    for (const auto& name : context.getVariableNames()) {
        engine.setVariable(name, context.getVariable(name));
    }

    return engine.evaluate(formula);
}

ParseResult parse(const std::string& formula) {
    Parser parser;
    return parser.parse(formula);
}

}  // namespace xl_formula