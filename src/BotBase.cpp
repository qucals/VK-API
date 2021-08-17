/**
 * Contains the class for working with vkbot.
 * @file BotBase.cpp
 * @author qucals
 * @version 0.0.5 18/08/21
 */

#include <BotBase.hpp>

namespace vk
{

namespace base
{

namespace bot
{

BotBase::BotBase(std::string groupId, std::string timeWait)
    : m_groupId(__MOVE(groupId))
    , m_timeWait(__MOVE(timeWait))
{}

bool BotBase::Auth(const std::string& accessToken)
{
    if (IsAuthorized()) { throw ex::AlreadyConnectedException(); }
    if (accessToken.empty()) { throw ex::EmptyArgumentException(); }

    if (m_accessToken != accessToken) { m_accessToken = accessToken; }

    json parametersData = {
        { "access_token", m_accessToken },
        { "group_id",     m_groupId },
        { "v",            VKAPI_API_VERSION }
    };

    const std::string method = MethodToString(METHODS::GET_LONG_POLL_SERVER);
    const std::string url = VKAPI_API_URL + method;
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(parametersData)));

    if (response.find("error") != response.end()) {
        throw ex::RequestError();
        // TODO (#12): Add error handling when BotBase::Auth failed
    } else {
        json answerDataStr = response["response"];

        m_secretKey = answerDataStr["key"].get<std::string>();
        m_serverUrl = answerDataStr["server"].get<std::string>();
        m_timeStamp = answerDataStr["ts"].get<std::string>();

        m_connectedToLongPoll = true;
    }

    return m_connectedToLongPoll;
}

BotBase::Event BotBase::WaitForEvent()
{
    if (!IsAuthorized()) { throw ex::NotConnectedException(); }

    json parametersData = {
        { "key",  m_secretKey },
        { "ts",   m_timeStamp },
        { "wait", m_timeWait }
    };

    std::string url = m_serverUrl + "?act=a_check&";
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(parametersData)));

    if (response.find("ts") != response.end()) {
        m_timeStamp = response.at("ts").get<std::string>();
    }

    json updates = response.at("updates")[0];
    std::string eventStr = updates.at("type").get<std::string>();

    return Event(GetTypeEvent(eventStr), updates);
}

std::string BotBase::MethodToString(const METHODS method)
{
    switch (method) {
    case METHODS::DELETE_COMMENT:
        return "board.deleteComment";
    case METHODS::RESTORE_COMMENT:
        return "board.restoreComment";
    case METHODS::ADD_ADDRESS:
        return "groups.addAddress";
    case METHODS::DELETE_ADDRESS:
        return "groups.deleteAddress";
    case METHODS::DISABLE_ONLINE:
        return "groups.disableOnline";
    case METHODS::EDIT_ADDRESS:
        return "groups.editAddress";
    case METHODS::ENABLE_ONLINE:
        return "groups.enableOnline";
    case METHODS::GET_BANNED:
        return "groups.getBanned";
    case METHODS::GET_LONG_POLL_SERVER:
        return "groups.getLongPollServer";
    case METHODS::GET_LONG_POLL_SETTINGS:
        return "groups.getLongPollSettings";
    case METHODS::GET_MEMBERS:
        return "groups.getMembers";
    case METHODS::GET_ONLINE_STATUS:
        return "groups.getOnlineStatus";
    case METHODS::GET_TOKEN_PERMISSIONS:
        return "groups.getTokenPermissions";
    case METHODS::IS_MEMBER:
        return "groups.isMember";
    case METHODS::SET_LONG_POLL_SETTINGS:
        return "groups.setLongPollSettings";
    case METHODS::SET_SETTINGS:
        return "groups.setSettings";
    case METHODS::CREATE_CHAT:
        return "messages.createChat";
    case METHODS::DELETE_MESSAGE:
        return "messages.delete";
    case METHODS::DELETE_CHAT_PHOTO:
        return "messages.deleteChatPhoto";
    case METHODS::DELETE_CONVERSATION:
        return "messages.deleteConversation";
    case METHODS::EDIT_MESSAGE:
        return "messages.edit";
    case METHODS::EDIT_CHAT:
        return "messages.editChat";
    case METHODS::GET_BY_CONVERSATION_MESSAGE_ID:
        return "messages.getByConversationMessageId";
    case METHODS::GET_BY_MESSAGE_ID:
        return "messages.getById";
    case METHODS::GET_CONVERSATION_MEMBERS:
        return "messages.getConversationMembers";
    case METHODS::GET_CONVERSATIONS:
        return "messages.getConversations";
    case METHODS::GET_CONVERSATION_BY_ID:
        return "messages.getConversationById";
    case METHODS::GET_HISTORY:
        return "messages.getHistory";
    case METHODS::GET_INVITE_LINK:
        return "messages.getInviteLink";
    case METHODS::PIN_MESSAGE:
        return "messages.pin";
    case METHODS::REMOVE_CHAT_USER:
        return "messages.removeChatUser";
    case METHODS::RESTORE_MESSAGE:
        return "messages.restore";
    case METHODS::SEARCH_MESSAGE:
        return "messages.search";
    case METHODS::SEND_MESSAGE:
        return "messages.send";
    case METHODS::UNPIN_MESSAGE:
        return "messages.unpin";
    case METHODS::GET_USER:
        return "users.get";
    case METHODS::CLOSE_COMMENTS:
        return "wall.closeComments";
    case METHODS::CREATE_COMMENT:
        return "wall.createComment";
    case METHODS::OPEN_COMMENTS:
        return "wall.openComments";
    }

    return "";
}

json BotBase::SendRequest(const METHODS method, const json& parametersData)
{
    if (!IsAuthorized()) { throw ex::NotConnectedException(); }

    std::string methodStr = MethodToString(method);
    std::string url = VKAPI_API_URL + methodStr;

    json pData = CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(pData)));

    return response;
}

json BotBase::SendRequest(const std::string& method, const json& parametersData)
{
    if (!IsAuthorized()) { throw ex::NotConnectedException(); }
    if (method.empty()) { throw ex::EmptyArgumentException(); }

    std::string url = VKAPI_API_URL + method;

    json pData = CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(pData)));

    return response;
}

#ifdef __CPLUSPLUS_OVER_11

auto BotBase::SendRequestAsync(const METHODS method, const json& parametersData)
{ return std::async(BotBase::SendRequestAsync_, this, method, parametersData); }

auto BotBase::SendRequestAsync(const std::string& method, const json& parametersData)
{ return std::async(BotBase::SendRequestAsync__, this, method, parametersData); }

json BotBase::SendRequestAsync_(BotBase* handle, const METHODS method, const json& parametersData)
{
    assert(handle == nullptr);
    if (!handle->IsAuthorized()) { throw ex::NotConnectedException(); }

    std::string methodStr = BotBase::MethodToString(method);
    std::string url = VKAPI_API_URL + methodStr;

    json pData = handle->CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, handle->ConvertParametersDataToURL(pData)));

    return response;
}

json BotBase::SendRequestAsync__(BotBase* handle, const std::string& method, const json& parametersData)
{
    assert(handle == nullptr);
    if (!handle->IsAuthorized()) { throw ex::NotConnectedException(); }
    if (method.empty()) { throw ex::EmptyArgumentException(); }

    std::string url = VKAPI_API_URL + method;

    json pData = handle->CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, handle->ConvertParametersDataToURL(pData)));

    return response;
}

#endif // __CPLUSPLUS_OVER_11

json BotBase::CheckValidationParameters(const json& parametersData)
{
    json cParametersData = parametersData;

    if (cParametersData.find("access_token") == cParametersData.end()) {
        cParametersData.push_back({ "access_token", m_accessToken });
    }

    if (cParametersData.find("group_id") == cParametersData.end()) {
        cParametersData.push_back({ "group_id", m_groupId });
    }

    if (cParametersData.find("v") == cParametersData.end()) {
        cParametersData.push_back({ "v", VKAPI_API_VERSION });
    }

    return cParametersData;
}

BotBase::EVENTS BotBase::GetTypeEvent(const std::string& typeEvent)
{
    if (typeEvent == "message_new") {
        return EVENTS::MESSAGE_NEW;
    } else if (typeEvent == "message_reply") {
        return EVENTS::MESSAGE_REPLY;
    } else if (typeEvent == "message_allow") {
        return EVENTS::MESSAGE_ALLOW;
    } else if (typeEvent == "message_deny") {
        return EVENTS::MESSAGE_DENY;
    } else if (typeEvent == "photo_new") {
        return EVENTS::PHOTO_NEW;
    } else if (typeEvent == "audio_new") {
        return EVENTS::AUDIO_NEW;
    } else if (typeEvent == "video_new") {
        return EVENTS::VIDEO_NEW;
    } else if (typeEvent == "wall_reply_new") {
        return EVENTS::WALL_REPLY_NEW;
    } else if (typeEvent == "wall_reply_edit") {
        return EVENTS::WALL_REPLY_EDIT;
    } else if (typeEvent == "wall_reply_delete") {
        return EVENTS::WALL_REPLY_DELETE;
    } else if (typeEvent == "wall_post_new") {
        return EVENTS::WALL_POST_NEW;
    } else if (typeEvent == "wall_repost") {
        return EVENTS::WALL_REPOST;
    } else if (typeEvent == "board_post_new") {
        return EVENTS::BOARD_POST_NEW;
    } else if (typeEvent == "board_post_edit") {
        return EVENTS::BOARD_POST_EDIT;
    } else if (typeEvent == "board_post_delete") {
        return EVENTS::BOARD_POST_DELETE;
    } else if (typeEvent == "board_post_restore") {
        return EVENTS::BOARD_POST_RESTORE;
    } else if (typeEvent == "photo_comment_new") {
        return EVENTS::PHOTO_COMMENT_NEW;
    } else if (typeEvent == "photo_comment_edit") {
        return EVENTS::PHOTO_COMMENT_EDIT;
    } else if (typeEvent == "photo_comment_delete") {
        return EVENTS::PHOTO_COMMENT_DELETE;
    } else if (typeEvent == "photo_comment_restore") {
        return EVENTS::PHOTO_COMMENT_RESTORE;
    } else if (typeEvent == "video_comment_new") {
        return EVENTS::VIDEO_COMMENT_NEW;
    } else if (typeEvent == "video_comment_edit") {
        return EVENTS::VIDEO_COMMENT_EDIT;
    } else if (typeEvent == "video_comment_delete") {
        return EVENTS::VIDEO_COMMENT_DELETE;
    } else if (typeEvent == "video_comment_restore") {
        return EVENTS::VIDEO_COMMENT_RESTORE;
    } else if (typeEvent == "market_comment_new") {
        return EVENTS::MARKET_COMMENT_NEW;
    } else if (typeEvent == "market_comment_edit") {
        return EVENTS::MARKET_COMMENT_EDIT;
    } else if (typeEvent == "market_comment_delete") {
        return EVENTS::MARKET_COMMENT_DELETE;
    } else if (typeEvent == "market_comment_restore") {
        return EVENTS::MARKET_COMMENT_RESTORE;
    } else if (typeEvent == "poll_vote_new") {
        return EVENTS::POLL_VOTE_NEW;
    } else if (typeEvent == "group_join") {
        return EVENTS::GROUP_JOIN;
    } else if (typeEvent == "group_leave") {
        return EVENTS::GROUP_LEAVE;
    } else if (typeEvent == "user_block") {
        return EVENTS::USER_BLOCK;
    } else if (typeEvent == "user_unblock") {
        return EVENTS::USER_UNBLOCK;
    } else if (typeEvent == "group_change_settings") {
        return EVENTS::GROUP_CHANGE_SETTINGS;
    } else if (typeEvent == "group_change_photo") {
        return EVENTS::GROUP_CHANGE_PHOTO;
    } else if (typeEvent == "group_officers_edit") {
        return EVENTS::GROUP_OFFICERS_EDIT;
    } else {
        return EVENTS::UNKNOWN;
    }
}

} // namespace bot

} // namespace base

} // namespace vk