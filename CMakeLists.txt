cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME "VKAPI")
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

set(NLOHMANN_JSON_PATH "${PROJECT_SOURCE_DIR}/include")
set(CURL_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(CURL_LIB_PATH "${PROJECT_SOURCE_DIR}/thirdparty")

file(GLOB TARGET_SRC "./src/*.cpp")
file(GLOB TARGET_HEADERS "./include/*.hpp")

add_library(vkapi STATIC ${TARGET_SRC})

include_directories(${PROJECT_SOURCE_DIR}/include)

# Include json
target_link_directories(vkapi PUBLIC ${NLOHMANN_JSON_PATH})

# Include curl
target_link_directories(vkapi PUBLIC ${CURL_INCLUDE_PATH})

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
    TARGETS vkapi
    RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/bin"
    LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/lib"
    ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/lib/static"
)

install(
    FILES
    ${TARGET_HEADERS} DESTINATION "${PROJECT_SOURCE_DIR}/bin/include"
)
