cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME "VKAPI")
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

set(NLOHMANN_JSON_PATH ${PROJECT_SOURCE_DIR}/include)
set(CURL_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
set(CURL_LIB_PATH "${CMAKE_SOURCE_DIR}/thirdparty/")

file(GLOB TARGET_SRC "./src/*.cpp")

add_library(vkapi STATIC ${TARGET_SRC})
target_link_libraries(vkapi INTERFACE "${CURL_LIB_PATH}/libcurl.lib")

# Include json
target_link_directories(vkapi PUBLIC ${NLOHMANN_JSON_PATH})

# Include curl
target_link_directories(vkapi PUBLIC ${CURL_INCLUDE_PATH})

target_link_directories(vkapi PUBLIC ${PROJECT_SOURCE_DIR}/include)

set_target_properties(
    vkapi PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED on
)

install(
    TARGETS vkapi 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    INCLUDES DESTINATION bin/include
)