/**
 * Describes the class for working with CURL.
 * @file Request.hpp
 * @author qucals
 * @version 0.0.5 18/08/21
 */

#pragma once

#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_

#include <curl/curl.h> // curl
#include <string> // string

#include <Defines.hpp>
#include <Utilities.hpp> // ConvertStrToUrlCode

namespace vk
{

namespace base
{

constexpr long APPLY_CURLOPT = 1L;
constexpr long NO_APPLY_CURLOPT = 0L;
constexpr long MAXREGIDS = 50L;
constexpr const char* USERAGENT = "VKAPI Client";

/**
 * @brief The class for working with request.
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
    __STATIC std::string Send(const std::string& url,
                              const std::string& postData);

protected:
    __STATIC std::size_t CurlWriteData(char* ptr, size_t size,
                                       size_t nmemb, std::string* data);
};

} // namespace base

} // namespace vk

#endif // _REQUEST_HPP_