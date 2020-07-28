<p align="center">
  <a href="" rel="noopener">
</p>

<h3 align="center">VK API Library</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/qucals/VK_API.svg)](https://github.com/qucals/VK_API/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/qucals/VK_API.svg)](https://github.com/qucals/VK_API/pulls)
[![License](https://img.shields.io/github/license/qucals/VK_API)](/LICENSE)

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
- [TODO](../TODO.md)
- [Authors](#authors)

## 🧐 About <a name = "about"></a>

This is a library for easy work with the VK API. It contains all the standard queries that are defined by VK. This makes it easier to access LongPollServer VK.

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See [deployment](#deployment) for notes on how to deploy the project on a live system.

Steps:
1. Copy this repository to your computer;
2. Open terminal in a directory where you copied the repository;
3. Change a main directory: `cd build`;
4. Enter the following command with your replacements: `cmake -DNLOHMANN_JSON_PATH=PATH:*path* -DCURL_INCLUDE_PATH=PATH:*path* -DCURL_LIB_PATH=PATH:*path*..`. Replace the `*path*` inserts with the corresponding paths;
5. Build the project by the command: `cmake --build .`;
6. If installing is ended successfully you can see in the directory `build` include's files and libs;

### Prerequisites

The library using 2 additional dependencies:
1. [curlpp](https://www.curlpp.org/) - A library for working with requests;
2. [json](https://github.com/nlohmann/json) - A library for working with json.

Note that you also can install it by `vcpkg` on Windows.

### Installing

A step by step series of examples that tell you how to get a development env running.

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo.

## 🔧 Running the tests <a name = "tests"></a>

Explain how to run the automated tests for this system.

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## 🎈 Usage <a name="usage"></a>

Add notes about how to use the system.

## 🚀 Deployment <a name = "deployment"></a>

Add additional notes about how to deploy this on a live system.

## ⛏️ Built Using <a name = "built_using"></a>

- [CMake](https://cmake.org/) - Cross-platform family of tools designed to build

## ✍️ Authors <a name = "authors"></a>

- [@qucals](https://github.com/qucals) - Idea & Initial work