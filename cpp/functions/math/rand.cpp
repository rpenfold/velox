#include <random>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief RAND function - returns a random number between 0 and 1
 * @param args Function arguments (should be empty)
 * @param context Evaluation context (unused for RAND)
 * @return Random number between 0 and 1
 */
Value rand_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // RAND takes no arguments
    if (!args.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Create random number generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0, 1.0);

    return Value(dis(gen));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula