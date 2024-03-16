# optionutil/CMakeLists.txt

set(SELF_LIB optionutil)
set(SELF_SRCS Side.cpp Size.cpp
    Price.cpp PxSize.cpp PxSize2.cpp
    Callput.cpp Size2.cpp Px2.cpp
    PxSizeTime.cpp PxSizeTime2.cpp
    Ivol.cpp Ivol2.cpp
    init_optionutil.cpp)

# build shared library 'option_util'
xo_add_shared_library4(${SELF_LIB} ${PROJECT_NAME}Targets ${PROJECT_VERSION} 1 ${SELF_SRCS})

# ----------------------------------------------------------------
# internal dependencies

xo_dependency(${SELF_LIB} printjson)

# ----------------------------------------------------------------
# external dependencies

#xo_external_target_dependency(${SELF_LIB} Eigen3 Eigen3::Eigen)

# end CMakeLists.txt
