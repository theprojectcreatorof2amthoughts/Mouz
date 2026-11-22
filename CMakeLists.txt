cmake_minimum_required(VERSION 3.22)
project(Mouz VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mouz_core src/lexer.cpp src/parser.cpp src/expression.cpp)
target_include_directories(mouz_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(mouz_runtime runtime/src/runtime.c)
target_include_directories(mouz_runtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/runtime/include)

add_executable(mouz compiler/main.cpp)
target_link_libraries(mouz PRIVATE mouz_core)
add_executable(mouzi interpreter/main.cpp)
target_link_libraries(mouzi PRIVATE mouz_runtime)

if(MSVC)
  target_compile_options(mouz_core PRIVATE /W4 /permissive-)
  target_compile_options(mouz PRIVATE /W4 /permissive-)
  target_compile_options(mouzi PRIVATE /W4 /permissive-)
else()
  target_compile_options(mouz_core PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_options(mouz PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_options(mouzi PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME mouz_check_example COMMAND mouz check ${CMAKE_CURRENT_SOURCE_DIR}/examples/first.mozf)
add_test(NAME mouz_expression_arithmetic COMMAND mouz eval "(12 + 8) * 3")
add_test(NAME mouz_expression_boolean COMMAND mouz eval "(9 >= 9) && true")
add_test(NAME mouz_expression_string COMMAND mouz eval "\"Hello, \" + \"Mouz\"")

add_test(NAME mouz_state_example COMMAND mouz check ${CMAKE_CURRENT_SOURCE_DIR}/examples/state-values.mozf)
