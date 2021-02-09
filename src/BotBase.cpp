#include "BotBase.hpp"

namespace vk
{

BotBase::BotBase(const std::string groupId, const std::string timeWait)
    : groupId_(groupId)
    , accessToken_("")
    , timeWait_(timeWait)
{}

bool BotBase::Auth(const std::string& accessToken)
{
    if (connectedToLongPoll_)
        throw ex::AlreadyConnectedException();

    if (accessToken.empty())
        throw ex::EmptyArgumentException();

    if (accessToken_ != accessToken)
        accessToken_ = accessToken;

    json parametersData = {
        { "access_token", accessToken_ },
        { "group_id", groupId_ },
        { "v", API_VERSION }
    };

    const std::string method = GetMethodStr(METHODS::GET_LONG_POLL_SERVER);
    const std::string url = API_URL + method;
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(parametersData)));

    if (response.find("error") != response.end()) {
        throw ex::RequestError;
        // std::cout << "Oops, somethind is wrong!\n"
        //     << "Error is: " << response << std::endl;
        /*
            TODO: Add processing this case
        */
    } else {
        json answerDataStr = response["response"];

        secretKey_ = answerDataStr["key"].get<std::string>();
        serverUrl_ = answerDataStr["server"].get<std::string>();
        timeStamp_ = answerDataStr["ts"].get<std::string>();

        connectedToLongPoll_ = true;
    }

    return connectedToLongPoll_;
}

BotBase::Event BotBase::WaitForEvent()
{
    if (!connectedToLongPoll_)
        throw ex::NotConnectedException();

    json parametersData = {
        { "key", secretKey_ },
        { "ts", timeStamp_ },
        { "wait", timeWait_ }
    };

    std::string url = serverUrl_ + "?act=a_check&";
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(parametersData)));

    if (response.find("ts") != response.end()) {
        timeStamp_ = response.at("ts").get<std::string>();
    }

    json updates = response.at("updates")[0];
    auto eventStr = updates.at("type").get<std::string>();

    return Event(GetTypeEvent(eventStr), updates);
}

std::string BotBase::GetMethodStr(const METHODS method)
{
    switch (method) {
    case METHODS::DELETE_COMMENT:
        return "board.deleteComment";
        break;
    case METHODS::RESTORE_COMMENT:
        return "board.restoreComment";
        break;
    case METHODS::ADD_ADDRESS:
        return "groups.addAddress";
        break;
    case METHODS::DELETE_ADDRESS:
        return "groups.deleteAddress";
        break;
    case METHODS::DISABLE_ONLINE:
        return "groups.disableOnline";
        break;
    case METHODS::EDIT_ADDRESS:
        return "groups.editAddress";
        break;
    case METHODS::ENABLE_ONLINE:
        return "groups.enableOnline";
        break;
    case METHODS::GET_BANNED:
        return "groups.getBanned";
        break;
    case METHODS::GET_LONG_POLL_SERVER:
        return "groups.getLongPollServer";
        break;
    case METHODS::GET_LONG_POLL_SETTINGS:
        return "groups.getLongPollSettings";
        break;
    case METHODS::GET_MEMBERS:
        return "groups.getMembers";
        break;
    case METHODS::GET_ONLINE_STATUS:
        return "groups.getOnlineStatus";
        break;
    case METHODS::GET_TOKEN_PERMISSIONS:
        return "groups.getTokenPermissions";
        break;
    case METHODS::IS_MEMBER:
        return "groups.isMember";
        break;
    case METHODS::SET_LONG_POLL_SETTINGS:
        return "groups.setLongPollSettings";
        break;
    case METHODS::SET_SETTINGS:
        return "groups.setSettings";
        break;
    case METHODS::CREATE_CHAT:
        return "messages.createChat";
        break;
    case METHODS::DELETE_MESSAGE:
        return "messages.delete";
        break;
    case METHODS::DELETE_CHAT_PHOTO:
        return "messages.deleteChatPhoto";
        break;
    case METHODS::DELETE_CONVERSATION:
        return "messages.deleteConversation";
        break;
    case METHODS::EDIT_MESSAGE:
        return "messages.edit";
        break;
    case METHODS::EDIT_CHAT:
        return "messages.editChat";
        break;
    case METHODS::GET_BY_CONVERSATION_MESSAGE_ID:
        return "messages.getByConversationMessageId";
        break;
    case METHODS::GET_BY_MESSAGE_ID:
        return "messages.getById";
        break;
    case METHODS::GET_CONVERSATION_MEMBERS:
        return "messages.getConversationMembers";
        break;
    case METHODS::GET_CONVERSATIONS:
        return "messages.getConversations";
        break;
    case METHODS::GET_CONVERSATION_BY_ID:
        return "messages.getConversationById";
        break;
    case METHODS::GET_HISTORY:
        return "messages.getHistory";
        break;
    case METHODS::GET_INVITE_LINK:
        return "messages.getInviteLink";
        break;
    case METHODS::PIN_MESSAGE:
        return "messages.pin";
        break;
    case METHODS::REMOVE_CHAT_USER:
        return "messages.removeChatUser";
        break;
    case METHODS::RESTORE_MESSAGE:
        return "messages.restore";
        break;
    case METHODS::SEARCH_MESSAGE:
        return "messages.search";
        break;
    case METHODS::SEND_MESSAGE:
        return "messages.send";
        break;
    case METHODS::UNPIN_MESSAGE:
        return "messages.unpin";
        break;
    case METHODS::GET_USER:
        return "users.get";
        break;
    case METHODS::CLOSE_COMMENTS:
        return "wall.closeComments";
        break;
    case METHODS::CREATE_COMMENT:
        return "wall.createComment";
        break;
    case METHODS::OPEN_COMMENTS:
        return "wall.openComments";
        break;
    }

    return "";
}

json BotBase::SendRequest(const METHODS method, const json& parametersData)
{
    if (!connectedToLongPoll_)
        throw ex::NotConnectedException();

    std::string methodStr = GetMethodStr(method);
    std::string url = API_URL + methodStr;

    json pData = CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(pData)));

    return response;
}

json BotBase::SendRequest(const std::string& method, const json& parametersData)
{
    if (!connectedToLongPoll_)
        throw ex::NotConnectedException();

    if (method.empty())
        throw ex::EmptyArgumentException();

    std::string url = API_URL + method;

    json pData = CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(pData)));

    return response;
}

json BotBase::CheckValidationParameters(const json& parametersData)
{
    json cParametersData = parametersData;

    if (cParametersData.find("access_token") == cParametersData.end())
        cParametersData.push_back({ "access_token", accessToken_ });

    if (cParametersData.find("group_id") == cParametersData.end())
        cParametersData.push_back({ "group_id", groupId_ });

    if (cParametersData.find("v") == cParametersData.end())
        cParametersData.push_back({ "v", API_VERSION });

    return cParametersData;
}

BotBase::EVENTS BotBase::GetTypeEvent(const std::string& typeEvent)
{
    if (typeEvent == "message_new")
        return EVENTS::MESSAGE_NEW;
    else if (typeEvent == "message_reply")
        return EVENTS::MESSAGE_REPLY;
    else if (typeEvent == "message_allow")
        return EVENTS::MESSAGE_ALLOW;
    else if (typeEvent == "message_deny")
        return EVENTS::MESSAGE_DENY;
    else if (typeEvent == "photo_new")
        return EVENTS::PHOTO_NEW;
    else if (typeEvent == "audio_new")
        return EVENTS::AUDIO_NEW;
    else if (typeEvent == "video_new")
        return EVENTS::VIDEO_NEW;
    else if (typeEvent == "wall_reply_new")
        return EVENTS::WALL_REPLY_NEW;
    else if (typeEvent == "wall_reply_edit")
        return EVENTS::WALL_REPLY_EDIT;
    else if (typeEvent == "wall_reply_delete")
        return EVENTS::WALL_REPLY_DELETE;
    else if (typeEvent == "wall_post_new")
        return EVENTS::WALL_POST_NEW;
    else if (typeEvent == "wall_repost")
        return EVENTS::WALL_REPOST;
    else if (typeEvent == "board_post_new")
        return EVENTS::BOARD_POST_NEW;
    else if (typeEvent == "board_post_edit")
        return EVENTS::BOARD_POST_EDIT;
    else if (typeEvent == "board_post_delete")
        return EVENTS::BOARD_POST_DELETE;
    else if (typeEvent == "board_post_restore")
        return EVENTS::BOARD_POST_RESTORE;
    else if (typeEvent == "photo_comment_new")
        return EVENTS::PHOTO_COMMENT_NEW;
    else if (typeEvent == "photo_comment_edit")
        return EVENTS::PHOTO_COMMENT_EDIT;
    else if (typeEvent == "photo_comment_delete")
        return EVENTS::PHOTO_COMMENT_DELETE;
    else if (typeEvent == "photo_comment_restore")
        return EVENTS::PHOTO_COMMENT_RESTORE;
    else if (typeEvent == "video_comment_new")
        return EVENTS::VIDEO_COMMENT_NEW;
    else if (typeEvent == "video_comment_edit")
        return EVENTS::VIDEO_COMMENT_EDIT;
    else if (typeEvent == "video_comment_delete")
        return EVENTS::VIDEO_COMMENT_DELETE;
    else if (typeEvent == "video_comment_restore")
        return EVENTS::VIDEO_COMMENT_RESTORE;
    else if (typeEvent == "market_comment_new")
        return EVENTS::MARKET_COMMENT_NEW;
    else if (typeEvent == "market_comment_edit")
        return EVENTS::MARKET_COMMENT_EDIT;
    else if (typeEvent == "market_comment_delete")
        return EVENTS::MARKET_COMMENT_DELETE;
    else if (typeEvent == "market_comment_restore")
        return EVENTS::MARKET_COMMENT_RESTORE;
    else if (typeEvent == "poll_vote_new")
        return EVENTS::POLL_VOTE_NEW;
    else if (typeEvent == "group_join")
        return EVENTS::GROUP_JOIN;
    else if (typeEvent == "group_leave")
        return EVENTS::GROUP_LEAVE;
    else if (typeEvent == "user_block")
        return EVENTS::USER_BLOCK;
    else if (typeEvent == "user_unblock")
        return EVENTS::USER_UNBLOCK;
    else if (typeEvent == "group_change_settings")
        return EVENTS::GROUP_CHANGE_SETTINGS;
    else if (typeEvent == "group_change_photo")
        return EVENTS::GROUP_CHANGE_PHOTO;
    else if (typeEvent == "group_officers_edit")
        return EVENTS::GROUP_OFFICERS_EDIT;
    else
        return EVENTS::UNKNOWN;
}
}