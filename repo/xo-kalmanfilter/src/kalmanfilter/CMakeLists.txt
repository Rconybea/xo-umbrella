# xo-kalmanfilter/src/kalmanfilter/CMakeLists.txt

set(SELF_LIB xo_kalmanfilter)

set(SELF_SRCS
    EigenUtil.cpp
    KalmanFilterInput.cpp
    KalmanFilterInputToConsole.cpp
    KalmanFilterState.cpp
    KalmanFilterStateToConsole.cpp
    KalmanFilterTransition.cpp
    KalmanFilterObservable.cpp
    KalmanFilterEngine.cpp
    KalmanFilter.cpp
    KalmanFilterStep.cpp
    KalmanFilterSpec.cpp
    KalmanFilterSvc.cpp
    init_filter.cpp
)

xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})

# ----------------------------------------------------------------
# Dependencies
#
# REMINDER:  must coordinate with find_dependency() calls in
#            [xo-kalmanfilter/cmake/xo_kalmanfilterConfig.cmake.in]

xo_dependency(${SELF_LIB} reactor)
xo_external_target_dependency(${SELF_LIB} Eigen3 Eigen3::Eigen)
