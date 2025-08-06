#include "xl-formula/functions.h"
#include <chrono>
#include <ctime>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief DAY function - returns the day from a date
 * @param args Function arguments (expects 1 date argument)
 * @param context Evaluation context (unused for DAY)
 * @return Day as numeric value (1-31)
 */
Value day(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }
    
    // Validate argument count
    if (args.size() != 1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // Check if argument is a date
    if (!args[0].isDate()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    try {
        auto date_val = args[0].asDate();
        auto time_t = std::chrono::system_clock::to_time_t(date_val);
        auto local_tm = *std::localtime(&time_t);
        
        return Value(static_cast<double>(local_tm.tm_mday));
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula