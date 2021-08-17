/**
 * Contains general objects for working with VK API.
 * @file ClientBase.hpp
 * @author qucals
 * @version 0.0.5 18/08/21
 */

#include <ClientBase.hpp>

namespace vk
{

namespace base
{

ClientBase::ClientBase()
    : m_connectedToLongPoll(false)
{}

void ClientBase::AddScope(std::string scope)
{ m_scope.insert(__MOVE(scope)); }

void ClientBase::AddScope(std::initializer_list<std::string> scopeList)
{
#ifdef __CPLUSPLUS_OVER_11
    for (const auto& scope : scopeList) { m_scope.insert(__MOVE(scope)); }
#else
    for (std::initializer_list<std::string>::iterator iter = scopeList.begin();
         iter != scopeList.end();
         iter++) {
        m_scope.insert(__MOVE(*iter));
    }
#endif // __CPLUSPLUS_OVER_11
}

void ClientBase::ClearScope()
{ m_scope.clear(); }

std::string ClientBase::ConvertParametersDataToURL(const json& parametersData)
{
    std::string result;

#ifdef __CPLUSPLUS_OVER_11
    for (const auto& parameter : parametersData.items()) {
        result += parameter.key() + "=" +
                  utilities::ConvertStrToUrlCode(parameter.value().get<std::string>()) + "&";
    }
#else
    for (json::iterator iter = parametersData.begin();
         iter != parametersData.end();
         iter++) {
        result += iter->key() + "=" +
            utilities::ConvertStrToUrlCode(iter->value().get<std::string>()) + "&";
    }
#endif // __CPLUSPLUS_OVER_11

    return result;
}

uint32_t ClientBase::GetRandomId()
{
#ifdef __CPLUSPLUS_OVER_11
    std::random_device rd;
    std::uniform_int_distribution<int32_t> dist(10000);
    return dist(rd);
#else
    uint32_t number = 999999 + (long int) ((double) rand() / (double) RAND_MAX * (LONG_MAX - 999999 + 1));
    return number;
#endif // __CPLUSPLUS_OVER_11
}

VK_REQUEST_ERROR_TYPES ClientBase::GetRequestErrorType(const std::string& errorStr)
{
    using VK_ERROR = VK_REQUEST_ERROR_TYPES;

    // TODO: Add all VK_ERRORS
    if (errorStr == VKAPI_INVALID_REQUEST) {
        return VK_ERROR::INVALID_REQUEST;
    } else if (errorStr == VKAPI_NEED_CAPTCHA) {
        return VK_ERROR::NEED_CAPTCHA;
    } else {
        return VK_ERROR::UNKNOWN_ERROR;
    }
}

} // namespace base

} // namespace vk