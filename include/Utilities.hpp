/**
 * Contains additional functions for working with the library.
 * @file Utilities.hpp
 * @author qucals
 * @version 0.0.6 19/08/21
 */

#pragma once

#ifndef VKAPI_UTILITIES_HPP
#define VKAPI_UTILITIES_HPP

#include <Defines.hpp>

#include <string> // string
#include <curl/curl.h> // curl

#ifndef __CPLUSPLUS_OVER_11
#include <sstream>
#endif // __CPLUSPLUS_OVER_11

namespace vk
{

namespace utilities
{

std::string ConvertStrToUrlCode(const std::string& str);

std::string ToString(int val);
std::string ToString(unsigned val);
std::string ToString(long val);
std::string ToString(unsigned long val);
std::string ToString(long long val);
std::string ToString(unsigned long long val);
std::string ToString(float val);
std::string ToString(double val);
std::string ToString(long double val);

} // namespace utilities

} // namespace vk

#endif // VKAPI_UTILITIES_HPP
