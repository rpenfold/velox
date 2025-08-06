#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class ConvertFunctionTest : public ::testing::Test {
protected:
    Context context;
    
    Value callConvert(const std::vector<Value>& args) {
        return convert(args, context);
    }
};

// Basic functionality tests
TEST_F(ConvertFunctionTest, NoArguments_ReturnsError) {
    auto result = callConvert({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ConvertFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callConvert({Value(1.0), Value("m")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ConvertFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callConvert({Value(1.0), Value("m"), Value("km"), Value("extra")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Distance conversions
TEST_F(ConvertFunctionTest, MetersToKilometers_ConvertsCorrectly) {
    auto result = callConvert({Value(1000.0), Value("m"), Value("km")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, InchesToFeet_ConvertsCorrectly) {
    auto result = callConvert({Value(12.0), Value("in"), Value("ft")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, KilometersToMiles_ConvertsCorrectly) {
    auto result = callConvert({Value(1.609344), Value("km"), Value("mi")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-5);
}

TEST_F(ConvertFunctionTest, CentimetersToMillimeters_ConvertsCorrectly) {
    auto result = callConvert({Value(1.0), Value("cm"), Value("mm")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(10.0, result.asNumber(), 1e-10);
}

// Weight conversions
TEST_F(ConvertFunctionTest, GramsToKilograms_ConvertsCorrectly) {
    auto result = callConvert({Value(1000.0), Value("g"), Value("kg")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, PoundsToGrams_ConvertsCorrectly) {
    auto result = callConvert({Value(1.0), Value("lbm"), Value("g")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(453.59237, result.asNumber(), 1e-5);
}

TEST_F(ConvertFunctionTest, OuncesToGrams_ConvertsCorrectly) {
    auto result = callConvert({Value(1.0), Value("ozm"), Value("g")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(28.349523125, result.asNumber(), 1e-6);
}

// Time conversions
TEST_F(ConvertFunctionTest, SecondsToMinutes_ConvertsCorrectly) {
    auto result = callConvert({Value(60.0), Value("sec"), Value("min")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, HoursToDays_ConvertsCorrectly) {
    auto result = callConvert({Value(24.0), Value("hr"), Value("day")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, MinutesToHours_ConvertsCorrectly) {
    auto result = callConvert({Value(120.0), Value("min"), Value("hr")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

// Temperature conversions
TEST_F(ConvertFunctionTest, CelsiusToKelvin_ConvertsCorrectly) {
    auto result = callConvert({Value(0.0), Value("C"), Value("K")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(273.15, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, FahrenheitToCelsius_ConvertsCorrectly) {
    auto result = callConvert({Value(32.0), Value("F"), Value("C")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, CelsiusToFahrenheit_ConvertsCorrectly) {
    auto result = callConvert({Value(100.0), Value("C"), Value("F")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(212.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, KelvinToCelsius_ConvertsCorrectly) {
    auto result = callConvert({Value(373.15), Value("K"), Value("C")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(100.0, result.asNumber(), 1e-10);
}

// Energy conversions
TEST_F(ConvertFunctionTest, CaloriesToJoules_ConvertsCorrectly) {
    auto result = callConvert({Value(1.0), Value("cal"), Value("j")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(4.184, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, WattHoursToJoules_ConvertsCorrectly) {
    auto result = callConvert({Value(1.0), Value("wh"), Value("j")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(3600.0, result.asNumber(), 1e-10);
}

// Power conversions
TEST_F(ConvertFunctionTest, HorsepowerToWatts_ConvertsCorrectly) {
    auto result = callConvert({Value(1.0), Value("hp"), Value("w")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(745.69987158227, result.asNumber(), 1e-8);
}

// Liquid measure conversions
TEST_F(ConvertFunctionTest, GallonsToLiters_ConvertsCorrectly) {
    auto result = callConvert({Value(1.0), Value("gal"), Value("l")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(3.785411784, result.asNumber(), 1e-9);
}

TEST_F(ConvertFunctionTest, CupsToOunces_ConvertsCorrectly) {
    auto result = callConvert({Value(1.0), Value("cup"), Value("oz")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(8.0, result.asNumber(), 1e-10);
}

// Case insensitive tests
TEST_F(ConvertFunctionTest, UpperCaseUnits_WorksCorrectly) {
    auto result = callConvert({Value(1000.0), Value("M"), Value("KM")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, MixedCaseUnits_WorksCorrectly) {
    auto result = callConvert({Value(1.0), Value("Kg"), Value("g")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1000.0, result.asNumber(), 1e-10);
}

// Error cases
TEST_F(ConvertFunctionTest, UnknownFromUnit_ReturnsError) {
    auto result = callConvert({Value(1.0), Value("xyz"), Value("m")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ConvertFunctionTest, UnknownToUnit_ReturnsError) {
    auto result = callConvert({Value(1.0), Value("m"), Value("xyz")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ConvertFunctionTest, IncompatibleUnits_ReturnsError) {
    auto result = callConvert({Value(1.0), Value("m"), Value("kg")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ConvertFunctionTest, DistanceToTime_ReturnsError) {
    auto result = callConvert({Value(1.0), Value("km"), Value("hr")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Type conversion tests
TEST_F(ConvertFunctionTest, TextNumberInput_ConvertsCorrectly) {
    auto result = callConvert({Value("1000"), Value("m"), Value("km")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, BooleanInput_ConvertsCorrectly) {
    auto result = callConvert({Value(true), Value("m"), Value("cm")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(100.0, result.asNumber(), 1e-10);  // 1 meter = 100 cm
}

// Error propagation
TEST_F(ConvertFunctionTest, ErrorInput_PropagatesError) {
    auto result = callConvert({Value::error(ErrorType::DIV_ZERO), Value("m"), Value("km")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ConvertFunctionTest, NonNumericValue_ReturnsError) {
    auto result = callConvert({Value("abc"), Value("m"), Value("km")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Same unit conversion
TEST_F(ConvertFunctionTest, SameUnits_ReturnsInput) {
    auto result = callConvert({Value(42.5), Value("m"), Value("m")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(42.5, result.asNumber(), 1e-10);
}

// Zero and negative values
TEST_F(ConvertFunctionTest, ZeroValue_ConvertsCorrectly) {
    auto result = callConvert({Value(0.0), Value("m"), Value("km")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(ConvertFunctionTest, NegativeValue_ConvertsCorrectly) {
    auto result = callConvert({Value(-1000.0), Value("m"), Value("km")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1.0, result.asNumber(), 1e-10);
}