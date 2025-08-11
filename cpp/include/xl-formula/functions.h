#pragma once

#include <cmath>
#include <stdexcept>
#include <vector>
#include "conditional_utils.h"
#include "evaluator.h"
#include "types.h"

namespace xl_formula {
namespace functions {

/**
 * @brief Built-in function implementations
 */
namespace builtin {

/**
 * @brief SUM function - adds all numeric arguments
 * @param args Function arguments
 * @param context Evaluation context (unused for SUM)
 * @return Sum of all numeric arguments
 */
Value sum(const std::vector<Value>& args, const Context& context);

/**
 * @brief MAX function - returns maximum value from arguments
 * Supports numbers, dates, and comparable types
 * @param args Function arguments
 * @param context Evaluation context (unused for MAX)
 * @return Maximum value from arguments
 */
Value max(const std::vector<Value>& args, const Context& context);

/**
 * @brief CONCATENATE function - concatenates all arguments as text
 * @param args Function arguments
 * @param context Evaluation context (unused for CONCATENATE)
 * @return Concatenated string
 */
Value concatenate(const std::vector<Value>& args, const Context& context);

/**
 * @brief TRIM function - removes leading and trailing whitespace
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for TRIM)
 * @return Trimmed string
 */
Value trim(const std::vector<Value>& args, const Context& context);

/**
 * @brief TRUE function - returns boolean true
 * @param args Function arguments (ignored)
 * @param context Evaluation context (unused for TRUE)
 * @return Boolean true value
 */
Value true_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief FALSE function - returns boolean false
 * @param args Function arguments (ignored)
 * @param context Evaluation context (unused for FALSE)
 * @return Boolean false value
 */
Value false_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief IF function - conditional logic
 * @param args Function arguments (condition, true_value, false_value)
 * @param context Evaluation context (unused for IF)
 * @return true_value if condition is true, false_value otherwise
 */
Value if_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief AND function - returns TRUE if all arguments are TRUE
 * @param args Function arguments (at least one required)
 * @param context Evaluation context (unused for AND)
 * @return TRUE if all arguments are TRUE, FALSE otherwise
 */
Value and_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief OR function - returns TRUE if any argument is TRUE
 * @param args Function arguments (at least one required)
 * @param context Evaluation context (unused for OR)
 * @return TRUE if any argument is TRUE, FALSE otherwise
 */
Value or_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief NOT function - reverses the logic of its argument
 * @param args Function arguments (expects 1 argument)
 * @param context Evaluation context (unused for NOT)
 * @return TRUE if argument is FALSE, FALSE if argument is TRUE
 */
Value not_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief XOR function - returns TRUE if an odd number of arguments are TRUE
 * @param args Function arguments (at least one required)
 * @param context Evaluation context (unused for XOR)
 * @return TRUE if odd number of arguments are TRUE, FALSE otherwise
 */
Value xor_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief IFERROR function - returns a value if expression is an error
 * @param args Function arguments (value, value_if_error)
 * @param context Evaluation context (unused for IFERROR)
 * @return value_if_error if value is an error, otherwise value
 */
Value iferror_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief IFNA function - returns a value if expression is #N/A
 * @param args Function arguments (value, value_if_na)
 * @param context Evaluation context (unused for IFNA)
 * @return value_if_na if value is #N/A, otherwise value
 */
Value ifna_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ISNUMBER function - tests if a value is a number
 * @param args Function arguments (expects 1 argument)
 * @param context Evaluation context (unused for ISNUMBER)
 * @return TRUE if value is a number or can be converted to number
 */
Value isnumber_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ISTEXT function - tests if a value is text
 * @param args Function arguments (expects 1 argument)
 * @param context Evaluation context (unused for ISTEXT)
 * @return TRUE if value is text
 */
Value istext_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ISBLANK function - tests if a value is blank
 * @param args Function arguments (expects 1 argument)
 * @param context Evaluation context (unused for ISBLANK)
 * @return TRUE if value is empty/blank
 */
Value isblank_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ISERROR function - tests if a value is an error
 * @param args Function arguments (expects 1 argument)
 * @param context Evaluation context (unused for ISERROR)
 * @return TRUE if value is an error
 */
Value iserror_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief SWITCH function - compares an expression against multiple values and returns corresponding result
 * @param args Function arguments (expression, value1, result1, [value2, result2, ...], [default])
 * @param context Evaluation context (unused for SWITCH)
 * @return The result corresponding to the first matching value, or default if no match
 */
Value switch_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief IFS function - checks multiple conditions and returns corresponding result for first TRUE condition
 * @param args Function arguments (condition1, result1, [condition2, result2, ...])
 * @param context Evaluation context (unused for IFS)
 * @return The result corresponding to the first TRUE condition, or #N/A if none are TRUE
 */
Value ifs_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief LEN function - returns length of text
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for LEN)
 * @return Length of text as number
 */
Value len(const std::vector<Value>& args, const Context& context);

/**
 * @brief LEFT function - returns the leftmost characters from text
 * @param args Function arguments (text, [num_chars])
 * @param context Evaluation context (unused for LEFT)
 * @return Leftmost characters from text
 */
Value left(const std::vector<Value>& args, const Context& context);

/**
 * @brief RIGHT function - returns the rightmost characters from text
 * @param args Function arguments (text, [num_chars])
 * @param context Evaluation context (unused for RIGHT)
 * @return Rightmost characters from text
 */
Value right(const std::vector<Value>& args, const Context& context);

/**
 * @brief MID function - returns characters from the middle of text
 * @param args Function arguments (text, start_num, num_chars)
 * @param context Evaluation context (unused for MID)
 * @return Characters from the middle of text
 */
Value mid(const std::vector<Value>& args, const Context& context);

/**
 * @brief UPPER function - converts text to uppercase
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for UPPER)
 * @return Text converted to uppercase
 */
Value upper(const std::vector<Value>& args, const Context& context);

/**
 * @brief LOWER function - converts text to lowercase
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for LOWER)
 * @return Text converted to lowercase
 */
Value lower(const std::vector<Value>& args, const Context& context);

/**
 * @brief PROPER function - capitalizes the first letter of each word
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for PROPER)
 * @return Text with first letter of each word capitalized
 */
Value proper(const std::vector<Value>& args, const Context& context);
    Value char_function(const std::vector<Value>& args, const Context& context);
    Value code_function(const std::vector<Value>& args, const Context& context);
    Value clean(const std::vector<Value>& args, const Context& context);
    Value exact(const std::vector<Value>& args, const Context& context);

/**
 * @brief RPT function - repeats text a specified number of times
 * @param args Function arguments (text, repeat_count)
 * @param context Evaluation context (unused for RPT)
 * @return Text repeated the specified number of times
 */
Value rpt(const std::vector<Value>& args, const Context& context);

/**
 * @brief T function - converts value to text, returns empty string for non-text values
 * @param args Function arguments (value)
 * @param context Evaluation context (unused for T)
 * @return Text representation if input is text, empty string otherwise
 */
Value t_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief TEXTJOIN function - joins text values with a delimiter
 * @param args Function arguments (delimiter, ignore_empty, text1, text2, ...)
 * @param context Evaluation context (unused for TEXTJOIN)
 * @return Joined text string
 */
Value textjoin(const std::vector<Value>& args, const Context& context);

/**
 * @brief UNICHAR function - returns Unicode character for given code point
 * @param args Function arguments (code_point)
 * @param context Evaluation context (unused for UNICHAR)
 * @return Unicode character corresponding to the code point
 */
Value unichar(const std::vector<Value>& args, const Context& context);

/**
 * @brief UNICODE function - returns Unicode code point for first character
 * @param args Function arguments (text)
 * @param context Evaluation context (unused for UNICODE)
 * @return Unicode code point of the first character
 */
Value unicode_function(const std::vector<Value>& args, const Context& context);

// New: CORREL and related regression functions
Value correl(const std::vector<Value>& args, const Context& context);
Value rsq(const std::vector<Value>& args, const Context& context);
Value slope(const std::vector<Value>& args, const Context& context);
Value intercept(const std::vector<Value>& args, const Context& context);
Value sumx2my2(const std::vector<Value>& args, const Context& context);
Value sumx2py2(const std::vector<Value>& args, const Context& context);
Value sumxmy2(const std::vector<Value>& args, const Context& context);

// New: ROMAN and ARABIC
Value roman(const std::vector<Value>& args, const Context& context);
Value arabic(const std::vector<Value>& args, const Context& context);

/**
 * @brief ABS function - returns absolute value
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for ABS)
 * @return Absolute value
 */
Value abs_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ROUND function - rounds number to specified decimal places
 * @param args Function arguments (number, decimal_places)
 * @param context Evaluation context (unused for ROUND)
 * @return Rounded number
 */
Value round_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ROUNDUP function - rounds a number up, away from zero
 * @param args Function arguments (number, num_digits)
 * @param context Evaluation context (unused for ROUNDUP)
 * @return Number rounded up to specified decimal places
 */
Value roundup(const std::vector<Value>& args, const Context& context);

/**
 * @brief ROUNDDOWN function - rounds a number down, toward zero
 * @param args Function arguments (number, num_digits)
 * @param context Evaluation context (unused for ROUNDDOWN)
 * @return Number rounded down to specified decimal places
 */
Value rounddown(const std::vector<Value>& args, const Context& context);

/**
 * @brief MROUND function - rounds a number to the nearest multiple
 * @param args Function arguments (number, multiple)
 * @param context Evaluation context (unused for MROUND)
 * @return Number rounded to the nearest multiple
 */
Value mround(const std::vector<Value>& args, const Context& context);

/**
 * @brief SUMSQ function - returns the sum of the squares of the arguments
 * @param args Function arguments (number values)
 * @param context Evaluation context (unused for SUMSQ)
 * @return Sum of squares of all numeric arguments
 */
Value sumsq(const std::vector<Value>& args, const Context& context);

/**
 * @brief QUOTIENT function - returns the integer portion of a division
 * @param args Function arguments (numerator, denominator)
 * @param context Evaluation context (unused for QUOTIENT)
 * @return Integer portion of numerator divided by denominator
 */
Value quotient(const std::vector<Value>& args, const Context& context);

/**
 * @brief EVEN function - rounds a number up to the nearest even integer
 * @param args Function arguments (number)
 * @param context Evaluation context (unused for EVEN)
 * @return Nearest even integer >= number (for positive) or <= number (for negative)
 */
Value even_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ODD function - rounds a number up to the nearest odd integer
 * @param args Function arguments (number)
 * @param context Evaluation context (unused for ODD)
 * @return Nearest odd integer >= number (for positive) or <= number (for negative)
 */
Value odd_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief MIN function - returns minimum value in a set of values
 * @param args Function arguments (values to compare)
 * @param context Evaluation context (unused for MIN)
 * @return Minimum value
 */
Value min(const std::vector<Value>& args, const Context& context);

/**
 * @brief AVERAGE function - returns average of arguments
 * @param args Function arguments (numeric values)
 * @param context Evaluation context (unused for AVERAGE)
 * @return Average value
 */
Value average(const std::vector<Value>& args, const Context& context);

/**
 * @brief COUNT function - counts numeric values
 * @param args Function arguments (values to count)
 * @param context Evaluation context (unused for COUNT)
 * @return Count of numeric values
 */
Value count(const std::vector<Value>& args, const Context& context);

/**
 * @brief COUNTA function - counts non-empty values
 * @param args Function arguments (values to count)
 * @param context Evaluation context (unused for COUNTA)
 * @return Count of non-empty values
 */
Value counta(const std::vector<Value>& args, const Context& context);

/**
 * @brief SQRT function - returns square root
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for SQRT)
 * @return Square root
 */
Value sqrt_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief POWER function - returns number raised to a power
 * @param args Function arguments (base, exponent)
 * @param context Evaluation context (unused for POWER)
 * @return Base raised to exponent
 */
Value power(const std::vector<Value>& args, const Context& context);

/**
 * @brief MOD function - returns remainder from division
 * @param args Function arguments (numerator, divisor)
 * @param context Evaluation context (unused for MOD)
 * @return Remainder
 */
Value mod(const std::vector<Value>& args, const Context& context);

/**
 * @brief PI function - returns the value of pi
 * @param args Function arguments (should be empty)
 * @param context Evaluation context (unused for PI)
 * @return Value of pi
 */
Value pi(const std::vector<Value>& args, const Context& context);

/**
 * @brief SIGN function - returns the sign of a number
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for SIGN)
 * @return 1 if positive, -1 if negative, 0 if zero
 */
Value sign(const std::vector<Value>& args, const Context& context);

/**
 * @brief INT function - rounds a number down to the nearest integer
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for INT)
 * @return Integer part of the number (rounded down)
 */
Value int_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief TRUNC function - truncates a number to an integer
 * @param args Function arguments (expects 1 or 2 arguments)
 * @param context Evaluation context (unused for TRUNC)
 * @return Truncated number
 */
Value trunc_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief CEILING function - rounds a number up to the nearest multiple
 * @param args Function arguments (expects 1 or 2 arguments)
 * @param context Evaluation context (unused for CEILING)
 * @return Number rounded up to nearest multiple
 */
Value ceiling_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief FLOOR function - rounds a number down to the nearest multiple
 * @param args Function arguments (expects 1 or 2 arguments)
 * @param context Evaluation context (unused for FLOOR)
 * @return Number rounded down to nearest multiple
 */
Value floor_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief RAND function - returns a random number between 0 and 1
 * @param args Function arguments (should be empty)
 * @param context Evaluation context (unused for RAND)
 * @return Random number between 0 and 1
 */
Value rand_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief RANDBETWEEN function - returns a random number between specified numbers
 * @param args Function arguments (expects 2 numeric arguments)
 * @param context Evaluation context (unused for RANDBETWEEN)
 * @return Random integer between bottom and top (inclusive)
 */
Value randbetween(const std::vector<Value>& args, const Context& context);

/**
 * @brief COUNTIF function - counts the number of cells that meet a criterion
 * @param args Function arguments (expects 2+ arguments: range..., criteria)
 * @param context Evaluation context (unused for COUNTIF)
 * @return Count of values that match criteria
 */
Value countif(const std::vector<Value>& args, const Context& context);

/**
 * @brief MEDIAN function - returns the median of a set of numbers
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for MEDIAN)
 * @return Median value of the arguments
 */
Value median(const std::vector<Value>& args, const Context& context);

/**
 * @brief MODE function - returns the most frequently occurring value
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for MODE)
 * @return Most frequently occurring value
 */
Value mode(const std::vector<Value>& args, const Context& context);

/**
 * @brief STDEV function - returns the standard deviation of a set of numbers
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for STDEV)
 * @return Standard deviation of the arguments
 */
Value stdev(const std::vector<Value>& args, const Context& context);

/**
 * @brief VAR function - returns the variance of a set of numbers
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for VAR)
 * @return Variance of the arguments
 */
Value var(const std::vector<Value>& args, const Context& context);

/**
 * @brief FIND function - finds one text string within another (case-sensitive)
 * @param args Function arguments (find_text, within_text, [start_num])
 * @param context Evaluation context (unused for FIND)
 * @return Position of find_text within within_text, or #VALUE! if not found
 */
Value find(const std::vector<Value>& args, const Context& context);

/**
 * @brief SEARCH function - finds one text string within another (case-insensitive)
 * @param args Function arguments (find_text, within_text, [start_num])
 * @param context Evaluation context (unused for SEARCH)
 * @return Position of find_text within within_text, or #VALUE! if not found
 */
Value search(const std::vector<Value>& args, const Context& context);

/**
 * @brief REPLACE function - replaces part of a text string
 * @param args Function arguments (old_text, start_num, num_chars, new_text)
 * @param context Evaluation context (unused for REPLACE)
 * @return Text with the specified characters replaced
 */
Value replace(const std::vector<Value>& args, const Context& context);

/**
 * @brief SUBSTITUTE function - substitutes new text for old text
 * @param args Function arguments (text, old_text, new_text, [instance_num])
 * @param context Evaluation context (unused for SUBSTITUTE)
 * @return Text with old_text replaced by new_text
 */
Value substitute(const std::vector<Value>& args, const Context& context);

/**
 * @brief TEXT function - formats a number as text
 * @param args Function arguments (value, format_text)
 * @param context Evaluation context (unused for TEXT)
 * @return Formatted text string
 */
Value text(const std::vector<Value>& args, const Context& context);

/**
 * @brief VALUE function - converts a text string to a number
 * @param args Function arguments (text)
 * @param context Evaluation context (unused for VALUE)
 * @return Numeric value
 */
Value value(const std::vector<Value>& args, const Context& context);

/**
 * @brief SIN function - returns the sine of an angle
 * @param args Function arguments (expects 1 numeric argument in radians)
 * @param context Evaluation context (unused for SIN)
 * @return Sine of the angle
 */
Value sin_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief COS function - returns the cosine of an angle
 * @param args Function arguments (expects 1 numeric argument in radians)
 * @param context Evaluation context (unused for COS)
 * @return Cosine of the angle
 */
Value cos_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief TAN function - returns the tangent of an angle
 * @param args Function arguments (expects 1 numeric argument in radians)
 * @param context Evaluation context (unused for TAN)
 * @return Tangent of the angle
 */
Value tan_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ASIN function - returns the arcsine
 * @param args Function arguments (expects 1 numeric argument between -1 and 1)
 * @param context Evaluation context (unused for ASIN)
 * @return Arcsine in radians
 */
Value asin_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ACOS function - returns the arccosine
 * @param args Function arguments (expects 1 numeric argument between -1 and 1)
 * @param context Evaluation context (unused for ACOS)
 * @return Arccosine in radians
 */
Value acos_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ATAN function - returns the arctangent
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for ATAN)
 * @return Arctangent in radians
 */
Value atan_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ATAN2 function - returns the arctangent from x and y coordinates
 * @param args Function arguments (expects 2 numeric arguments: y, x)
 * @param context Evaluation context (unused for ATAN2)
 * @return Arctangent in radians
 */
Value atan2_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief SINH function - returns the hyperbolic sine
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for SINH)
 * @return Hyperbolic sine
 */
Value sinh_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief COSH function - returns the hyperbolic cosine
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for COSH)
 * @return Hyperbolic cosine
 */
Value cosh_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief TANH function - returns the hyperbolic tangent
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for TANH)
 * @return Hyperbolic tangent
 */
Value tanh_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief DEGREES function - converts radians to degrees
 * @param args Function arguments (expects 1 numeric argument in radians)
 * @param context Evaluation context (unused for DEGREES)
 * @return Angle in degrees
 */
Value degrees_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief RADIANS function - converts degrees to radians
 * @param args Function arguments (expects 1 numeric argument in degrees)
 * @param context Evaluation context (unused for RADIANS)
 * @return Angle in radians
 */
Value radians_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief EXP function - returns e raised to a power
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for EXP)
 * @return e raised to the power
 */
Value exp_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief LN function - returns the natural logarithm
 * @param args Function arguments (expects 1 positive numeric argument)
 * @param context Evaluation context (unused for LN)
 * @return Natural logarithm
 */
Value ln_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief LOG function - returns the logarithm to a specified base
 * @param args Function arguments (expects 1 or 2 arguments: number, [base])
 * @param context Evaluation context (unused for LOG)
 * @return Logarithm to the specified base (default 10)
 */
Value log_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief LOG10 function - returns the base-10 logarithm
 * @param args Function arguments (expects 1 positive numeric argument)
 * @param context Evaluation context (unused for LOG10)
 * @return Base-10 logarithm
 */
Value log10_function(const std::vector<Value>& args, const Context& context);

// Date & Time Functions

/**
 * @brief NOW function - returns the current date and time
 * @param args Function arguments (should be empty)
 * @param context Evaluation context (unused for NOW)
 * @return Current date and time
 */
Value now(const std::vector<Value>& args, const Context& context);

/**
 * @brief TODAY function - returns the current date
 * @param args Function arguments (should be empty)
 * @param context Evaluation context (unused for TODAY)
 * @return Current date at midnight
 */
Value today(const std::vector<Value>& args, const Context& context);

/**
 * @brief DATE function - creates a date from year, month, day
 * @param args Function arguments (year, month, day)
 * @param context Evaluation context (unused for DATE)
 * @return Date value created from the given components
 */
Value date(const std::vector<Value>& args, const Context& context);

/**
 * @brief TIME function - creates a time from hour, minute, second
 * @param args Function arguments (hour, minute, second)
 * @param context Evaluation context (unused for TIME)
 * @return Time value as fraction of a day
 */
Value time_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief YEAR function - returns the year from a date
 * @param args Function arguments (date)
 * @param context Evaluation context (unused for YEAR)
 * @return Year as numeric value
 */
Value year(const std::vector<Value>& args, const Context& context);

/**
 * @brief MONTH function - returns the month from a date
 * @param args Function arguments (date)
 * @param context Evaluation context (unused for MONTH)
 * @return Month as numeric value (1-12)
 */
Value month(const std::vector<Value>& args, const Context& context);

/**
 * @brief DAY function - returns the day from a date
 * @param args Function arguments (date)
 * @param context Evaluation context (unused for DAY)
 * @return Day as numeric value (1-31)
 */
Value day(const std::vector<Value>& args, const Context& context);

/**
 * @brief HOUR function - returns the hour from a time/date
 * @param args Function arguments (time/date)
 * @param context Evaluation context (unused for HOUR)
 * @return Hour as numeric value (0-23)
 */
Value hour(const std::vector<Value>& args, const Context& context);

/**
 * @brief MINUTE function - returns the minute from a time/date
 * @param args Function arguments (time/date)
 * @param context Evaluation context (unused for MINUTE)
 * @return Minute as numeric value (0-59)
 */
Value minute(const std::vector<Value>& args, const Context& context);

/**
 * @brief SECOND function - returns the second from a time/date
 * @param args Function arguments (time/date)
 * @param context Evaluation context (unused for SECOND)
 * @return Second as numeric value (0-59)
 */
Value second(const std::vector<Value>& args, const Context& context);

/**
 * @brief WEEKDAY function - returns the day of the week
 * @param args Function arguments (date, [return_type])
 * @param context Evaluation context (unused for WEEKDAY)
 * @return Day of the week as numeric value
 */
Value weekday(const std::vector<Value>& args, const Context& context);

/**
 * @brief DATEDIF function - calculates the difference between dates
 * @param args Function arguments (start_date, end_date, unit)
 * @param context Evaluation context (unused for DATEDIF)
 * @return Date difference in specified unit
 */
Value datedif(const std::vector<Value>& args, const Context& context);
Value edate(const std::vector<Value>& args, const Context& context);
Value eomonth(const std::vector<Value>& args, const Context& context);

// Engineering & Specialized Functions

/**
 * @brief CONVERT function - converts units
 * @param args Function arguments (number, from_unit, to_unit)
 * @param context Evaluation context (unused for CONVERT)
 * @return Converted value
 */
Value convert(const std::vector<Value>& args, const Context& context);

/**
 * @brief HEX2DEC function - converts hexadecimal to decimal
 * @param args Function arguments (hex_string)
 * @param context Evaluation context (unused for HEX2DEC)
 * @return Decimal value
 */
Value hex2dec(const std::vector<Value>& args, const Context& context);

/**
 * @brief DEC2HEX function - converts decimal to hexadecimal
 * @param args Function arguments (number, [places])
 * @param context Evaluation context (unused for DEC2HEX)
 * @return Hexadecimal string
 */
Value dec2hex(const std::vector<Value>& args, const Context& context);

/**
 * @brief BIN2DEC function - converts binary to decimal
 * @param args Function arguments (binary_string)
 * @param context Evaluation context (unused for BIN2DEC)
 * @return Decimal value
 */
Value bin2dec(const std::vector<Value>& args, const Context& context);

/**
 * @brief DEC2BIN function - converts decimal to binary
 * @param args Function arguments (number, [places])
 * @param context Evaluation context (unused for DEC2BIN)
 * @return Binary string
 */
Value dec2bin(const std::vector<Value>& args, const Context& context);

/**
 * @brief BITAND function - bitwise AND
 * @param args Function arguments (number1, number2)
 * @param context Evaluation context (unused for BITAND)
 * @return Result of bitwise AND
 */
Value bitand_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief BITOR function - bitwise OR
 * @param args Function arguments (number1, number2)
 * @param context Evaluation context (unused for BITOR)
 * @return Result of bitwise OR
 */
Value bitor_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief BITXOR function - bitwise XOR
 * @param args Function arguments (number1, number2)
 * @param context Evaluation context (unused for BITXOR)
 * @return Result of bitwise XOR
 */
Value bitxor_function(const std::vector<Value>& args, const Context& context);

// Engineering: additional base conversions
Value dec2oct(const std::vector<Value>& args, const Context& context);
Value bin2oct(const std::vector<Value>& args, const Context& context);
Value oct2bin(const std::vector<Value>& args, const Context& context);
Value hex2oct(const std::vector<Value>& args, const Context& context);
Value oct2hex(const std::vector<Value>& args, const Context& context);

// Engineering: complex number helpers
Value complex_function(const std::vector<Value>& args, const Context& context);
Value imreal(const std::vector<Value>& args, const Context& context);
Value imaginary(const std::vector<Value>& args, const Context& context);
Value imabs(const std::vector<Value>& args, const Context& context);
Value imargument(const std::vector<Value>& args, const Context& context);
Value imsum(const std::vector<Value>& args, const Context& context);
Value imsub(const std::vector<Value>& args, const Context& context);
Value improduct(const std::vector<Value>& args, const Context& context);
Value imdiv(const std::vector<Value>& args, const Context& context);
Value impower(const std::vector<Value>& args, const Context& context);

// Financial Functions

/**
 * @brief PV function - calculates present value
 * @param args Function arguments (rate, nper, pmt, [fv], [type])
 * @param context Evaluation context (unused for PV)
 * @return Present value
 */
Value pv(const std::vector<Value>& args, const Context& context);

/**
 * @brief FV function - calculates future value
 * @param args Function arguments (rate, nper, pmt, [pv], [type])
 * @param context Evaluation context (unused for FV)
 * @return Future value
 */
Value fv(const std::vector<Value>& args, const Context& context);

/**
 * @brief PMT function - calculates payment amount
 * @param args Function arguments (rate, nper, pv, [fv], [type])
 * @param context Evaluation context (unused for PMT)
 * @return Payment amount
 */
Value pmt(const std::vector<Value>& args, const Context& context);

/**
 * @brief RATE function - calculates interest rate
 * @param args Function arguments (nper, pmt, pv, [fv], [type], [guess])
 * @param context Evaluation context (unused for RATE)
 * @return Interest rate
 */
Value rate(const std::vector<Value>& args, const Context& context);

/**
 * @brief NPER function - calculates number of periods
 * @param args Function arguments (rate, pmt, pv, [fv], [type])
 * @param context Evaluation context (unused for NPER)
 * @return Number of periods
 */
Value nper(const std::vector<Value>& args, const Context& context);

/**
 * @brief NPV function - calculates net present value
 * @param args Function arguments (rate, value1, [value2], ...)
 * @param context Evaluation context (unused for NPV)
 * @return Net present value
 */
Value npv(const std::vector<Value>& args, const Context& context);

/**
 * @brief IRR function - calculates internal rate of return
 * @param args Function arguments (values, [guess])
 * @param context Evaluation context (unused for IRR)
 * @return Internal rate of return
 */
Value irr(const std::vector<Value>& args, const Context& context);

/**
 * @brief MIRR function - calculates modified internal rate of return
 * @param args Function arguments (values, finance_rate, reinvest_rate)
 * @param context Evaluation context (unused for MIRR)
 * @return Modified internal rate of return
 */
Value mirr(const std::vector<Value>& args, const Context& context);

/**
 * @brief GCD function - calculates greatest common divisor
 * @param args Function arguments (numbers)
 * @param context Evaluation context
 * @return Greatest common divisor
 */
Value gcd(const std::vector<Value>& args, const Context& context);

/**
 * @brief LCM function - calculates least common multiple
 * @param args Function arguments (numbers)
 * @param context Evaluation context
 * @return Least common multiple
 */
Value lcm(const std::vector<Value>& args, const Context& context);

/**
 * @brief FACT function - calculates factorial
 * @param args Function arguments (number)
 * @param context Evaluation context
 * @return Factorial of the number
 */
Value fact(const std::vector<Value>& args, const Context& context);

/**
 * @brief COMBIN function - calculates number of combinations
 * @param args Function arguments (n, k)
 * @param context Evaluation context
 * @return Number of combinations C(n,k)
 */
Value combin(const std::vector<Value>& args, const Context& context);

/**
 * @brief PERMUT function - calculates number of permutations
 * @param args Function arguments (n, k)
 * @param context Evaluation context
 * @return Number of permutations P(n,k)
 */
Value permut(const std::vector<Value>& args, const Context& context);

/**
 * @brief SUMPRODUCT function - calculates sum of products
 * @param args Function arguments (arrays/values)
 * @param context Evaluation context
 * @return Sum of products
 */
Value sumproduct(const std::vector<Value>& args, const Context& context);

/**
 * @brief SUMIF function - sums values that meet a criterion
 * @param args Function arguments (range, criteria, [sum_range])
 * @param context Evaluation context
 * @return Sum of values meeting criteria
 */
Value sumif(const std::vector<Value>& args, const Context& context);

/**
 * @brief SUMIFS function - sums values that meet multiple criteria
 * @param args Function arguments (sum_range, criteria_range1, criteria1, ...)
 * @param context Evaluation context
 * @return Sum of values meeting all criteria
 */
Value sumifs(const std::vector<Value>& args, const Context& context);

/**
 * @brief AVERAGEIF function - averages values that meet a criterion
 * @param args Function arguments (range, criteria, [average_range])
 * @param context Evaluation context
 * @return Average of values meeting criteria
 */
Value averageif(const std::vector<Value>& args, const Context& context);

/**
 * @brief AVERAGEIFS function - averages values that meet multiple criteria
 * @param args Function arguments (average_range, criteria_range1, criteria1, ...)
 * @param context Evaluation context
 * @return Average of values meeting all criteria
 */
Value averageifs(const std::vector<Value>& args, const Context& context);

/**
 * @brief Helper function for simple wildcard matching without regex
 * @param text The text to match
 * @param pattern Pattern with * and ? wildcards
 * @return true if text matches pattern
 */
bool simpleWildcardMatch(const std::string& text, const std::string& pattern);

}  // namespace builtin

/**
 * @brief Built-in function dispatcher using perfect hash for optimal performance
 */
namespace dispatcher {

/**
 * @brief Compile-time hash function for function names
 * Uses FNV-1a hash algorithm for good distribution and perfect hash properties
 */
constexpr uint32_t hash_function_name(const char* str) {
    uint32_t hash = 2166136261u;  // FNV-1a offset basis
    while (*str) {
        hash ^= static_cast<uint32_t>(*str++);
        hash *= 16777619u;  // FNV-1a prime
    }
    return hash;
}

/**
 * @brief Fast built-in function dispatcher using perfect hash
 * @param name Function name (must be uppercase)
 * @param args Function arguments
 * @param context Evaluation context
 * @return Function result or empty Value if not a built-in function
 */
Value dispatch_builtin_function(const std::string& name, const std::vector<Value>& args,
                                const Context& context);

/**
 * @brief Get list of all built-in function names
 * @return Vector of all built-in function names (uppercase)
 */
std::vector<std::string> get_builtin_function_names();

}  // namespace dispatcher

/**
 * @brief Utility functions for argument validation
 */
namespace utils {

/**
 * @brief Validate minimum number of arguments
 * @param args Arguments to validate
 * @param min_count Minimum required arguments
 * @param function_name Function name for error messages
 * @return Error value if validation fails, empty value if success
 */
Value validateMinArgs(const std::vector<Value>& args, size_t min_count,
                      const std::string& function_name);

/**
 * @brief Validate exact number of arguments
 * @param args Arguments to validate
 * @param count Required number of arguments
 * @param function_name Function name for error messages
 * @return Error value if validation fails, empty value if success
 */
Value validateArgCount(const std::vector<Value>& args, size_t count,
                       const std::string& function_name);

/**
 * @brief Check if any argument is an error
 * @param args Arguments to check
 * @return First error found, or empty value if no errors
 */
Value checkForErrors(const std::vector<Value>& args);

/**
 * @brief Convert value to number, handling errors
 * @param value Value to convert
 * @param function_name Function name for error messages
 * @return Converted number or error value
 */
Value toNumberSafe(const Value& value, const std::string& function_name);

}  // namespace utils

// Template function declarations for common patterns
namespace templates {

/**
 * @brief Template for single-argument numeric functions
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The numeric operation to perform
 * @return Result of the operation
 */
template <typename Func>
Value singleNumericFunction(const std::vector<Value>& args, const Context& context,
                            const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, name);
    if (!validation.isEmpty()) {
        return validation;
    }

    auto num = utils::toNumberSafe(args[0], name);
    if (num.isError()) {
        return num;
    }

    try {
        return Value(operation(num.asNumber()));
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::NUM_ERROR);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for multi-argument numeric functions
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The numeric operation to perform
 * @return Result of the operation
 */
template <typename Func>
Value multiNumericFunction(const std::vector<Value>& args, const Context& context,
                           const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    (void)name;     // Unused parameter

    // Allow empty arguments for functions like SUM
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::vector<double> numbers;
    numbers.reserve(args.size());

    for (const auto& arg : args) {
        if (!arg.isEmpty() && arg.canConvertToNumber()) {
            numbers.push_back(arg.toNumber());
        }
    }

    if (numbers.empty()) {
        return Value(0.0);
    }

    try {
        return Value(operation(numbers));
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::DIV_ZERO);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for single-argument text functions
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The text operation to perform
 * @return Result of the operation
 */
template <typename Func>
Value singleTextFunction(const std::vector<Value>& args, const Context& context,
                         const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, name);
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert to text if needed
    std::string text;
    if (args[0].isText()) {
        text = args[0].asText();
    } else {
        text = args[0].toString();
    }

    return Value(operation(text));
}

/**
 * @brief Template for no-argument functions
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform
 * @return Result of the operation
 */
template <typename Func>
Value noArgFunction(const std::vector<Value>& args, const Context& context, const std::string& name,
                    Func operation) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 0, name);
    if (!validation.isEmpty()) {
        return validation;
    }

    return operation();
}

/**
 * @brief Template for functions that take 1-2 arguments (like ROUND, FLOOR, CEILING, TRUNC)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform (takes value and optional second arg)
 * @return Result of the operation
 */
template <typename Func>
Value oneOrTwoArgFunction(const std::vector<Value>& args, const Context& context,
                          const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    // Validate argument count (1 or 2 arguments)
    if (args.size() < 1 || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert first argument to number
    auto number = utils::toNumberSafe(args[0], name);
    if (number.isError()) {
        return number;
    }

    double value = number.asNumber();

    // If second argument provided, convert it too
    if (args.size() == 2) {
        auto second = utils::toNumberSafe(args[1], name);
        if (second.isError()) {
            return second;
        }

        try {
            return Value(operation(value, second.asNumber()));
        } catch (const std::runtime_error&) {
            return Value::error(ErrorType::NUM_ERROR);
        } catch (...) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    } else {
        // Only one argument
        try {
            return Value(operation(value, 0.0));  // Default second argument
        } catch (const std::runtime_error&) {
            return Value::error(ErrorType::NUM_ERROR);
        } catch (...) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    }
}

/**
 * @brief Template for text functions that take 1-2 arguments (like LEFT, RIGHT)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The text operation to perform (takes text and optional num_chars)
 * @return Result of the operation
 */
template <typename Func>
Value oneOrTwoArgTextFunction(const std::vector<Value>& args, const Context& context,
                              const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    (void)name;     // Unused parameter

    // Validate argument count (1 or 2 arguments)
    if (args.size() < 1 || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert first argument to text
    std::string text = args[0].toString();

    // If second argument provided, validate it's a number
    int num_chars = 1;  // Default value
    if (args.size() == 2) {
        if (!args[1].isNumber()) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        num_chars = static_cast<int>(args[1].asNumber());

        // If num_chars is negative, return empty string
        if (num_chars < 0) {
            return Value("");
        }
    }

    return Value(operation(text, num_chars));
}

/**
 * @brief Template for functions that take exactly 2 arguments: text + number
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform (takes text and number)
 * @return Result of the operation
 */
template <typename Func>
Value twoArgTextNumberFunction(const std::vector<Value>& args, const Context& context,
                              const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    // Validate argument count (exactly 2 arguments)
    auto error = utils::validateArgCount(args, 2, name);
    if (!error.isEmpty()) {
        return error;
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert first argument to text
    std::string text = args[0].toString();

    // Convert second argument to number safely
    auto numResult = utils::toNumberSafe(args[1], name);
    if (numResult.isError()) {
        return numResult;
    }

    double number = numResult.asNumber();

    return Value(operation(text, number));
}

/**
 * @brief Template for functions that take any number of arguments (like CONCATENATE)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform (takes all arguments)
 * @return Result of the operation
 */
template <typename Func>
Value multiArgFunction(const std::vector<Value>& args, const Context& context,
                       const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    (void)name;     // Unused parameter

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    return Value(operation(args));
}

/**
 * @brief Template for single date argument functions (YEAR, MONTH, DAY)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform on the tm structure
 * @return Result of the operation
 */
template <typename Func>
Value singleDateFunction(const std::vector<Value>& args, const Context& context,
                         const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, name);
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    if (!args[0].isDate()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    try {
        auto date_val = args[0].asDate();
        auto time_t = std::chrono::system_clock::to_time_t(date_val);
        auto local_tm = *std::localtime(&time_t);

        return Value(static_cast<double>(operation(local_tm)));
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for date/time extraction functions (HOUR, MINUTE, SECOND)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param dateOperation Operation to perform on tm structure for date values
 * @param fractionOperation Operation to perform on time fraction for numeric values
 * @return Result of the operation
 */
template <typename DateFunc, typename FractionFunc>
Value dateTimeExtractionFunction(const std::vector<Value>& args, const Context& context,
                                 const std::string& name, DateFunc dateOperation,
                                 FractionFunc fractionOperation) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, name);
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    if (args[0].isDate()) {
        // Handle date value
        try {
            auto date_val = args[0].asDate();
            auto time_t = std::chrono::system_clock::to_time_t(date_val);
            auto local_tm = *std::localtime(&time_t);

            return Value(static_cast<double>(dateOperation(local_tm)));
        } catch (...) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    } else if (args[0].canConvertToNumber()) {
        // Handle time fraction (Excel-style)
        double time_fraction = args[0].toNumber();
        return Value(static_cast<double>(fractionOperation(time_fraction)));
    } else {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for three numeric argument functions (DATE, TIME)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform with the three numbers
 * @return Result of the operation
 */
template <typename Func>
Value threeNumberFunction(const std::vector<Value>& args, const Context& context,
                          const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 3, name);
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert arguments to numbers
    if (!args[0].canConvertToNumber() || !args[1].canConvertToNumber() ||
        !args[2].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    int first = static_cast<int>(args[0].toNumber());
    int second = static_cast<int>(args[1].toNumber());
    int third = static_cast<int>(args[2].toNumber());

    try {
        return operation(first, second, third);
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::NUM_ERROR);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for base conversion functions (like HEX2DEC, BIN2DEC)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The conversion operation to perform
 * @return Result of the conversion
 */
template <typename Func>
Value baseConversionFunction(const std::vector<Value>& args, const Context& context,
                             const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, name);
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert to text string for processing
    std::string input_str = args[0].toString();

    try {
        return Value(operation(input_str));
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::NUM_ERROR);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for decimal to base conversion functions (like DEC2HEX, DEC2BIN)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The conversion operation to perform
 * @return Result of the conversion
 */
template <typename Func>
Value decimalToBaseFunction(const std::vector<Value>& args, const Context& context,
                            const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    // Accept 1 or 2 arguments (number, [places])
    if (args.size() < 1 || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert first argument to number
    auto number = utils::toNumberSafe(args[0], name);
    if (number.isError()) {
        return number;
    }

    long long value = static_cast<long long>(number.asNumber());
    int places = 0;  // Default: no padding

    // Handle optional places argument
    if (args.size() == 2) {
        auto places_val = utils::toNumberSafe(args[1], name);
        if (places_val.isError()) {
            return places_val;
        }
        places = static_cast<int>(places_val.asNumber());
        if (places < 0) {
            return Value::error(ErrorType::NUM_ERROR);
        }
    }

    try {
        return Value(operation(value, places));
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::NUM_ERROR);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for bitwise operation functions (like BITAND, BITOR, BITXOR)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The bitwise operation to perform
 * @return Result of the operation
 */
template <typename Func>
Value bitwiseFunction(const std::vector<Value>& args, const Context& context,
                      const std::string& name, Func operation) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 2, name);
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert arguments to integers
    auto num1 = utils::toNumberSafe(args[0], name);
    if (num1.isError()) {
        return num1;
    }

    auto num2 = utils::toNumberSafe(args[1], name);
    if (num2.isError()) {
        return num2;
    }

    long long value1 = static_cast<long long>(num1.asNumber());
    long long value2 = static_cast<long long>(num2.asNumber());

    // Validate range (48-bit integers for Excel compatibility)
    if (value1 < 0 || value1 > 281474976710655LL || value2 < 0 || value2 > 281474976710655LL) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    try {
        return Value(static_cast<double>(operation(value1, value2)));
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for time value of money functions with 3-5 arguments
 * @param args Function arguments
 * @param context Evaluation context (unused)
 * @param name Function name for error messages
 * @param operation Lambda function that performs the calculation
 * @return Result of the calculation
 */
template <typename Func>
Value financialFunction(const std::vector<Value>& args, const Context& context,
                        const std::string& name, size_t min_args, size_t max_args, Func operation) {
    (void)context;  // Unused parameter
    (void)name;     // Unused parameter

    try {
        // Validate argument count
        if (args.size() < min_args || args.size() > max_args) {
            return Value::error(ErrorType::VALUE_ERROR);
        }

        // Check for errors in arguments
        auto errorCheck = utils::checkForErrors(args);
        if (!errorCheck.isEmpty()) {
            return errorCheck;
        }

        // Convert arguments to numbers
        std::vector<double> numeric_args;
        for (const auto& arg : args) {
            auto num_val = utils::toNumberSafe(arg, name);
            if (num_val.isError()) {
                return num_val;
            }
            numeric_args.push_back(num_val.asNumber());
        }

        return operation(numeric_args);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for cash flow functions (NPV, IRR, MIRR)
 * @param args Function arguments
 * @param context Evaluation context (unused)
 * @param name Function name for error messages
 * @param operation Lambda function that performs the calculation
 * @return Result of the calculation
 */
template <typename Func>
Value cashFlowFunction(const std::vector<Value>& args, const Context& context,
                       const std::string& name, size_t min_args, Func operation) {
    (void)context;  // Unused parameter
    (void)name;     // Unused parameter

    try {
        // Validate minimum argument count
        if (args.size() < min_args) {
            return Value::error(ErrorType::VALUE_ERROR);
        }

        // Check for errors in arguments
        auto errorCheck = utils::checkForErrors(args);
        if (!errorCheck.isEmpty()) {
            return errorCheck;
        }

        // Convert arguments to numbers
        std::vector<double> numeric_args;
        for (const auto& arg : args) {
            auto num_val = utils::toNumberSafe(arg, name);
            if (num_val.isError()) {
                return num_val;
            }
            numeric_args.push_back(num_val.asNumber());
        }

        return operation(numeric_args);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace templates

/**
 * @brief Utility functions for date/time operations
 */
namespace datetime_utils {

/**
 * @brief Extract fractional part of time and normalize to [0,1)
 * @param time_fraction Input time fraction
 * @return Normalized fractional part
 */
inline double normalizeFraction(double time_fraction) {
    time_fraction = time_fraction - std::floor(time_fraction);
    if (time_fraction < 0)
        time_fraction += 1.0;
    return time_fraction;
}

/**
 * @brief Convert time fraction to total seconds in the day
 * @param time_fraction Normalized time fraction
 * @return Total seconds
 */
inline double fractionToSeconds(double time_fraction) {
    return time_fraction * 24.0 * 60.0 * 60.0;
}

/**
 * @brief Extract hour from time fraction
 * @param time_fraction Input time fraction
 * @return Hour (0-23)
 */
inline int extractHourFromFraction(double time_fraction) {
    double normalized = normalizeFraction(time_fraction);
    double total_seconds = fractionToSeconds(normalized);
    return static_cast<int>(total_seconds / 3600.0) % 24;
}

/**
 * @brief Extract minute from time fraction
 * @param time_fraction Input time fraction
 * @return Minute (0-59)
 */
inline int extractMinuteFromFraction(double time_fraction) {
    double normalized = normalizeFraction(time_fraction);
    double total_seconds = fractionToSeconds(normalized);
    return static_cast<int>((total_seconds / 60.0)) % 60;
}

/**
 * @brief Extract second from time fraction
 * @param time_fraction Input time fraction
 * @return Second (0-59)
 */
inline int extractSecondFromFraction(double time_fraction) {
    double normalized = normalizeFraction(time_fraction);
    double total_seconds = fractionToSeconds(normalized);
    return static_cast<int>(total_seconds) % 60;
}

}  // namespace datetime_utils

}  // namespace functions
}  // namespace xl_formula