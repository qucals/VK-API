/**
 * Describes the class for working with CURL.
 * @file Request.hpp
 * @author qucals
 * @version 0.0.8 24/08/21
 */

#ifndef VKAPI_REQUEST_HPP
#define VKAPI_REQUEST_HPP

#include "Defines.hpp"
#include "Utilities.hpp" // ConvertStrToUrlCode

#include <curl/curl.h> // curl
#include <string> // string

namespace vk
{

namespace base
{

_VKAPI_UNUSED_FOR_ANALYZER _VKAPI_CONSTEXPR long APPLY_CURLOPT = 1L;
_VKAPI_CONSTEXPR long NO_APPLY_CURLOPT = 0L;
_VKAPI_CONSTEXPR long MAXREGIDS = 50L;
_VKAPI_CONSTEXPR const char* USERAGENT = "VKAPI Client";

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
     *
     * @param url: the request in url format.
     * @param postData: the additional parameters which need to add to the request.
     *
     * @retval an answer in a string.
     */
    _VKAPI_STATIC std::string Send(const std::string& url,
                                   const std::string& postData);

protected:
    _VKAPI_STATIC std::size_t CurlWriteData(char* ptr, size_t size,
                                            size_t nmemb, std::string* data);
};

} // namespace base

} // namespace vk

#endif // VKAPI_REQUEST_HPP
