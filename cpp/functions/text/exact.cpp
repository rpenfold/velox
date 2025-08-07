 #include "xl-formula/functions.h"
 #include <string>

 namespace xl_formula {
 namespace functions {
 namespace builtin {

 // EXACT(text1, text2) -> case-sensitive equality returning TRUE/FALSE
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

