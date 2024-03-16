# xo-submodule2/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(xo-submodule2 VERSION 1.0)

# ----------------------------------------------------------------
# global build settings

# Adopting submodule builds directly into this cmake.
# Submodule builds will pickup dependent xo artifacts directly
# from sibling build dirs.
# (Contrast with a build that relies on install step).
# In particular,  configure step in satellite projects
# needs to avoid using cmake find_package() on sibling xo projects:
# 1. .cmake support files
#      fooConfig.cmake
#      fooConfigVersion.cmake
#      fooTargets.cmake
#    won't have been installed
# 2. In any case,  they point to final install location;
#    we need build location
#
set(XO_SUBMODULE_BUILD True)

# toplevel source directory;  used only with XO_SUBMODULE_BUILD
set(XO_UMBRELLA_SOURCE_DIR ${CMAKE_SOURCE_DIR})
# toplevel binary directory;  used only with XO_SUBMODULE_BUILD
set(XO_UMBRELLA_BINARY_DIR ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/repo/xo-cmake/cmake)

# ----------------------------------------------------------------
# unit test setup

enable_testing()
# enable code coverage for all executables+libraries
# (when configured with -DCODE_COVERAGE=ON)
#
#add_code_coverage()
#add_code_coverage_all_targets(
#  EXCLUDE
#  /nix/store/*
#  ${PROJECT_SOURCE_DIR}/utest/*)

# ----------------------------------------------------------------
# global c++ settings.

enable_language(CXX)

# temporary compiler flags here
set(PROJECT_CXX_FLAGS "")
add_definitions(${PROJECT_CXX_FLAGS})

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(NOT CMAKE_INSTALL_RPATH)
    set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib CACHE STRING "runpath for installed libraries/executables")
endif()

message("-- CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
message("-- CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message("-- CMAKE_INSTALL_RPATH=${CMAKE_INSTALL_RPATH}")

# ----------------------------------------------------------------
# xo satellite projects

add_subdirectory(repo/xo-cmake)
add_subdirectory(repo/xo-indentlog)
#add_subdirectory(repo/subsys)
#add_subdirectory(repo/refcnt)
#add_subdirectory(repo/reflect)
#add_subdirectory(repo/randomgen)
#add_subdirectory(repo/xo-ordinaltree)
#add_subdirectory(repo/xo-pyutil)
#add_subdirectory(repo/xo-pyreflect)
#add_subdirectory(repo/xo-printjson)
#add_subdirectory(repo/xo-pyprintjson)
#add_subdirectory(repo/xo-callback)
#add_subdirectory(repo/xo-webutil)
#add_subdirectory(repo/xo-pywebutil)
#add_subdirectory(repo/xo-reactor)
#add_subdirectory(repo/xo-pyreactor)
#add_subdirectory(repo/xo-simulator)
#add_subdirectory(repo/xo-process)
#add_subdirectory(repo/xo-pyprocess)
#add_subdirectory(repo/xo-websock)
#add_subdirectory(repo/xo-pywebsock)
#add_subdirectory(repo/xo-statistics)
#add_subdirectory(repo/xo-kalmanfilter)
#add_subdirectory(repo/xo-pykalmanfilter)
#add_subdirectory(repo/xo-distribution)
#add_subdirectory(repo/xo-pydistribution)

# ----------------------------------------------------------------
