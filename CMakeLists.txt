cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME VKAPI)
project(${PROJECT_NAME} LANGUAGES CXX)

set(VKAPI_MAJOR_VERSION "0")
set(VKAPI_MINOR_VERSION "0")
set(VKAPI_PATCH_VERSION "2")
set(VKAPI_VERSION_STRING "${VKAPI_MAJOR_VERSION}.${VKAPI_MINOR_VERSION}.${VKAPI_PATCH_VERSION}")

set(NLOHMANN_JSON_PATH "${PROJECT_SOURCE_DIR}/include")
set(CURL_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(CURL_LIB_PATH "${PROJECT_SOURCE_DIR}/thirdparty")

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

set(VKAPI_INCLUDE_DIRS bin/include CACHE PATH "The path to include's files")

option(BUILD_EXAMPLES "Determines whether examples should be performed" OFF)

file(GLOB TARGET_SRC "./src/*.cpp")
file(GLOB TARGET_HEADERS "./include/*.hpp")

add_library(${PROJECT_NAME} STATIC ${TARGET_SRC} ${TARGET_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC include)

# Include json
target_link_directories(${PROJECT_NAME} PUBLIC ${NLOHMANN_JSON_PATH})

# Include curl
target_link_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_PATH})

# TODO: Add automatic generation of libcurl
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC INTERFACE "${CURL_LIB_PATH}/libcurl.lib")
endif()

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED on
)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(
    TARGETS ${PROJECT_NAME} EXPORT VKAPITargets
    ARCHIVE DESTINATION lib
)

install(
    FILES
    ${TARGET_HEADERS} DESTINATION ${VKAPI_INCLUDE_DIRS}
)

install(
    EXPORT VKAPITargets
    FILE VKAPIConfig.cmake
    NAMESPACE VKAPI::
    DESTINATION lib/cmake
)