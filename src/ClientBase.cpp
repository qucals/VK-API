#include "ClientBase.hpp"

namespace vk
{

ClientBase::ClientBase()
    : connectedToLongPoll_(false)
{}

void ClientBase::AddScope(const std::string scope)
{
    scope_.insert(scope);
}

void ClientBase::AddScope(const std::initializer_list<std::string> scopeList)
{
    for (const auto& scope : scopeList) {
        scope_.insert(scope);
    }
}

void ClientBase::ClearScope()
{
    scope_.clear();
}

std::string ClientBase::ConvertParametersDataToURL(const json& parametersData)
{
    std::string result;

    for (const auto& parameter : parametersData.items())
        result += parameter.key() + "=" + Utilities::ConvertStrToUrlCode(parameter.value().get<std::string>()) + "&";

    return result;
}

uint32_t ClientBase::GetRandomId()
{
    std::random_device rd;
    std::uniform_int_distribution<int32_t> dist(10000);
    return dist(rd);
}

VK_REQUEST_ERROR_TYPES ClientBase::GetRequestErrorType(const std::string& errorStr)
{
    using VK_ERROR = VK_REQUEST_ERROR_TYPES;

    // TODO: Add all VK_ERRORS
    if (errorStr == "invalid_request")
        return VK_ERROR::INVALID_REQUEST;
    else if (errorStr == "need_captcha")
        return VK_ERROR::NEED_CAPTCHA;
    else
        return VK_ERROR::UNKNOWN_ERROR;
}
}