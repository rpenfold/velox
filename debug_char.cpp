#include <iostream>
#include "xl-formula/xl-formula.h"

int main() {
    xl_formula::Context ctx;
    xl_formula::Value result = xl_formula::functions::builtin::char_function({xl_formula::Value("65")}, ctx);
    std::cout << "isText: " << result.isText() << std::endl;
    std::cout << "isError: " << result.isError() << std::endl;
    std::cout << "isNumber: " << result.isNumber() << std::endl;
    if (result.isError()) {
        std::cout << "Is error" << std::endl;
    }
    if (result.isText()) {
        std::cout << "Text: " << result.asText() << std::endl;
    }
    return 0;
}
