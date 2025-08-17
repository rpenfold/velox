#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace dispatcher {

Value dispatch_builtin_function(const std::string& name, const std::vector<Value>& args,
                                const Context& context) {
    // Use compile-time hash for ultra-fast dispatch
    const uint32_t hash = hash_function_name(name.c_str());

    switch (hash) {
        // Math functions
        case hash_function_name("SUM"):
            return builtin::sum(args, context);
        case hash_function_name("MAX"):
            return builtin::max(args, context);
        case hash_function_name("MIN"):
            return builtin::min(args, context);
        case hash_function_name("AVERAGE"):
            return builtin::average(args, context);
        case hash_function_name("COUNT"):
            return builtin::count(args, context);
        case hash_function_name("COUNTA"):
            return builtin::counta(args, context);
        case hash_function_name("ABS"):
            return builtin::abs_function(args, context);
        case hash_function_name("ROUND"):
            return builtin::round_function(args, context);
        case hash_function_name("ROUNDUP"):
            return builtin::roundup(args, context);
        case hash_function_name("ROUNDDOWN"):
            return builtin::rounddown(args, context);
        case hash_function_name("MROUND"):
            return builtin::mround(args, context);
        case hash_function_name("SUMSQ"):
            return builtin::sumsq(args, context);
        case hash_function_name("QUOTIENT"):
            return builtin::quotient(args, context);
        case hash_function_name("EVEN"):
            return builtin::even_function(args, context);
        case hash_function_name("ODD"):
            return builtin::odd_function(args, context);
        case hash_function_name("SQRT"):
            return builtin::sqrt_function(args, context);
        case hash_function_name("POWER"):
            return builtin::power(args, context);
        case hash_function_name("MOD"):
            return builtin::mod(args, context);
        case hash_function_name("PI"):
            return builtin::pi(args, context);
        case hash_function_name("SIGN"):
            return builtin::sign(args, context);
        case hash_function_name("INT"):
            return builtin::int_function(args, context);
        case hash_function_name("TRUNC"):
            return builtin::trunc_function(args, context);
        case hash_function_name("CEILING"):
            return builtin::ceiling_function(args, context);
        case hash_function_name("FLOOR"):
            return builtin::floor_function(args, context);
        case hash_function_name("RAND"):
            return builtin::rand_function(args, context);
        case hash_function_name("RANDBETWEEN"):
            return builtin::randbetween(args, context);
        case hash_function_name("COUNTIF"):
            return builtin::countif(args, context);
        case hash_function_name("MEDIAN"):
            return builtin::median(args, context);
        case hash_function_name("MODE"):
            return builtin::mode(args, context);
        case hash_function_name("STDEV"):
            return builtin::stdev(args, context);
        case hash_function_name("VAR"):
            return builtin::var(args, context);
        case hash_function_name("CORREL"):
            return builtin::correl(args, context);
        case hash_function_name("PEARSON"):
            return builtin::correl(args, context);
        case hash_function_name("RSQ"):
            return builtin::rsq(args, context);
        case hash_function_name("SLOPE"):
            return builtin::slope(args, context);
        case hash_function_name("INTERCEPT"):
            return builtin::intercept(args, context);
        case hash_function_name("COVARIANCE.P"):
            return builtin::covariance_p(args, context);
        case hash_function_name("COVARIANCE.S"):
            return builtin::covariance_s(args, context);
        case hash_function_name("COVAR"):
            return builtin::covar(args, context);

        // Trigonometric functions
        case hash_function_name("SIN"):
            return builtin::sin_function(args, context);
        case hash_function_name("COS"):
            return builtin::cos_function(args, context);
        case hash_function_name("TAN"):
            return builtin::tan_function(args, context);
        case hash_function_name("ASIN"):
            return builtin::asin_function(args, context);
        case hash_function_name("ACOS"):
            return builtin::acos_function(args, context);
        case hash_function_name("ATAN"):
            return builtin::atan_function(args, context);
        case hash_function_name("ATAN2"):
            return builtin::atan2_function(args, context);
        case hash_function_name("SINH"):
            return builtin::sinh_function(args, context);
        case hash_function_name("COSH"):
            return builtin::cosh_function(args, context);
        case hash_function_name("TANH"):
            return builtin::tanh_function(args, context);
        case hash_function_name("DEGREES"):
            return builtin::degrees_function(args, context);
        case hash_function_name("RADIANS"):
            return builtin::radians_function(args, context);
        case hash_function_name("EXP"):
            return builtin::exp_function(args, context);
        case hash_function_name("LN"):
            return builtin::ln_function(args, context);
        case hash_function_name("LOG"):
            return builtin::log_function(args, context);
        case hash_function_name("LOG10"):
            return builtin::log10_function(args, context);

        // Text functions
        case hash_function_name("CONCATENATE"):
            return builtin::concatenate(args, context);
        case hash_function_name("CONCAT"):
            return builtin::concatenate(args, context);
        case hash_function_name("TRIM"):
            return builtin::trim(args, context);
        case hash_function_name("LEN"):
            return builtin::len(args, context);
        case hash_function_name("LEFT"):
            return builtin::left(args, context);
        case hash_function_name("RIGHT"):
            return builtin::right(args, context);
        case hash_function_name("MID"):
            return builtin::mid(args, context);
        case hash_function_name("UPPER"):
            return builtin::upper(args, context);
        case hash_function_name("LOWER"):
            return builtin::lower(args, context);
        case hash_function_name("PROPER"):
            return builtin::proper(args, context);
        case hash_function_name("RPT"):
            return builtin::rpt(args, context);
        case hash_function_name("REPT"):
            return builtin::rpt(args, context);
        case hash_function_name("FIND"):
            return builtin::find(args, context);
        case hash_function_name("SEARCH"):
            return builtin::search(args, context);
        case hash_function_name("REPLACE"):
            return builtin::replace(args, context);
        case hash_function_name("SUBSTITUTE"):
            return builtin::substitute(args, context);
        case hash_function_name("TEXT"):
            return builtin::text(args, context);
        case hash_function_name("VALUE"):
            return builtin::value(args, context);
        case hash_function_name("T"):
            return builtin::t_function(args, context);
        case hash_function_name("TEXTJOIN"):
            return builtin::textjoin(args, context);
        case hash_function_name("UNICHAR"):
            return builtin::unichar(args, context);
        case hash_function_name("UNICODE"):
            return builtin::unicode_function(args, context);

        // Date & Time functions
        case hash_function_name("NOW"):
            return builtin::now(args, context);
        case hash_function_name("TODAY"):
            return builtin::today(args, context);
        case hash_function_name("DATE"):
            return builtin::date(args, context);
        case hash_function_name("TIME"):
            return builtin::time_function(args, context);
        case hash_function_name("YEAR"):
            return builtin::year(args, context);
        case hash_function_name("MONTH"):
            return builtin::month(args, context);
        case hash_function_name("DAY"):
            return builtin::day(args, context);
        case hash_function_name("HOUR"):
            return builtin::hour(args, context);
        case hash_function_name("MINUTE"):
            return builtin::minute(args, context);
        case hash_function_name("SECOND"):
            return builtin::second(args, context);
        case hash_function_name("WEEKDAY"):
            return builtin::weekday(args, context);
        case hash_function_name("DATEDIF"):
            return builtin::datedif(args, context);
        case hash_function_name("EDATE"):
            return builtin::edate(args, context);
        case hash_function_name("EOMONTH"):
            return builtin::eomonth(args, context);
        case hash_function_name("DATEVALUE"):
            return builtin::datevalue(args, context);
        case hash_function_name("TIMEVALUE"):
            return builtin::timevalue(args, context);

        // Logical functions
        case hash_function_name("TRUE"):
            return builtin::true_function(args, context);
        case hash_function_name("FALSE"):
            return builtin::false_function(args, context);
        case hash_function_name("IF"):
            return builtin::if_function(args, context);
        case hash_function_name("AND"):
            return builtin::and_function(args, context);
        case hash_function_name("OR"):
            return builtin::or_function(args, context);
        case hash_function_name("NOT"):
            return builtin::not_function(args, context);
        case hash_function_name("XOR"):
            return builtin::xor_function(args, context);
        case hash_function_name("IFERROR"):
            return builtin::iferror_function(args, context);
        case hash_function_name("IFNA"):
            return builtin::ifna_function(args, context);
        case hash_function_name("ISNUMBER"):
            return builtin::isnumber_function(args, context);
        case hash_function_name("ISTEXT"):
            return builtin::istext_function(args, context);
        case hash_function_name("ISBLANK"):
            return builtin::isblank_function(args, context);
        case hash_function_name("ISERROR"):
            return builtin::iserror_function(args, context);
        case hash_function_name("SWITCH"):
            return builtin::switch_function(args, context);
        case hash_function_name("IFS"):
            return builtin::ifs_function(args, context);

        // Engineering functions
        case hash_function_name("CONVERT"):
            return builtin::convert(args, context);
        case hash_function_name("HEX2DEC"):
            return builtin::hex2dec(args, context);
        case hash_function_name("DEC2HEX"):
            return builtin::dec2hex(args, context);
        case hash_function_name("BIN2DEC"):
            return builtin::bin2dec(args, context);
        case hash_function_name("DEC2BIN"):
            return builtin::dec2bin(args, context);
        case hash_function_name("BITAND"):
            return builtin::bitand_function(args, context);
        case hash_function_name("BITOR"):
            return builtin::bitor_function(args, context);
        case hash_function_name("BITXOR"):
            return builtin::bitxor_function(args, context);
        case hash_function_name("DEC2OCT"):
            return builtin::dec2oct(args, context);
        case hash_function_name("BIN2OCT"):
            return builtin::bin2oct(args, context);
        case hash_function_name("OCT2BIN"):
            return builtin::oct2bin(args, context);
        case hash_function_name("HEX2OCT"):
            return builtin::hex2oct(args, context);
        case hash_function_name("OCT2HEX"):
            return builtin::oct2hex(args, context);
        case hash_function_name("COMPLEX"):
            return builtin::complex_function(args, context);
        case hash_function_name("IMREAL"):
            return builtin::imreal(args, context);
        case hash_function_name("IMAGINARY"):
            return builtin::imaginary(args, context);

        // Financial functions
        case hash_function_name("PV"):
            return builtin::pv(args, context);
        case hash_function_name("FV"):
            return builtin::fv(args, context);
        case hash_function_name("PMT"):
            return builtin::pmt(args, context);
        case hash_function_name("RATE"):
            return builtin::rate(args, context);
        case hash_function_name("NPER"):
            return builtin::nper(args, context);
        case hash_function_name("NPV"):
            return builtin::npv(args, context);
        case hash_function_name("IRR"):
            return builtin::irr(args, context);
        case hash_function_name("MIRR"):
            return builtin::mirr(args, context);

        // Phase 11: Additional Math Functions
        case hash_function_name("GCD"):
            return builtin::gcd(args, context);
        case hash_function_name("LCM"):
            return builtin::lcm(args, context);
        case hash_function_name("FACT"):
            return builtin::fact(args, context);
        case hash_function_name("COMBIN"):
            return builtin::combin(args, context);
        case hash_function_name("PERMUT"):
            return builtin::permut(args, context);
        case hash_function_name("SUMPRODUCT"):
            return builtin::sumproduct(args, context);
        case hash_function_name("SUMIF"):
            return builtin::sumif(args, context);
        case hash_function_name("SUMIFS"):
            return builtin::sumifs(args, context);
        case hash_function_name("SUMX2MY2"):
            return builtin::sumx2my2(args, context);
        case hash_function_name("SUMX2PY2"):
            return builtin::sumx2py2(args, context);
        case hash_function_name("SUMXMY2"):
            return builtin::sumxmy2(args, context);
        case hash_function_name("AVERAGEIF"):
            return builtin::averageif(args, context);
        case hash_function_name("AVERAGEIFS"):
            return builtin::averageifs(args, context);

        // Lookup & Reference
        case hash_function_name("CHOOSE"):
            return builtin::choose(args, context);
        case hash_function_name("ROW"):
            return builtin::row_function(args, context);
        case hash_function_name("COLUMN"):
            return builtin::column_function(args, context);

        // Text additions
        case hash_function_name("CHAR"):
            return builtin::char_function(args, context);
        case hash_function_name("CODE"):
            return builtin::code_function(args, context);
        case hash_function_name("CLEAN"):
            return builtin::clean(args, context);
        case hash_function_name("EXACT"):
            return builtin::exact(args, context);
        case hash_function_name("ROMAN"):
            return builtin::roman(args, context);
        case hash_function_name("ARABIC"):
            return builtin::arabic(args, context);

        // Non-standard namespace (NS_*)
        case hash_function_name("NS_UNIXTIME"):
            return builtin::ns_unixtime(args, context);
        case hash_function_name("NS_NEARESTDATE"):
            return builtin::ns_nearestdate(args, context);
        case hash_function_name("NS_FURTHESTDATE"):
            return builtin::ns_furthestdate(args, context);

        default:
            // Not a built-in function - return empty Value to indicate fallback needed
            return Value();
    }
}

std::vector<std::string> get_builtin_function_names() {
    return {// Math functions
            "SUM", "MAX", "MIN", "AVERAGE", "COUNT", "COUNTA", "ABS", "ROUND", "ROUNDUP",
            "ROUNDDOWN", "MROUND", "SQRT", "POWER", "MOD", "PI", "SIGN", "INT", "TRUNC", "CEILING",
            "FLOOR", "RAND", "RANDBETWEEN", "COUNTIF", "MEDIAN", "MODE", "STDEV", "VAR", "GCD",
            "LCM", "FACT", "COMBIN", "PERMUT", "SUMPRODUCT", "SUMIF", "SUMIFS", "AVERAGEIF",
            "AVERAGEIFS", "SUMSQ", "QUOTIENT", "EVEN", "ODD",

            // Trigonometric functions
            "SIN", "COS", "TAN", "ASIN", "ACOS", "ATAN", "ATAN2", "SINH", "COSH", "TANH", "DEGREES",
            "RADIANS", "EXP", "LN", "LOG", "LOG10",

            // Statistics & Regression
            "CORREL", "PEARSON", "RSQ", "SLOPE", "INTERCEPT", "COVARIANCE.P", "COVARIANCE.S",
            "COVAR",

            // Sum of squares variants
            "SUMX2MY2", "SUMX2PY2", "SUMXMY2",

            // Text functions
            "CONCATENATE", "CONCAT", "TRIM", "LEN", "LEFT", "RIGHT", "MID", "UPPER", "LOWER",
            "PROPER", "RPT", "CHAR", "CODE", "CLEAN", "EXACT", "ROMAN", "ARABIC", "FIND", "SEARCH",
            "REPLACE", "SUBSTITUTE", "TEXT", "VALUE", "T", "TEXTJOIN", "UNICHAR", "UNICODE",

            // Date & Time functions
            "NOW", "TODAY", "DATE", "TIME", "YEAR", "MONTH", "DAY", "HOUR", "MINUTE", "SECOND",
            "WEEKDAY", "DATEDIF", "EDATE", "EOMONTH", "DATEVALUE", "TIMEVALUE",

            // Lookup & Reference
            "CHOOSE", "ROW", "COLUMN",

            // Logical functions
            "TRUE", "FALSE", "IF", "AND", "OR", "NOT", "XOR", "IFERROR", "IFNA", "ISNUMBER",
            "ISTEXT", "ISBLANK", "ISERROR", "SWITCH", "IFS",

            // Engineering functions
            "CONVERT", "HEX2DEC", "DEC2HEX", "BIN2DEC", "DEC2BIN", "BITAND", "BITOR", "BITXOR",
            "DEC2OCT", "BIN2OCT", "OCT2BIN", "HEX2OCT", "OCT2HEX", "COMPLEX", "IMREAL", "IMAGINARY",

            // Financial functions
            "PV", "FV", "PMT", "RATE", "NPER", "NPV", "IRR", "MIRR"};
}

}  // namespace dispatcher
}  // namespace functions
}  // namespace xl_formula