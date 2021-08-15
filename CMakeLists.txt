cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME VKAPI)
project(${PROJECT_NAME} LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(VKAPI_MAJOR_VERSION "0")
set(VKAPI_MINOR_VERSION "0")
set(VKAPI_PATCH_VERSION "2")
set(VKAPI_VERSION_STRING "${VKAPI_MAJOR_VERSION}.${VKAPI_MINOR_VERSION}.${VKAPI_PATCH_VERSION}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

set(NLOHMANN_JSON_PATH "${PROJECT_SOURCE_DIR}/include")
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

option(BUILD_EXAMPLES "Determines whether examples should be performed" OFF)

file(GLOB TARGET_SRC "./src/*.cpp")
file(GLOB TARGET_HEADERS "./include/*.hpp")

# target_include_directories(${PROJECT_NAME} PUBLIC include)
add_library(${PROJECT_NAME} STATIC ${TARGET_SRC} ${TARGET_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INSTALL_INCLUDEDIR})

# Include json
target_link_directories(${PROJECT_NAME} PUBLIC ${NLOHMANN_JSON_PATH})

add_definitions(-DCURL_STATICLIB)

find_package(CURL REQUIRED)
target_link_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIR})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED on
)

configure_package_config_file(
    VKAPIConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/VKAPIConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VKAPI.cmake
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/VKAPIConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/)

install(TARGETS ${PROJECT_NAME} EXPORT VKAPITargets)
install(EXPORT VKAPITargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ NAMESPACE VKAPI::)

install(FILES ${TARGET_HEADERS} DESTINATION ${CMAKE_INSTALL_BINDIR}/include/vkapi)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()