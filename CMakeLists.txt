cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME "VKAPI")
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB TARGET_SRC "./src/*.cpp")

add_library(vkapi STATIC ${TARGET_SRC})
target_link_libraries(vkapi INTERFACE "${CMAKE_SOURCE_DIR}/lib/libcurl.lib")

set_target_properties(
    vkapi PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED on
)

install(
    TARGETS vkapi 
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION bin/static
)