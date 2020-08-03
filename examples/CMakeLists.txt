cmake_minimum_required(VERSION 3.1)
project(VKAPI_TESTS VERSION 0.0.1 LANGUAGES CXX)

set(MODULES 1 2)

foreach(MODULE ${MODULES})
    add_subdirectory(${MODULE})
endforeach()