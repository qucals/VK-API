<p align="center">
  <a href="" rel="noopener">
</p>

<h3 align="center">VK API Library</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/qucals/VK-API.svg)](https://github.com/qucals/VK-API/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/qucals/VK-API.svg)](https://github.com/qucals/VK-API/pulls)
[![License](https://img.shields.io/github/license/qucals/VK-API)](/LICENSE)

</div>

---

<p align="center"> The easiest C++ library to use for working with the VK API.
    <br> 
</p>

## 📝 Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Deployment](#deployment)
- [Usage](#usage)
- [Built Using](#built_using)
- [Authors](#authors)
  
## 🧐 About <a name = "about"></a>

This is a library for easy work with the VK API. It contains all the standard queries that are defined by VK. This makes it easier to access LongPollServer VK.

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See [deployment](#deployment) for notes on how to deploy the project on a live system.

Steps <a name = "steps"></a>:
1. Copy this repository to your computer;
2. Open terminal in a directory where you copied the repository;
3. Change a main directory: `cd build`;
4. Enter the following command with your replacements: `cmake -DNLOHMANN_JSON_PATH=PATH:*path* -DCURL_INCLUDE_PATH=PATH:*path* -DCURL_LIB_PATH=PATH:*path* ..`. Replace the `*path*` inserts with the corresponding paths;
5. Build the project by the next command: `cmake --build .`.

### Prerequisites

The library using 2 additional dependencies:
1. [curlpp](https://www.curlpp.org/) - A library for working with requests;
2. [json](https://github.com/nlohmann/json) - A library for working with json.

Note that you also can install it by `vcpkg` on Windows.

### Installing <a name = "installing"></a>

For connection this library to yours you need to follow the next steps:
1. Copy this repository to your computer;
2. Open terminal in a directory where you copied the repository;
3. Change a main directory: `cd build`;
4. Enter the following command with your replacements: `cmake -DNLOHMANN_JSON_PATH=PATH:*path* -DCURL_INCLUDE_PATH=PATH:*path* -DCURL_LIB_PATH=PATH:*path* ..`. Replace the `*path*` inserts with the corresponding paths;
5. Build and install the project by the next command: `cmake --build . --target install --config Release`;
6. If building and install is ended successfully you can see there are `include` and `lib`'s directories in `install`'s directory.

After the above steps you will have a static library and include's files that you can connection to your project.

## 🎈 Usage <a name="usage"></a>

There are two base examples.

Authorization for Bot:

```CXX
#include "Bot.hpp"

int main(int argc, const char** argv)
{
    std::string access_token = "your_token_there";
    std::string group_id = "your_group_id_there";

    VK::Bot bot(group_id);

    if (bot.Auth(access_token) == true) {
        // The further behaviour
    } else {
        // The further behaviour
    }

    return 0;
}
```

Authorization for User:

```CXX
#include "User.hpp"

int main(int argc, const char** argv)
{
    std::string app_id = "your_app_id_there";
    std::string app_secure_key = "your_app_secure_key_there";
    std::string login = "your_login_there";
    std::string password = "your_password_there";
    std::string access_token = "your_access_token_there";

    VK::User user(app_id, app_secure_key);
    
    if(user.Auth(login, password) == true) {
        // The further behaviour
    } else {
        // The further behaviour
    }

    // or

    if (user.Auth(access_token) == true) {
        // The further behaviour
    } else {
        // The further behaviour
    }

    return 0;
}
```

There are other examples of using this library in the `examples` directory.

## 🚀 Deployment <a name = "deployment"></a>

### CMake

If you use CMake you make use the `find_package` instruction.
After [installing](#installing) the project you can find vkapiConfig.cmake in the `lib/cmake` directory. For connection this library to your project you need to add the following code to your cmake file:
```CMake
    find_package(VKAPI CONFIG REQUIRED HINTS *path_to_directory_lib_cmake*)
    target_link_libraries(*your_target* PRIVATE VKAPI::VKAPI)
    target_include_directories(*your_target* PRIVATE *path_to_directory_bin_include*)
```
Replace: 
1. `*path_to_directory_lib_cmake*` to the path to directory `lib/cmake`;
2. `*your_target*` to your target;
3. `*path_to_directory_bin_include*`  to the path to directory `bin/include`;

### Others

If you use another building system you need to connection include's files and lib file which is located `bin\include` and `lib` directories.

## ⛏️ Built Using <a name = "built_using"></a>

- [CMake](https://cmake.org/) - Cross-platform family of tools designed to build

## ✍️ Authors <a name = "authors"></a>

- [@qucals](https://github.com/qucals) - Idea & Initial work