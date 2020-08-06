#pragma once

#ifndef INCLUDE_UTILITIES_HPP_
#define INCLUDE_UTILITIES_HPP_

#include <curl/curl.h> // curl
#include <string> // string

namespace vk {

namespace Utilities {

    inline std::string ConvertStrToUrlCode(const std::string& str)
    {
        std::string temp(str);
        CURL* curl = curl_easy_init();

        if (curl) {
            char* output = curl_easy_escape(curl, str.c_str(), str.length());

            if (output) {
                temp = output;
                curl_free(output);
            }
        }
        return temp;
    }

}

}

#endif // INCLUDE_UTILITIES_HPP_