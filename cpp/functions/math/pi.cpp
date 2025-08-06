#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief PI function - returns the value of pi
 * @param args Function arguments (should be empty)
 * @param context Evaluation context (unused for PI)
 * @return Value of pi (3.14159265358979323846...)
 */
Value pi(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    // PI takes no arguments
    if (!args.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    return Value(M_PI);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula