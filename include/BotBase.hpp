/**
 * Contains the class for working with vkbot.
 * @file BotBase.hpp
 * @author qucals
 * @version 0.0.5 18/08/21
 */

#pragma once

#ifndef _BOTBASE_HPP_
#define _BOTBASE_HPP_

#include <ClientBase.hpp>

namespace vk
{

namespace base
{

namespace bot
{

constexpr const char* DEFAULT_TIME_WAIT = "25";

/**
 * @brief The class for working from bots by Long Poll Server.
 */
class BotBase : public ClientBase
{
public:
    // There are bot methods, but not all.
    // Description about below methods you can find at https://vk.com/dev/methods
    enum class METHODS
    {
        DELETE_COMMENT,
        RESTORE_COMMENT,
        ADD_ADDRESS,
        DELETE_ADDRESS,
        DISABLE_ONLINE,
        EDIT_ADDRESS,
        ENABLE_ONLINE,
        GET_BANNED,
        GET_LONG_POLL_SERVER,
        GET_LONG_POLL_SETTINGS,
        GET_MEMBERS,
        GET_ONLINE_STATUS,
        GET_TOKEN_PERMISSIONS,
        IS_MEMBER,
        SET_LONG_POLL_SETTINGS,
        SET_SETTINGS,
        CREATE_CHAT,
        DELETE_MESSAGE,
        DELETE_CHAT_PHOTO,
        DELETE_CONVERSATION,
        EDIT_MESSAGE,
        EDIT_CHAT,
        GET_BY_CONVERSATION_MESSAGE_ID,
        GET_BY_MESSAGE_ID,
        GET_CONVERSATION_MEMBERS,
        GET_CONVERSATIONS,
        GET_CONVERSATION_BY_ID,
        GET_HISTORY,
        GET_INVITE_LINK,
        PIN_MESSAGE,
        REMOVE_CHAT_USER,
        RESTORE_MESSAGE,
        SEARCH_MESSAGE,
        SEND_MESSAGE,
        UNPIN_MESSAGE,
        GET_USER,
        CLOSE_COMMENTS,
        CREATE_COMMENT,
        OPEN_COMMENTS,
    };

    // The standard events which the server can return.
    enum class EVENTS
    {
        MESSAGE_NEW,
        MESSAGE_REPLY,
        MESSAGE_ALLOW,
        MESSAGE_DENY,
        PHOTO_NEW,
        AUDIO_NEW,
        VIDEO_NEW,
        WALL_REPLY_NEW,
        WALL_REPLY_EDIT,
        WALL_REPLY_DELETE,
        WALL_POST_NEW,
        WALL_REPOST,
        BOARD_POST_NEW,
        BOARD_POST_EDIT,
        BOARD_POST_DELETE,
        BOARD_POST_RESTORE,
        PHOTO_COMMENT_NEW,
        PHOTO_COMMENT_EDIT,
        PHOTO_COMMENT_DELETE,
        PHOTO_COMMENT_RESTORE,
        VIDEO_COMMENT_NEW,
        VIDEO_COMMENT_EDIT,
        VIDEO_COMMENT_DELETE,
        VIDEO_COMMENT_RESTORE,
        MARKET_COMMENT_NEW,
        MARKET_COMMENT_EDIT,
        MARKET_COMMENT_DELETE,
        MARKET_COMMENT_RESTORE,
        POLL_VOTE_NEW,
        GROUP_JOIN,
        GROUP_LEAVE,
        USER_BLOCK,
        USER_UNBLOCK,
        GROUP_CHANGE_SETTINGS,
        GROUP_CHANGE_PHOTO,
        GROUP_OFFICERS_EDIT,
        UNKNOWN
    };

    struct Event
    {
        json parameters;
        EVENTS type;

        Event(const EVENTS _type, json _parameters)
            : parameters(__MOVE(_parameters))
            , type(_type)
        {}
    };

public:
    /**
     * @param  groupId: the id of the bot's group.
     * @param  timeWait:
     *  The time of waiting for any event.
     *  In the default timeWait equals DEFAULT_TIME_WAIT ("25").
     */
    __EXPLICIT BotBase(std::string groupId, std::string timeWait = DEFAULT_TIME_WAIT);

    ~BotBase() = default;

    /**
     * @brief  The authorization function by the access token
     * @param  accessToken: the access token
     * @retval 'true' if authorization is successfully and 'false' in another case.
     */
    bool Auth(const std::string& accessToken) __FINAL;

    /**
     * @brief  The function of waiting for any event after authorization.
     * @retval If the event has happened the function returns the description about it.
     */
    Event WaitForEvent();

    /**
     * @brief  The function of converting an enum's method to a string (URL).
     * @param  method: the enum's method.
     * @retval a string (URL) of this method.
     */
    __STATIC std::string MethodToString(METHODS method);

    /**
     * @brief  The function of sending any request to the VK server.
     * @param  method: the enum's method.
     * @param  parametersData: the data of parameters for your request.
     * @retval the answer of your request in json.
     */
    json SendRequest(METHODS method, const json& parametersData);

    /**
     * @brief  The function of sending any request to the VK server.
     * @param  method: your method in str format.
     * @param  parametersData: the data of parameters for your request.
     * @retval the answer of your request in json.
     */
    json SendRequest(const std::string& method, const json& parametersData);

#ifdef __CPLUSPLUS_OVER_11

    /**
     * @brief  The function witch calls private function for sending a request in asynchronous mode.
     * @param  method: the enum's method.
     * @param  parametersData: the data of parameters for your request.
     * @retval the answer of your request in json.
     */
    auto SendRequestAsync(METHODS method, const json& parametersData);

    /**
     * @brief  The function witch calls private function for sending a request in asynchronous mode.
     * @param  method: your method in str format.
     * @param  parametersData: the data of parameters for your request.
     * @retval the answer of your request in json.
     */
    auto SendRequestAsync(const std::string& method, const json& parametersData);

#endif // __CPLUSPLUS_OVER_11

protected:
    /**
     * @brief
     *  Checking the data of parameters on validation.
     *  If the following items won't be found the function will add it.
     * @param  parametersData: the data for checking validation.
     * @retval the correctly data of parameters in json.
     */
    json CheckValidationParameters(const json& parametersData) __OVERRIDE;

    /**
     * @brief  Get the type of events by string.
     * @param  typeEvent: the event in str.
     * @retval the type of event in enum (EVENTS).
     */
    __STATIC EVENTS GetTypeEvent(const std::string& typeEvent);

private:

#ifdef __CPLUSPLUS_OVER_11
    /**
     * @brief The static function of sending any request to the VK server.
     * @param handle: a pointer to your handle of BotBase.
     * @param method: the enum's method.
     * @param parametersData:  the data of parameters for your request.
     */
    __STATIC json SendRequestAsync_(BotBase* handle, METHODS method, const json& parametersData);

    /**
     * @brief The static function of sending any request to the VK server.
     * @param handle: a pointer to your handle of BotBase.
     * @param method: your method in str format.
     * @param parametersData:  the data of parameters for your request.
     */
    __STATIC json SendRequestAsync__(BotBase* handle, const std::string& method, const json& parametersData);

#endif // #ifdef __CPLUSPLUS_OVER_11

private:
    const std::string m_groupId;
    std::string m_accessToken;

    json m_previousEvent;

    std::string m_secretKey;
    std::string m_serverUrl;
    std::string m_timeStamp;

    std::string m_timeWait;
};

} // namespace bot

} // namespace base

} // namespace vk

#endif // _BOTBASE_HPP_