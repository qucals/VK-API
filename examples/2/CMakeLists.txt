cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME Example2)
project(${PROJECT_NAME} LANGUAGES CXX)

set(VKAPI_MAJOR_VERSION "0")
set(VKAPI_MINOR_VERSION "0")
set(VKAPI_PATCH_VERSION "2")

set(VKAPI_CONFIG_CMAKE "../../lib/cmake")
set(VKAPI_INCLUDE_DIRS "../../bin/include")

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED on
)

find_package(VKAPI CONFIG REQUIRED HINTS ${VKAPI_CONFIG_CMAKE})
target_link_libraries(${PROJECT_NAME} PRIVATE VKAPI::VKAPI)
target_include_directories(${PROJECT_NAME} PRIVATE ${VKAPI_INCLUDE_DIRS})