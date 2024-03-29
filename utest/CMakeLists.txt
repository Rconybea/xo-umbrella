# build unittest observable/utest

set(SELF_EXECUTABLE_NAME utest.observable)
set(SELF_SOURCE_FILES
    observable_utest_main.cpp
    linear_interpolation.test.cpp dimension.test.cpp unit.test.cpp quantity.test.cpp)

add_executable(${SELF_EXECUTABLE_NAME} ${SELF_SOURCE_FILES})
xo_include_options2(${SELF_EXECUTABLE_NAME})

add_test(NAME ${SELF_EXECUTABLE_NAME} COMMAND ${SELF_EXECUTABLE_NAME})
target_code_coverage(${SELF_EXECUTABLE_NAME} AUTO ALL)

# ----------------------------------------------------------------
# internal dependencies: logutil, ...

xo_self_dependency(${SELF_EXECUTABLE_NAME} xo_observable)
xo_dependency(${SELF_EXECUTABLE_NAME} randomgen)

# ----------------------------------------------------------------
# 3rd party dependency: catch2:

xo_external_target_dependency(${SELF_EXECUTABLE_NAME} Catch2 Catch2::Catch2)

# shouldn't need this anymore
#
## ----------------------------------------------------------------
## make standard directories for std:: includes explicit
## so that
## (1) they appear in compile_commands.json.
## (2) clangd (run from emacs lsp-mode) can find them
##
#if(CMAKE_EXPORT_COMPILE_COMMANDS)
#  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
#      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
#endif()

# end CMakeLists.txt
