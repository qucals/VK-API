#pragma once

#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_

#include <curl/curl.h> // curl
#include <string> // string

#include "Utilities.hpp" // ConvertStrToUrlCode

namespace vk
{

constexpr auto APPLY_CURLOPT = 1L;
constexpr auto NO_APPLY_CURLOPT = 0L;
constexpr auto MAXREGIDS = 50L;
constexpr auto USERAGENT = "VK API Client";

/**
 * @brief The class for working with requst.
 */
class Request
{
public:
    Request() = delete;
    ~Request() = delete;

    Request& operator=(const Request&) = delete;

    /**
     * @brief Sending your request to the VK server.
     * @param  url: the request in url format.
     * @param  postData: the additional parameters which need to add to the request.
     * @retval an answer in a string.
     */
    static std::string Send(const std::string& url,
        const std::string& postData);

protected:
    static std::size_t CurlWriteData(char* ptr, size_t size,
        size_t nmemb, std::string* data);
};

}

#endif // _REQUEST_HPP_