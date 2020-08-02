cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME "VKAPI")
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

set(NLOHMANN_JSON_PATH "${PROJECT_SOURCE_DIR}/include")
set(CURL_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(CURL_LIB_PATH "${PROJECT_SOURCE_DIR}/thirdparty")

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
set(INSTALL_HEADERS_PATH bin/include)

file(GLOB TARGET_SRC "./src/*.cpp")
file(GLOB TARGET_HEADERS "./include/*.hpp")

add_library(vkapi STATIC ${TARGET_SRC} ${TARGET_HEADERS})

target_include_directories(vkapi PRIVATE include)

# Include json
target_link_directories(vkapi PRIVATE ${NLOHMANN_JSON_PATH})

# Include curl
target_link_directories(vkapi PRIVATE ${CURL_INCLUDE_PATH})

# TODO: Add automatic generation of libcurl
if(WIN32)
    target_link_libraries(vkapi INTERFACE "${CURL_LIB_PATH}/libcurl.lib")
endif()

set_target_properties(
    vkapi PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED on
)

install(
    TARGETS vkapi EXPORT vkapiTargets
    ARCHIVE DESTINATION lib
)

install(
    FILES
    ${TARGET_HEADERS} DESTINATION ${INSTALL_HEADERS_PATH}
)

install(
    EXPORT vkapiTargets
    FILE vkapiConfig.cmake
    NAMESPACE vkapi::
    DESTINATION lib/cmake
)