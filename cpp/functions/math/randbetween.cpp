#include <cmath>
#include <random>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief RANDBETWEEN function - returns a random number between specified numbers
 * @param args Function arguments (expects 2 numeric arguments)
 * @param context Evaluation context (unused for RANDBETWEEN)
 * @return Random integer between bottom and top (inclusive)
 */
Value randbetween(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    // Validate argument count
    if (args.size() != 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Convert arguments to numbers
    if (!args[0].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    if (!args[1].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Get integer values (Excel RANDBETWEEN works with integers)
    int bottom = static_cast<int>(std::floor(args[0].toNumber()));
    int top = static_cast<int>(std::floor(args[1].toNumber()));

    // Validate range
    if (bottom > top) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    // Create random number generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(bottom, top);

    return Value(static_cast<double>(dis(gen)));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula