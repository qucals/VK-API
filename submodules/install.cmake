if((NOT NLOHMANN_JSON_INCLUDE_DIR) OR (NOT EXISTS ${NLOHMANN_JSON_INCLUDE_DIR}))    
    message("Unable to find nlohmann/json, cloning...")

    execute_process(COMMAND git submodule update --init -- submodules/json
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    set(NLOHMANN_JSON_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/json/
    CACHE PATH "nlohmann/json include directory")

    install(FILES ${NLOHMANN_JSON_SOURCE_DIR}/single_include/nlohmann/json.hpp DESTINATION ${CMAKE_INSTALL_BINDIR}/include/vkapi/nlohmann)

    # Setup a target
    add_library(nlohmann_json INTERFACE)
    target_include_directories(nlohmann_json INTERFACE
                               $<BUILD_INTERFACE:${NLOHMANN_JSON_INCLUDE_DIR}>
                               $<INSTALL_INTERFACE:include>)

    install(TARGETS nlohmann_json EXPORT nlohmann_json DESTINATION ${CMAKE_INSTALL_BINDIR}/include/vkapi/nlohmann)
    install(
        EXPORT nlohmann_json 
        FILE nlohmann_jsonTargets.cmake 
        # NAMESPACE nlohmann_json::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/
    )
endif()