# Velox Formulas Library
# High-performance Excel-like formula parsing and evaluation engine

# Create the formulas library
add_library(velox-formulas
    core/api.cpp
    core/context.cpp
    core/types.cpp
    engine/evaluator.cpp
    engine/formula_engine.cpp
    parser/ast.cpp
    parser/lexer.cpp
    parser/parser.cpp
    functions/fn_dispatcher.cpp
    functions/nonstd.cpp
    functions/utils/conditional_utils.cpp
    functions/utils/validation.cpp
    functions/math/abs.cpp
    functions/math/acos.cpp
    functions/math/asin.cpp
    functions/math/atan.cpp
    functions/math/atan2.cpp
    functions/math/average.cpp
    functions/math/averageif.cpp
    functions/math/averageifs.cpp
    functions/math/ceiling.cpp
    functions/math/combin.cpp
    functions/math/correl.cpp
    functions/math/cos.cpp
    functions/math/cosh.cpp
    functions/math/count.cpp
    functions/math/countif.cpp
    functions/math/degrees.cpp
    functions/math/even.cpp
    functions/math/exp.cpp
    functions/math/fact.cpp
    functions/math/floor.cpp
    functions/math/gcd.cpp
    functions/math/int.cpp
    functions/math/lcm.cpp
    functions/math/ln.cpp
    functions/math/log.cpp
    functions/math/log10.cpp
    functions/math/max.cpp
    functions/math/median.cpp
    functions/math/min.cpp
    functions/math/mod.cpp
    functions/math/mode.cpp
    functions/math/mround.cpp
    functions/math/odd.cpp
    functions/math/permut.cpp
    functions/math/pi.cpp
    functions/math/power.cpp
    functions/math/quotient.cpp
    functions/math/radians.cpp
    functions/math/rand.cpp
    functions/math/randbetween.cpp
    functions/math/round.cpp
    functions/math/rounddown.cpp
    functions/math/roundup.cpp
    functions/math/sign.cpp
    functions/math/sin.cpp
    functions/math/sinh.cpp
    functions/math/sqrt.cpp
    functions/math/stdev.cpp
    functions/math/sum.cpp
    functions/math/sumif.cpp
    functions/math/sumifs.cpp
    functions/math/sumproduct.cpp
    functions/math/sumsq.cpp
    functions/math/sumx_variants.cpp
    functions/math/tan.cpp
    functions/math/tanh.cpp
    functions/math/trunc.cpp
    functions/math/var.cpp
    functions/text/char.cpp
    functions/text/clean.cpp
    functions/text/code.cpp
    functions/text/concatenate.cpp
    functions/text/exact.cpp
    functions/text/find.cpp
    functions/text/left.cpp
    functions/text/len.cpp
    functions/text/lower.cpp
    functions/text/mid.cpp
    functions/text/proper.cpp
    functions/text/replace.cpp
    functions/text/right.cpp
    functions/text/roman_arabic.cpp
    functions/text/rpt.cpp
    functions/text/search.cpp
    functions/text/substitute.cpp
    functions/text/t.cpp
    functions/text/text.cpp
    functions/text/textjoin.cpp
    functions/text/trim.cpp
    functions/text/unichar.cpp
    functions/text/unicode.cpp
    functions/text/upper.cpp
    functions/text/value.cpp
    functions/logical/and.cpp
    functions/logical/false.cpp
    functions/logical/if.cpp
    functions/logical/iferror.cpp
    functions/logical/ifna.cpp
    functions/logical/ifs.cpp
    functions/logical/isblank.cpp
    functions/logical/iserror.cpp
    functions/logical/isnumber.cpp
    functions/logical/istext.cpp
    functions/logical/not.cpp
    functions/logical/or.cpp
    functions/logical/switch.cpp
    functions/logical/true.cpp
    functions/logical/xor.cpp
    functions/datetime/date.cpp
    functions/datetime/datedif.cpp
    functions/datetime/day.cpp
    functions/datetime/hour.cpp
    functions/datetime/minute.cpp
    functions/datetime/month.cpp
    functions/datetime/now.cpp
    functions/datetime/second.cpp
    functions/datetime/time.cpp
    functions/datetime/today.cpp
    functions/datetime/weekday.cpp
    functions/datetime/year.cpp
    functions/financial/fv.cpp
    functions/financial/irr.cpp
    functions/financial/mirr.cpp
    functions/financial/nper.cpp
    functions/financial/npv.cpp
    functions/financial/pmt.cpp
    functions/financial/pv.cpp
    functions/financial/rate.cpp
    functions/engineering/bin2dec.cpp
    functions/engineering/bin2oct.cpp
    functions/engineering/bitand.cpp
    functions/engineering/bitor.cpp
    functions/engineering/bitxor.cpp
    functions/engineering/complex.cpp
    functions/engineering/convert.cpp
    functions/engineering/dec2bin.cpp
    functions/engineering/dec2hex.cpp
    functions/engineering/dec2oct.cpp
    functions/engineering/hex2dec.cpp
    functions/engineering/hex2oct.cpp
    functions/engineering/imaginary.cpp
    functions/engineering/imreal.cpp
    functions/engineering/oct2bin.cpp
    functions/engineering/oct2hex.cpp
    functions/lookup/choose.cpp
    functions/lookup/row_column.cpp
)

# Set include directories
target_include_directories(velox-formulas
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set compile definitions
target_compile_definitions(velox-formulas
    PRIVATE
        VELOX_FORMULAS_EXPORTS
)

# Set C++ standard
target_compile_features(velox-formulas
    PUBLIC
        cxx_std_17
)

# Set properties
set_target_properties(velox-formulas PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME "VeloxFormulas"
)

# Install rules
install(TARGETS velox-formulas
    EXPORT VeloxFormulasTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/velox/formulas/
    DESTINATION include/velox/formulas
    FILES_MATCHING PATTERN "*.h"
)

# Export targets
install(EXPORT VeloxFormulasTargets
    FILE VeloxFormulasTargets.cmake
    NAMESPACE Velox::
    DESTINATION lib/cmake/VeloxFormulas
)
