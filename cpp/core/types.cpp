#include "xl-formula/types.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace xl_formula {

// Value implementation
double Value::asNumber() const {
    if (type_ != ValueType::NUMBER) {
        throw std::runtime_error("Value is not a number");
    }
    return std::get<double>(data_);
}

std::string Value::asText() const {
    if (type_ != ValueType::TEXT) {
        throw std::runtime_error("Value is not text");
    }
    return std::get<std::string>(data_);
}

bool Value::asBoolean() const {
    if (type_ != ValueType::BOOLEAN) {
        throw std::runtime_error("Value is not a boolean");
    }
    return std::get<bool>(data_);
}

Value::DateType Value::asDate() const {
    if (type_ != ValueType::DATE) {
        throw std::runtime_error("Value is not a date");
    }
    return std::get<DateType>(data_);
}

ErrorType Value::asError() const {
    if (type_ != ValueType::ERROR) {
        throw std::runtime_error("Value is not an error");
    }
    return std::get<ErrorType>(data_);
}

const std::vector<Value>& Value::asArray() const {
    if (type_ != ValueType::ARRAY) {
        throw std::runtime_error("Value is not an array");
    }
    return *std::get<ArrayType>(data_);
}

bool Value::canConvertToNumber() const {
    switch (type_) {
        case ValueType::NUMBER:
            return true;
        case ValueType::BOOLEAN:
            return true;
        case ValueType::TEXT: {
            const auto& text = std::get<std::string>(data_);
            try {
                std::stod(text);
                return true;
            } catch (...) {
                return false;
            }
        }
        default:
            return false;
    }
}

double Value::toNumber() const {
    switch (type_) {
        case ValueType::NUMBER:
            return std::get<double>(data_);
        case ValueType::BOOLEAN:
            return std::get<bool>(data_) ? 1.0 : 0.0;
        case ValueType::TEXT: {
            const auto& text = std::get<std::string>(data_);
            try {
                return std::stod(text);
            } catch (...) {
                throw std::runtime_error("Cannot convert text to number: " + text);
            }
        }
        default:
            throw std::runtime_error("Cannot convert value to number");
    }
}

std::string Value::toString() const {
    switch (type_) {
        case ValueType::NUMBER: {
            std::ostringstream oss;
            double num = std::get<double>(data_);
            if (num == static_cast<long long>(num)) {
                oss << static_cast<long long>(num);
            } else {
                oss << std::fixed << std::setprecision(6) << num;
                // Remove trailing zeros
                std::string str = oss.str();
                str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                str.erase(str.find_last_not_of('.') + 1, std::string::npos);
                return str;
            }
            return oss.str();
        }
        case ValueType::TEXT:
            return std::get<std::string>(data_);
        case ValueType::BOOLEAN:
            return std::get<bool>(data_) ? "TRUE" : "FALSE";
        case ValueType::DATE: {
            // Simple date formatting - could be improved
            auto time_t = std::chrono::system_clock::to_time_t(std::get<DateType>(data_));
            std::ostringstream oss;
            oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d");
            return oss.str();
        }
        case ValueType::ERROR:
            switch (std::get<ErrorType>(data_)) {
                case ErrorType::DIV_ZERO:
                    return "#DIV/0!";
                case ErrorType::VALUE_ERROR:
                    return "#VALUE!";
                case ErrorType::REF_ERROR:
                    return "#REF!";
                case ErrorType::NAME_ERROR:
                    return "#NAME?";
                case ErrorType::NUM_ERROR:
                    return "#NUM!";
                case ErrorType::NA_ERROR:
                    return "#N/A";
                case ErrorType::PARSE_ERROR:
                    return "#PARSE!";
                default:
                    return "#ERROR!";
            }
        case ValueType::ARRAY: {
            std::ostringstream oss;
            oss << "{";
            const auto& arr = *std::get<ArrayType>(data_);
            for (size_t i = 0; i < arr.size(); ++i) {
                if (i > 0) oss << ", ";
                oss << arr[i].toString();
            }
            oss << "}";
            return oss.str();
        }
        case ValueType::EMPTY:
            return "";
        default:
            return "#UNKNOWN!";
    }
}

bool Value::operator==(const Value& other) const {
    if (type_ != other.type_) {
        return false;
    }
    return data_ == other.data_;
}

bool Value::operator<(const Value& other) const {
    if (type_ != other.type_) {
        return static_cast<int>(type_) < static_cast<int>(other.type_);
    }

    switch (type_) {
        case ValueType::NUMBER:
            return std::get<double>(data_) < std::get<double>(other.data_);
        case ValueType::TEXT:
            return std::get<std::string>(data_) < std::get<std::string>(other.data_);
        case ValueType::BOOLEAN:
            return std::get<bool>(data_) < std::get<bool>(other.data_);
        case ValueType::DATE:
            return std::get<DateType>(data_) < std::get<DateType>(other.data_);
        case ValueType::ARRAY:
            // Arrays compare lexicographically
            return *std::get<ArrayType>(data_) < *std::get<ArrayType>(other.data_);
        default:
            return false;
    }
}

bool Value::operator<=(const Value& other) const {
    return *this < other || *this == other;
}

bool Value::operator>(const Value& other) const {
    return !(*this <= other);
}

bool Value::operator>=(const Value& other) const {
    return !(*this < other);
}

}  // namespace xl_formula