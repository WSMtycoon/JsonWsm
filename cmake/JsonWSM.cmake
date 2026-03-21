# cmake/JsonWSM.cmake

cmake_minimum_required(VERSION 3.20)

set(MODULE_JSON_DIR "${CMAKE_CURRENT_LIST_DIR}/../Module")
set(MODULE_SOURCE_DIR "${MODULE_JSON_DIR}/Source")

file(GLOB JSON_MODULE_HEADERS "${MODULE_JSON_DIR}"/*.h)
file(GLOB JSON_MODULE_HEADERS "${MODULE_SOURCE_DIR}"/*.cpp)

if(NOT JSON_MODULE_HEADERS AND NOT JSON_MODULE_HEADERS)
    message(FATAL_ERROR "JsonWSM module: no source or header files found!")
endif()

# IMPORTANT:
#list(APPEND PROJECT_HEADER ${JSON_MODULE_HEADERS})
#list(APPEND PROJECT_SOURCE ${JSON_MODULE_SOURCES})

# Use include_directories() (compatible with CMake 3.20 and your style)
include_directories(${MODULE_JSON_DIR})