/**
 * Describes the class for working with CURL.
 * @file Request.hpp
 * @author qucals
 * @version 0.0.3 15/08/21
 */

#include <Request.hpp>

namespace vk
{

namespace base
{

std::string Request::Send(const std::string& url, const std::string& postData)
{
    CURL* curl = curl_easy_init();

    if (curl) {
        std::string errorBuf;
        std::string callbackBuf;

        errorBuf.resize(CURL_ERROR_SIZE);

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuf.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, NO_APPLY_CURLOPT);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, NO_APPLY_CURLOPT);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, MAXREGIDS);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Request::CurlWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callbackBuf);

        if (!postData.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        }

        CURLcode result = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if (result == CURLE_OK) {
            return callbackBuf;
        } else {
            return errorBuf;
        }
    }

    return postData;
}

std::size_t Request::CurlWriteData(char* ptr, size_t size,
                                   size_t nmemb, std::string* data)
{
    if (data) {
        data->append(ptr, size * nmemb);
        return size * nmemb;
    }

    return 0;
}

} // namespace base

} // namespace vk