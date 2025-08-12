 #include "xl-formula/functions.h"
 #include <string>

 namespace xl_formula {
 namespace functions {
 namespace builtin {

/**
 * @brief Checks whether two text strings are exactly the same (case-sensitive)
 * @ingroup text
 * @param text1 First text string
 * @param text2 Second text string
 * @code
 * EXACT("Hello", "hello") -> FALSE
 * @endcode
 */
 Value exact(const std::vector<Value>& args, const Context& context) {
     (void)context;

     auto countValidation = utils::validateArgCount(args, 2, "EXACT");
     if (!countValidation.isEmpty()) {
         return countValidation;
     }

     auto errorCheck = utils::checkForErrors(args);
     if (!errorCheck.isEmpty()) {
         return errorCheck;
     }

     std::string a = args[0].toString();
     std::string b = args[1].toString();
     return Value(a == b);
 }

 }  // namespace builtin
 }  // namespace functions
 }  // namespace xl_formula

