# xo-observable/src/observable/CMakeLists.txt

set(SELF_LIB xo_observable)

set(SELF_SRCS init_observable.cpp)

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})

# ----------------------------------------------------------------
# Dependencies:
#
# REMINDER:  must coordinate with find_dependency() calls in
#            [xo-observable/cmake/xo_observableConfig.cmake.in]

xo_dependency(${SELF_LIB} reflect)
xo_dependency(${SELF_LIB} subsys)
#xo_external_target_dependency(${SELF_LIB} Eigen3 Eigen3::Eigen)
