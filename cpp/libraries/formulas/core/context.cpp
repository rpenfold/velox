#include "velox/formulas/types.h"

namespace xl_formula {

// Context implementation
void Context::setVariable(const std::string& name, const Value& value) {
    variables_[name] = value;
}

Value Context::getVariable(const std::string& name) const {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        return it->second;
    }
    return Value::empty();
}

bool Context::hasVariable(const std::string& name) const {
    return variables_.find(name) != variables_.end();
}

void Context::removeVariable(const std::string& name) {
    variables_.erase(name);
}

void Context::clear() {
    variables_.clear();
}

std::vector<std::string> Context::getVariableNames() const {
    std::vector<std::string> names;
    names.reserve(variables_.size());
    for (const auto& pair : variables_) {
        names.push_back(pair.first);
    }
    return names;
}

}  // namespace xl_formula