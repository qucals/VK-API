cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME Example1)
project(${PROJECT_NAME} LANGUAGES CXX)

set(VKAPI_MAJOR_VERSION "0")
set(VKAPI_MINOR_VERSION "0")
set(VKAPI_PATCH_VERSION "2")

set(CMAKE_CXX_FLAGS "-lcurl")

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED on
)

set(VKAPI_DIR ../../lib/cmake)

find_package(VKAPI CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} VKAPI::VKAPI)