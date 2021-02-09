#pragma once

#ifndef _CLIENTBASE_HPP_
#define _CLIENTBASE_HPP_

#include <iostream> // cout, endl
#include <random> // rand
#include <set> // set
#include <string> // string

#include "Request.hpp" // Request
#include "Utilities.hpp" // ConvertStrToUrlCode
#include "Exceptions.hpp" // already_connected, not_connected, empty_argument
#include "Defines.hpp" // ABSTRACT

#include "nlohmann/json.hpp" // json

namespace vk
{

using json = nlohmann::json;

constexpr auto API_URL = "https://api.vk.com/method/";
constexpr auto AUTH_URL = "https://oauth.vk.com/token?";
constexpr auto OAUTH_URL = "https://oauth.vk.com/authorize?";
constexpr auto OAUTH_URL_SECOND = "https://oauth.vk.com/access_token?";
constexpr auto API_VERSION = "5.120";

// The types of error which the VK server can return.
// You can see the description of these on https://vk.com/dev/errors
enum class VK_REQUEST_ERROR_TYPES : uint16_t
{
    UNKNOWN_ERROR = 1,
    APP_IS_DISABLED = 2,
    UNKNOWN_METHOD = 3,
    INVALID_SIGNATURE = 4,
    AUTHORIZATION_FAILED = 5,
    TOO_MANY_REQUESTS = 6,
    NO_PERMISSIONS = 7,
    INVALID_REQUEST = 8,
    TOO_MANY_SIMILAR_ACTIONS = 9,
    INTERNAL_SERVER_ERROR = 10,
    TEST_MODE = 11,
    NEED_CAPTCHA = 14,
    ACCESS_DENIED = 15,
    NEED_HTTPS = 16,
    USER_VALIDATION_REQUIRED = 17,
    PAGE_DELETE_OR_BLOCKED = 18,
    ACTION_PROHIBITED_FOR_NON_STANDALONE_APP = 20,
    ACTION_FOR_ONLY_STANDOLE_OR_OPEN_APP = 21,
    METHOD_DISABLED = 23,
    COMMUNITY_ACCESS_TOKEN_INVALID = 27,
    APP_ACCESS_TOKEN_INVALID = 28,
    LIMIT_CALLING_REACHED = 29,
    PROFILE_IS_PRIVATE = 30,
    PARAMETER_OMITTED_OR_INVALID = 100,
    INVALID_APP_ID = 101,
    INVALID_USER_ID = 113,
    INVALID_TIMESTAMP = 150,
    ACCESS_ALBUM_PROHIBITED = 200,
    ACCESS_AUDIO_PROHIBITED = 201,
    ACCESS_TO_GROUP_PROHIBITED = 203,
    ALBUM_IS_FULL = 300,
    RECAPTCHA_NEEDED = 3300,
    PHONE_VALIDATION_NEEDED = 3301,
    PASSWORD_VALIDATION_NEEDED = 3302,
    OPT_APP_VALIDATION_NEEDED = 3303,
    EMAIL_CONFIRMATION_NEEDED = 3304,
    ASSERT_VOTES = 3305,
    TOKEN_EXTENSION_REQUIRED = 3609,
    USER_IS_DEACTIVATED = 3610,
    SERVICE_IS_DEACTIVATED_FOR_USER = 3611,
    OTHERS = 0
};

// The base-parent class for UserBase & BotBase classes.
// It has a standard function of Auth and some functions with working with URL scope.
class ClientBase
{
public:
    ClientBase();

    /**
     * @brief  The function of authorization by access token.
     * @note
     *  ClientBase hasn't the own realization of auth's function because of
     *  implementation for user and bot auths is differently.
     * @param  accessToken: the access token
     */
    virtual bool Auth(const std::string& accessToken) = 0;

    /**
     * @brief  Add a scope to the main scope's list.
     * @param  scope: your scope.
     */
    virtual void AddScope(const std::string scope);

    /**
     * @brief  Add a list of scopes to the main scope's list.
     * @param  scopeList: your list of scopes.
     */
    virtual void AddScope(const std::initializer_list<std::string> scopeList);

    /**
     * @brief  Clear the main scope's list.
     */
    virtual void ClearScope();

    /**
     * @brief  Generate a 32 bits random number.
     * @note For example, it is used in the messaging request to the VK server.
     * @retval a 32 bits random number.
     */
    static uint32_t GetRandomId();

    /**
     * @brief Says about the connection.
     *
     * @return true
     * @return false
     */
    virtual bool IsAuthorized() const { return connectedToLongPoll_; }

    ClientBase& operator=(const ClientBase&) = delete;

protected:
    /**
     * @brief  Convert parameters data to URL format.
     * @param  parametersData: your parameters data which need to convert to URL format.
     * @retval a string with parameters in URL format.
     */
    virtual std::string ConvertParametersDataToURL(const json& parametersData);

    /**
     * @brief  Check validation parameters on having items like access_token and others.
     * @note   If the data of paramers won't have necessary parameters the function will add it.
     * @param  parametersData: the data of parameters that you want to check.
     * @retval the correctly data of parameters.
     */
    virtual json CheckValidationParameters(const json& parametersData) = 0;

    /**
     * @brief  Get the error type by a string.
     * @param  errorStr: the error in string format.
     * @retval the type of the error in enum format (VK_REQUEST_ERROR_TYPES).
     */
    VK_REQUEST_ERROR_TYPES GetRequestErrorType(const std::string& errorStr);

protected:
    std::set<std::string> scope_;

    bool connectedToLongPoll_;
};

}

#endif // _CLIENTBASE_HPP_