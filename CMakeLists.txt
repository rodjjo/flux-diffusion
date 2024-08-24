cmake_minimum_required(VERSION 3.6.0 FATAL_ERROR)

project(FluxDiffusion C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake-modules)
set(CMAKE_CXX_STANDARD 17)
set(OPTION_BUILD_SHARED_LIBS OFF)

include_directories(
    FluxDiffusion
    ${PROJECT_SOURCE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    "${CMAKE_CURRENT_LIST_DIR}/dependencies/dfe-ui/src/include"
    "${CMAKE_CURRENT_LIST_DIR}/dependencies/pybind11/include"
)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/dependencies/pybind11")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/dependencies/dfe-ui")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src")


