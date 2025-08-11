#include <string>
#include <sstream>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// Parse a complex number in form a+bi or a-bi. Returns pair(real, imag)
static std::pair<double, double> parseComplex(const std::string& s) {
    // Remove spaces
    std::string str;
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c))) str.push_back(c);
    }
    if (str.empty()) return {0.0, 0.0};

    // Find the last '+' or '-' that splits real and imaginary, ignoring leading sign
    size_t pos = std::string::npos;
    for (size_t i = 1; i < str.size(); ++i) {
        if (str[i] == '+' || str[i] == '-') {
            pos = i; // keep last occurrence
        }
    }
    if (pos == std::string::npos) {
        // Could be pure imaginary like "3i" or pure real
        if (str.back() == 'i' || str.back() == 'I') {
            std::string imag_part = str.substr(0, str.size() - 1);
            double imag = imag_part.empty() || imag_part == "+" ? 1.0 : (imag_part == "-" ? -1.0 : std::stod(imag_part));
            return {0.0, imag};
        }
        return {std::stod(str), 0.0};
    }

    std::string real_part = str.substr(0, pos);
    std::string imag_part = str.substr(pos, str.size() - pos);
    // imag_part must end with i
    if (imag_part.empty() || (imag_part.back() != 'i' && imag_part.back() != 'I')) {
        // If no i, treat entire as real
        return {std::stod(str), 0.0};
    }
    imag_part.pop_back();
    if (imag_part == "+" || imag_part == "-") {
        imag_part += "1";
    }
    double real = real_part.empty() ? 0.0 : std::stod(real_part);
    double imag = imag_part.empty() ? 0.0 : std::stod(imag_part);
    return {real, imag};
}

Value complex_function(const std::vector<Value>& args, const Context& context) {
    (void)context;
    // COMPLEX(real_num, i_num, [suffix])
    auto err = utils::validateMinArgs(args, 2, "COMPLEX");
    if (!err.isEmpty()) return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;

    auto real_v = utils::toNumberSafe(args[0], "COMPLEX");
    if (real_v.isError()) return real_v;
    auto imag_v = utils::toNumberSafe(args[1], "COMPLEX");
    if (imag_v.isError()) return imag_v;
    std::string suffix = "i";
    if (args.size() >= 3) {
        suffix = args[2].toString();
        if (!(suffix == "i" || suffix == "j" || suffix == "I" || suffix == "J")) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        // Normalize to lower case i/j
        suffix = std::string(1, static_cast<char>(std::tolower(suffix[0])));
    }

    double real = real_v.asNumber();
    double imag = imag_v.asNumber();

    std::ostringstream oss;
    oss << real;
    if (imag >= 0) oss << "+";
    oss << imag << suffix;
    return Value(oss.str());
}

Value imreal(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 1, "IMREAL");
    if (!err.isEmpty()) return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;
    auto p = parseComplex(args[0].toString());
    return Value(p.first);
}

Value imaginary(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 1, "IMAGINARY");
    if (!err.isEmpty()) return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;
    auto p = parseComplex(args[0].toString());
    return Value(p.second);
}

Value imabs(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 1, "IMABS");
    if (!err.isEmpty()) return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;
    auto p = parseComplex(args[0].toString());
    double mag = std::sqrt(p.first * p.first + p.second * p.second);
    return Value(mag);
}

Value imargument(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 1, "IMARGUMENT");
    if (!err.isEmpty()) return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;
    auto p = parseComplex(args[0].toString());
    double ang = std::atan2(p.second, p.first);
    return Value(ang);
}

static std::pair<double,double> addC(const std::pair<double,double>& a, const std::pair<double,double>& b){return {a.first+b.first, a.second+b.second};}
static std::pair<double,double> subC(const std::pair<double,double>& a, const std::pair<double,double>& b){return {a.first-b.first, a.second-b.second};}
static std::pair<double,double> mulC(const std::pair<double,double>& a, const std::pair<double,double>& b){return {a.first*b.first - a.second*b.second, a.first*b.second + a.second*b.first};}
static std::pair<double,double> divC(const std::pair<double,double>& a, const std::pair<double,double>& b){double d=b.first*b.first+b.second*b.second;return { (a.first*b.first + a.second*b.second)/d, (a.second*b.first - a.first*b.second)/d};}

Value imsum(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.empty()) return Value::error(ErrorType::VALUE_ERROR);
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;
    std::pair<double,double> acc{0.0,0.0};
    for (const auto& v : args) {
        auto p = parseComplex(v.toString());
        acc = addC(acc, p);
    }
    std::ostringstream oss; oss<<acc.first; if (acc.second>=0) oss<<"+"; oss<<acc.second<<"i"; return Value(oss.str());
}

Value imsub(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 2, "IMSUB");
    if (!err.isEmpty()) return err;
    auto a = parseComplex(args[0].toString());
    auto b = parseComplex(args[1].toString());
    auto r = subC(a,b);
    std::ostringstream oss; oss<<r.first; if (r.second>=0) oss<<"+"; oss<<r.second<<"i"; return Value(oss.str());
}

Value improduct(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.empty()) return Value::error(ErrorType::VALUE_ERROR);
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;
    std::pair<double,double> acc{1.0,0.0};
    for (const auto& v : args) {
        acc = mulC(acc, parseComplex(v.toString()));
    }
    std::ostringstream oss; oss<<acc.first; if (acc.second>=0) oss<<"+"; oss<<acc.second<<"i"; return Value(oss.str());
}

Value imdiv(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 2, "IMDIV");
    if (!err.isEmpty()) return err;
    auto a = parseComplex(args[0].toString());
    auto b = parseComplex(args[1].toString());
    auto r = divC(a,b);
    std::ostringstream oss; oss<<r.first; if (r.second>=0) oss<<"+"; oss<<r.second<<"i"; return Value(oss.str());
}

Value impower(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 2, "IMPOWER");
    if (!err.isEmpty()) return err;
    auto a = parseComplex(args[0].toString());
    auto expV = utils::toNumberSafe(args[1], "IMPOWER");
    if (expV.isError()) return expV;
    int n = static_cast<int>(expV.asNumber());
    std::pair<double,double> acc{1.0,0.0};
    std::pair<double,double> base=a;
    int p = std::abs(n);
    while (p>0){ if (p&1) acc = mulC(acc, base); base = mulC(base, base); p >>= 1; }
    if (n<0){ acc = divC({1.0,0.0}, acc); }
    std::ostringstream oss; oss<<acc.first; if (acc.second>=0) oss<<"+"; oss<<acc.second<<"i"; return Value(oss.str());
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


