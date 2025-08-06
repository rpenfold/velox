#include "xl-formula/evaluator.h"
#include "xl-formula/functions.h"

namespace xl_formula {

std::unique_ptr<FunctionRegistry> FunctionRegistry::createDefault() {
    auto registry = std::make_unique<FunctionRegistry>();

    // Math functions
    registry->registerFunction("SUM", functions::builtin::sum);
    registry->registerFunction("MAX", functions::builtin::max);
    registry->registerFunction("MIN", functions::builtin::min);
    registry->registerFunction("AVERAGE", functions::builtin::average);
    registry->registerFunction("COUNT", functions::builtin::count);
    registry->registerFunction("COUNTA", functions::builtin::counta);
    registry->registerFunction("ABS", functions::builtin::abs_function);
    registry->registerFunction("ROUND", functions::builtin::round_function);
    registry->registerFunction("SQRT", functions::builtin::sqrt_function);
    registry->registerFunction("POWER", functions::builtin::power);
    registry->registerFunction("MOD", functions::builtin::mod);
    registry->registerFunction("PI", functions::builtin::pi);
    registry->registerFunction("SIGN", functions::builtin::sign);
    registry->registerFunction("INT", functions::builtin::int_function);
    registry->registerFunction("TRUNC", functions::builtin::trunc_function);
    registry->registerFunction("CEILING", functions::builtin::ceiling_function);
    registry->registerFunction("FLOOR", functions::builtin::floor_function);
    registry->registerFunction("RAND", functions::builtin::rand_function);
    registry->registerFunction("RANDBETWEEN", functions::builtin::randbetween);
    registry->registerFunction("COUNTIF", functions::builtin::countif);
    registry->registerFunction("MEDIAN", functions::builtin::median);
    registry->registerFunction("MODE", functions::builtin::mode);
    registry->registerFunction("STDEV", functions::builtin::stdev);
    registry->registerFunction("VAR", functions::builtin::var);

    // Text functions
    registry->registerFunction("CONCATENATE", functions::builtin::concatenate);
    registry->registerFunction("TRIM", functions::builtin::trim);
    registry->registerFunction("LEN", functions::builtin::len);
    registry->registerFunction("LEFT", functions::builtin::left);
    registry->registerFunction("RIGHT", functions::builtin::right);
    registry->registerFunction("MID", functions::builtin::mid);
    registry->registerFunction("UPPER", functions::builtin::upper);
    registry->registerFunction("LOWER", functions::builtin::lower);
    registry->registerFunction("PROPER", functions::builtin::proper);

    // Logical functions
    registry->registerFunction("TRUE", functions::builtin::true_function);
    registry->registerFunction("FALSE", functions::builtin::false_function);
    registry->registerFunction("IF", functions::builtin::if_function);

    return registry;
}

}  // namespace xl_formula