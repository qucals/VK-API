cmake_minimum_required(VERSION 3.1)
project(VKAPI_TESTS LANGUAGES CXX)

set(VKAPI_MAJOR_VERSION "0")
set(VKAPI_MINOR_VERSION "0")
set(VKAPI_PATCH_VERSION "2")

set(MODULES 1 2)

foreach(MODULE ${MODULES})
    add_subdirectory(${MODULE})
endforeach()