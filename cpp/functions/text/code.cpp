 #include "xl-formula/functions.h"
 #include <string>

 namespace xl_formula {
 namespace functions {
 namespace builtin {

/**
 * @brief Returns a numeric code for the first character in a text string
 * @ingroup text
 * @param text The text to examine
 * @code
 * CODE("A") -> 65
 * @endcode
 */
 Value code_function(const std::vector<Value>& args, const Context& context) {
     (void)context;

     auto countValidation = utils::validateArgCount(args, 1, "CODE");
     if (!countValidation.isEmpty()) {
         return countValidation;
     }

     auto errorCheck = utils::checkForErrors(args);
     if (!errorCheck.isEmpty()) {
         return errorCheck;
     }

     std::string text = args[0].toString();
     if (text.empty()) {
         return Value::error(ErrorType::VALUE_ERROR);
     }

     unsigned char c = static_cast<unsigned char>(text[0]);
     // Return as number
     return Value(static_cast<double>(c));
 }

 }  // namespace builtin
 }  // namespace functions
 }  // namespace xl_formula

