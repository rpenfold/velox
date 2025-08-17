#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// Unit conversion data structure
struct UnitInfo {
    std::string category;
    double base_factor;  // Factor to convert to base unit
    double base_offset;  // Offset for temperature conversions
};

// Static unit database
static const std::unordered_map<std::string, UnitInfo> UNITS = {
        // Weight and mass
        {"g", {"mass", 1.0, 0.0}},             // gram (base)
        {"sg", {"mass", 1.0, 0.0}},            // gram (alias)
        {"kg", {"mass", 1000.0, 0.0}},         // kilogram
        {"lbm", {"mass", 453.59237, 0.0}},     // pound mass
        {"u", {"mass", 1.66053886e-24, 0.0}},  // atomic mass unit
        {"ozm", {"mass", 28.349523125, 0.0}},  // ounce mass

        // Distance
        {"m", {"distance", 1.0, 0.0}},               // meter (base)
        {"km", {"distance", 1000.0, 0.0}},           // kilometer
        {"cm", {"distance", 0.01, 0.0}},             // centimeter
        {"mm", {"distance", 0.001, 0.0}},            // millimeter
        {"in", {"distance", 0.0254, 0.0}},           // inch
        {"ft", {"distance", 0.3048, 0.0}},           // foot
        {"yd", {"distance", 0.9144, 0.0}},           // yard
        {"mi", {"distance", 1609.344, 0.0}},         // mile
        {"nmi", {"distance", 1852.0, 0.0}},          // nautical mile
        {"ang", {"distance", 1e-10, 0.0}},           // angstrom
        {"pica", {"distance", 0.00423333333, 0.0}},  // pica

        // Time
        {"sec", {"time", 1.0, 0.0}},        // second (base)
        {"min", {"time", 60.0, 0.0}},       // minute
        {"hr", {"time", 3600.0, 0.0}},      // hour
        {"day", {"time", 86400.0, 0.0}},    // day
        {"yr", {"time", 31557600.0, 0.0}},  // year

        // Pressure
        {"pa", {"pressure", 1.0, 0.0}},        // pascal (base)
        {"atm", {"pressure", 101325.0, 0.0}},  // atmosphere
        {"mmhg", {"pressure", 133.322, 0.0}},  // mmHg

        // Force
        {"n", {"force", 1.0, 0.0}},                // newton (base)
        {"dyn", {"force", 1e-5, 0.0}},             // dyne
        {"lbf", {"force", 4.4482216152605, 0.0}},  // pound force

        // Energy
        {"j", {"energy", 1.0, 0.0}},               // joule (base)
        {"e", {"energy", 1.602176462e-19, 0.0}},   // electron volt
        {"cal", {"energy", 4.184, 0.0}},           // calorie
        {"ev", {"energy", 1.602176462e-19, 0.0}},  // electron volt (alias)
        {"hph", {"energy", 2684519.537696, 0.0}},  // horsepower-hour
        {"wh", {"energy", 3600.0, 0.0}},           // watt-hour
        {"flb", {"energy", 1.355817948, 0.0}},     // foot-pound
        {"btu", {"energy", 1055.05585262, 0.0}},   // British thermal unit

        // Power
        {"w", {"power", 1.0, 0.0}},               // watt (base)
        {"hp", {"power", 745.69987158227, 0.0}},  // horsepower

        // Magnetism
        {"t", {"magnetism", 1.0, 0.0}},    // tesla (base)
        {"ga", {"magnetism", 1e-4, 0.0}},  // gauss

        // Temperature (special handling)
        {"c", {"temperature", 1.0, 273.15}},                      // Celsius
        {"cel", {"temperature", 1.0, 273.15}},                    // Celsius (alias)
        {"f", {"temperature", 5.0 / 9.0, 459.67 * 5.0 / 9.0}},    // Fahrenheit
        {"fah", {"temperature", 5.0 / 9.0, 459.67 * 5.0 / 9.0}},  // Fahrenheit (alias)
        {"k", {"temperature", 1.0, 0.0}},                         // Kelvin (base)
        {"kel", {"temperature", 1.0, 0.0}},                       // Kelvin (alias)

        // Liquid measure
        {"tsp", {"liquid", 4.92892159375e-6, 0.0}},  // teaspoon
        {"tbs", {"liquid", 1.47867647813e-5, 0.0}},  // tablespoon
        {"oz", {"liquid", 2.95735295625e-5, 0.0}},   // ounce
        {"cup", {"liquid", 0.0002365882365, 0.0}},   // cup
        {"pt", {"liquid", 0.000473176473, 0.0}},     // pint
        {"qt", {"liquid", 0.000946352946, 0.0}},     // quart
        {"gal", {"liquid", 0.003785411784, 0.0}},    // gallon
        {"l", {"liquid", 0.001, 0.0}},               // liter
        {"lt", {"liquid", 0.001, 0.0}},              // liter (alias)
};

/**
 * @brief Converts a number from one measurement system to another
 * @ingroup engineering
 * @param number Value to convert
 * @param from_unit Unit to convert from
 * @param to_unit Unit to convert to
 * @code
 * CONVERT(32,"F","C") -> 0
 * @endcode
 */
Value convert(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 3, "CONVERT");
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Get number to convert
    auto number = utils::toNumberSafe(args[0], "CONVERT");
    if (number.isError()) {
        return number;
    }
    double value = number.asNumber();

    // Get from and to units
    std::string from_unit = args[1].toString();
    std::string to_unit = args[2].toString();

    // Convert to lowercase for case-insensitive matching
    std::transform(from_unit.begin(), from_unit.end(), from_unit.begin(), ::tolower);
    std::transform(to_unit.begin(), to_unit.end(), to_unit.begin(), ::tolower);

    // Look up units
    auto from_it = UNITS.find(from_unit);
    auto to_it = UNITS.find(to_unit);

    if (from_it == UNITS.end()) {
        return Value::error(ErrorType::VALUE_ERROR);  // Unknown from unit
    }

    if (to_it == UNITS.end()) {
        return Value::error(ErrorType::VALUE_ERROR);  // Unknown to unit
    }

    const UnitInfo& from_info = from_it->second;
    const UnitInfo& to_info = to_it->second;

    // Check if units are in the same category
    if (from_info.category != to_info.category) {
        return Value::error(ErrorType::VALUE_ERROR);  // Incompatible units
    }

    double result;

    if (from_info.category == "temperature") {
        // Special handling for temperature conversions
        // Convert from source to Kelvin, then to target
        double kelvin;
        std::string from_unit = args[1].toString();
        std::transform(from_unit.begin(), from_unit.end(), from_unit.begin(), ::tolower);

        // Convert from source to Kelvin
        if (from_unit == "c" || from_unit == "cel") {
            kelvin = value + 273.15;
        } else if (from_unit == "f" || from_unit == "fah") {
            kelvin = (value + 459.67) * 5.0 / 9.0;
        } else {  // Kelvin
            kelvin = value;
        }

        // Convert from Kelvin to target
        std::string to_unit = args[2].toString();
        std::transform(to_unit.begin(), to_unit.end(), to_unit.begin(), ::tolower);

        if (to_unit == "c" || to_unit == "cel") {
            result = kelvin - 273.15;
        } else if (to_unit == "f" || to_unit == "fah") {
            result = (kelvin * 9.0 / 5.0) - 459.67;
        } else {  // Kelvin
            result = kelvin;
        }
    } else {
        // Standard linear conversion
        // Convert from source to base unit, then to target unit
        double base_value = value * from_info.base_factor;
        result = base_value / to_info.base_factor;
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula