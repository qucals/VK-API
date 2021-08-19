/**
 * Contains additional functions for working with the library.
 * @file Utilities.hpp
 * @author qucals
 * @version 0.0.6 19/08/21
 */

#include <Utilities.hpp>

namespace vk
{

namespace utilities
{

std::string ConvertStrToUrlCode(const std::string& str)
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

std::string ToString(int val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

std::string ToString(unsigned val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

std::string ToString(long val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

std::string ToString(unsigned long val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

std::string ToString(long long val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

std::string ToString(unsigned long long val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

std::string ToString(float val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

std::string ToString(double val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

std::string ToString(long double val)
{
#ifdef __CPLUSPLUS_OVER_11
    return std::to_string(val);
#else
    std::ostringstream ostr;
    ostr << val;
    std::string str = ostr.str();
    return str;
#endif
}

} // namespace utilities

} // namespace vk
