cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME Example1)
project(${PROJECT_NAME} LANGUAGES CXX)

if(UNIX)
    set(CMAKE_CXX_FLAGS "-L/usr/local/lib/ -L/usr/lib/ ${CMAKE_CXX_FLAGS} -pthread")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl")

set(VKAPI_DIR ../../lib/cmake)
find_package(VKAPI CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} VKAPI::VKAPI)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED on
)