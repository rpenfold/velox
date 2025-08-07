 #include "xl-formula/functions.h"
 #include <string>

 namespace xl_formula {
 namespace functions {
 namespace builtin {

 // CLEAN(text) -> removes non-printable ASCII (codes < 32)
 Value clean(const std::vector<Value>& args, const Context& context) {
     (void)context;

     auto countValidation = utils::validateArgCount(args, 1, "CLEAN");
     if (!countValidation.isEmpty()) {
         return countValidation;
     }

     auto errorCheck = utils::checkForErrors(args);
     if (!errorCheck.isEmpty()) {
         return errorCheck;
     }

     std::string text = args[0].toString();
     std::string result;
     result.reserve(text.size());
     for (unsigned char ch : text) {
         if (ch >= 32) {
             result.push_back(static_cast<char>(ch));
         }
     }
     return Value(result);
 }

 }  // namespace builtin
 }  // namespace functions
 }  // namespace xl_formula

