# Examples
add_executable(basic_example basic_example.cpp)
target_link_libraries(basic_example xl-formula)
target_include_directories(basic_example PRIVATE ${CMAKE_SOURCE_DIR}/cpp/include)

add_executable(advanced_example advanced_example.cpp)
target_link_libraries(advanced_example xl-formula)
target_include_directories(advanced_example PRIVATE ${CMAKE_SOURCE_DIR}/cpp/include)