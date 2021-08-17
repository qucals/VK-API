/**
 * Contains additional functions for working with the library.
 * @file Utilities.hpp
 * @author qucals
 * @version 0.0.5 18/08/21
 */

#pragma once

#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_

#include <Defines.hpp>

#include <curl/curl.h> // curl
#include <string> // string

namespace vk
{

namespace utilities
{

__INLINE std::string ConvertStrToUrlCode(const std::string& str)
{
    std::string temp(str);
    CURL* curl = curl_easy_init();

    if (curl) {
        char* output = curl_easy_escape(curl, str.c_str(), static_cast<int>(str.length()));

        if (output) {
            temp = output;
            curl_free(output);
        }
    }
    return temp;
}

}

}

#endif // _UTILITIES_HPP_