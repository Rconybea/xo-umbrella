# xo_pyprocess/src/pyprocess/CMakeLists.txt

set(SELF_LIB xo_pyprocess)
set(SELF_SRCS pyprocess.cpp)

xo_pybind11_library(${SELF_LIB} ${PROJECT_NAME}Targets ${SELF_SRCS})

xo_pybind11_dependency(${SELF_LIB} process)
xo_pybind11_header_dependency(${SELF_LIB} xo_pyreactor)
xo_pybind11_header_dependency(${SELF_LIB} xo_pywebutil)
