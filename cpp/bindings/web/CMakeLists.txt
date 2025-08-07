# WebAssembly bindings for XL-Formula

# Create the web bindings library
add_executable(xl-formula-web web_bindings.cpp)

# Link with the core library
target_link_libraries(xl-formula-web xl-formula)

# Emscripten-specific settings
set_target_properties(xl-formula-web PROPERTIES
    OUTPUT_NAME "xl-formula"
    SUFFIX ".js"
)

# Set Emscripten compiler and linker flags
target_link_options(xl-formula-web PRIVATE
    "SHELL:-s MODULARIZE=1"
    "SHELL:-s EXPORT_NAME='XLFormula'"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s INITIAL_MEMORY=16MB"
    "SHELL:-s MAXIMUM_MEMORY=32MB"
    "SHELL:-s ENVIRONMENT=web"
    "SHELL:-s SINGLE_FILE=0"
    "SHELL:-s EXPORT_ES6=1"
    "SHELL:-s WASM=1"
    "SHELL:-s NO_EXIT_RUNTIME=1"
    "SHELL:-s ASSERTIONS=0"
    "SHELL:-s SAFE_HEAP=0"
    "SHELL:-s STACK_OVERFLOW_CHECK=0"
    "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
    "SHELL:--bind"
)

# Set the output directory to the packages/web folder
set_target_properties(xl-formula-web PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/packages/web"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/packages/web"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/packages/web"
)

# Copy outputs to the web package directory and docs public directory
add_custom_command(TARGET xl-formula-web POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:xl-formula-web>/xl-formula.wasm"
        "${CMAKE_SOURCE_DIR}/packages/web/xl-formula.wasm"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:xl-formula-web>/xl-formula.wasm"
        "${CMAKE_SOURCE_DIR}/apps/docs/public/xl-formula.wasm"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:xl-formula-web>"
        "${CMAKE_SOURCE_DIR}/apps/docs/public/xl-formula.js"
    COMMENT "Copying WASM and JS files to web package and docs public directories"
)