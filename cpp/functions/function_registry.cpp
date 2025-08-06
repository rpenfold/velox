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

    // Trigonometric functions
    registry->registerFunction("SIN", functions::builtin::sin_function);
    registry->registerFunction("COS", functions::builtin::cos_function);
    registry->registerFunction("TAN", functions::builtin::tan_function);
    registry->registerFunction("ASIN", functions::builtin::asin_function);
    registry->registerFunction("ACOS", functions::builtin::acos_function);
    registry->registerFunction("ATAN", functions::builtin::atan_function);
    registry->registerFunction("ATAN2", functions::builtin::atan2_function);
    registry->registerFunction("SINH", functions::builtin::sinh_function);
    registry->registerFunction("COSH", functions::builtin::cosh_function);
    registry->registerFunction("TANH", functions::builtin::tanh_function);
    registry->registerFunction("DEGREES", functions::builtin::degrees_function);
    registry->registerFunction("RADIANS", functions::builtin::radians_function);
    registry->registerFunction("EXP", functions::builtin::exp_function);
    registry->registerFunction("LN", functions::builtin::ln_function);
    registry->registerFunction("LOG", functions::builtin::log_function);
    registry->registerFunction("LOG10", functions::builtin::log10_function);

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
    registry->registerFunction("FIND", functions::builtin::find);
    registry->registerFunction("SEARCH", functions::builtin::search);
    registry->registerFunction("REPLACE", functions::builtin::replace);
    registry->registerFunction("SUBSTITUTE", functions::builtin::substitute);
    registry->registerFunction("TEXT", functions::builtin::text);
    registry->registerFunction("VALUE", functions::builtin::value);

    // Date & Time functions
    registry->registerFunction("NOW", functions::builtin::now);
    registry->registerFunction("TODAY", functions::builtin::today);
    registry->registerFunction("DATE", functions::builtin::date);
    registry->registerFunction("TIME", functions::builtin::time_function);
    registry->registerFunction("YEAR", functions::builtin::year);
    registry->registerFunction("MONTH", functions::builtin::month);
    registry->registerFunction("DAY", functions::builtin::day);
    registry->registerFunction("HOUR", functions::builtin::hour);
    registry->registerFunction("MINUTE", functions::builtin::minute);
    registry->registerFunction("SECOND", functions::builtin::second);
    registry->registerFunction("WEEKDAY", functions::builtin::weekday);
    registry->registerFunction("DATEDIF", functions::builtin::datedif);

    // Logical functions
    registry->registerFunction("TRUE", functions::builtin::true_function);
    registry->registerFunction("FALSE", functions::builtin::false_function);
    registry->registerFunction("IF", functions::builtin::if_function);
    registry->registerFunction("AND", functions::builtin::and_function);
    registry->registerFunction("OR", functions::builtin::or_function);
    registry->registerFunction("NOT", functions::builtin::not_function);
    registry->registerFunction("XOR", functions::builtin::xor_function);
    registry->registerFunction("IFERROR", functions::builtin::iferror_function);
    registry->registerFunction("IFNA", functions::builtin::ifna_function);
    registry->registerFunction("ISNUMBER", functions::builtin::isnumber_function);
    registry->registerFunction("ISTEXT", functions::builtin::istext_function);
    registry->registerFunction("ISBLANK", functions::builtin::isblank_function);
    registry->registerFunction("ISERROR", functions::builtin::iserror_function);

    // Engineering functions
    registry->registerFunction("CONVERT", functions::builtin::convert);
    registry->registerFunction("HEX2DEC", functions::builtin::hex2dec);
    registry->registerFunction("DEC2HEX", functions::builtin::dec2hex);
    registry->registerFunction("BIN2DEC", functions::builtin::bin2dec);
    registry->registerFunction("DEC2BIN", functions::builtin::dec2bin);
    registry->registerFunction("BITAND", functions::builtin::bitand_function);
    registry->registerFunction("BITOR", functions::builtin::bitor_function);
    registry->registerFunction("BITXOR", functions::builtin::bitxor_function);

    return registry;
}

}  // namespace xl_formula