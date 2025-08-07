 #include "xl-formula/functions.h"
 #include <string>

 namespace xl_formula {
 namespace functions {
 namespace builtin {

 // CHAR(number) -> returns character for code 1-255
 Value char_function(const std::vector<Value>& args, const Context& context) {
     (void)context;

     auto countValidation = utils::validateArgCount(args, 1, "CHAR");
     if (!countValidation.isEmpty()) {
         return countValidation;
     }

     auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    auto numResult = utils::toNumberSafe(args[0], "CHAR");
    if (numResult.isError()) {
        return numResult;
    }

    double code = numResult.asNumber();
     // Truncate to integer
     int intCode = static_cast<int>(code);
     if (intCode <= 0 || intCode > 255) {
         return Value::error(ErrorType::VALUE_ERROR);
     }

     char c = static_cast<char>(static_cast<unsigned char>(intCode));
     std::string result(1, c);
     return Value(result);
 }

 }  // namespace builtin
 }  // namespace functions
 }  // namespace xl_formula

