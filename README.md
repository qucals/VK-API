# Introduction
    VK API Library is used for easy working with API VK by C++.
    It has standard set of functions to send any requests to the Long Poll Server as a bot, so is a client.

# Set up
## Windows
### Building
    1. Download this repository;
    2. Regenerate building files of the project from `build` directory by:
        `cd build && cmake ..`
    3. Build the project by:
        `cmake --build .`.
    4. Then in `bin` you can see the include's files and the lib of vkapi.

    Note: 
        You don't need link any dependencies to the project. But if you want you have any parameters:
        1. `DNLOHMANN_JSON_PATH` - The path to a hpp file of nlohmann_json. (https://github.com/nlohmann/json)
        2. `DCURL_INCLUDE_PATH` - The path to hpp files of curlpp. (https://www.curlpp.org/)
        3. `DCURL_LIB_PATH` - The path to lib file of curlpp. (https://www.curlpp.org/)

        Note that ###curlpp### and ###nlohmann_json### are allowed in ###vcpkg###.

### Linking
    You can link this library by cmake to your project. Steps:
    1. Download this repository.
    2. In your cmake file add the following lines:
        `
        find_package(*There is the path to this repository*)
        
        if(NOT VKAPI)
            message(ERROR "VKAPI has not been found!")
        else()
            target_link_directories(*name_your_project* ${VKAPI_INCLUDE_DIRS})
            target_link_libraries(*name_your_project* ${VKAPI_LIBRARIES})
        endif()
        `

## UNIX
### Building
### Linking

# Using