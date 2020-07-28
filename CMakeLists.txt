cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME "VKAPI")
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug")

set(NLOHMANN_JSON_PATH ${CMAKE_PREFIX_PATH}/include)
set(CURL_INCLUDE_PATH ${CMAKE_PREFIX_PATH}/include)
set(CURL_LIB_PATH "${CMAKE_PREFIX_PATH}/thirdparty")

file(GLOB TARGET_SRC "./src/*.cpp")

add_library(vkapi STATIC ${TARGET_SRC})

# Include json
target_link_directories(vkapi PUBLIC ${NLOHMANN_JSON_PATH})

# Include curl
target_link_directories(vkapi PUBLIC ${CURL_INCLUDE_PATH})

target_link_directories(vkapi PUBLIC ${CMAKE_PREFIX_PATH}/include)

target_link_libraries(vkapi INTERFACE "${CURL_LIB_PATH}/libcurl.lib")

set_target_properties(
    vkapi PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED on
)

install(
    TARGETS vkapi
    CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
    RUNTIME DESTINATION bin/${CMAKE_CONFIGURATION_TYPES}
    LIBRARY DESTINATION lib/${CMAKE_CONFIGURATION_TYPES}
    ARCHIVE DESTINATION lib/${CMAKE_CONFIGURATION_TYPES}/static
    INCLUDES DESTINATION bin/${CMAKE_CONFIGURATION_TYPES}/include
)