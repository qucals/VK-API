/**
 * Describes the class for working with VK account.
 * @file UserBase.hpp
 * @author qucals
 * @version 0.0.5 18/08/21
 */

#include <UserBase.hpp>

namespace vk
{

namespace base
{

namespace user
{

UserBase::UserBase(std::string appId, std::string appSecureKey)
    : ClientBase()
    , appId_(__MOVE(appId))
    , appSecureKey_(__MOVE(appSecureKey))
{
    if (appId_.empty() || appSecureKey_.empty()) { throw ex::EmptyArgumentException(); }
}

bool UserBase::Auth(std::string& login, std::string& password)
{
    if (login.empty() || password.empty()) { throw ex::EmptyArgumentException(); }
    if (IsAuthorized()) { m_connectedToLongPoll = false; }

    std::string scope;

    if (!m_scope.empty()) {
#ifdef __CPLUSPLUS_OVER_11
        for (const auto& i : m_scope) { scope += "," + i; }
#else
        for (std::set<std::string>::iterator iter = m_scope.begin();
             iter != m_scope.end();
             iter++) {
            scope += "," + i;
        }
#endif // __CPLUSPLUS_OVER_11
        scope = scope.substr(0, scope.size() - 1);
    }

    json parametersData = {
        { "client_id",     appId_ },
        { "grant_type",    "password" },
        { "client_secret", appSecureKey_ },
        { "scope",         scope },
        { "username",      login },
        { "password",      password }
    };

    json response = json::parse(Request::Send(VKAPI_AUTH_URL,
                                              ConvertParametersDataToURL(parametersData)));

    try {
        if (response.find("error") != response.end()) {
            std::string errorTypeStr = response["error"].get<std::string>();
            VK_REQUEST_ERROR_TYPES errorType = GetRequestErrorType(errorTypeStr);

            if (errorType == VK_REQUEST_ERROR_TYPES::NEED_CAPTCHA) {
                std::cout << GetURLCaptcha(parametersData, response);

                std::string captchaKey;
                std::cin >> captchaKey;

                parametersData.push_back({ "captcha_key", captchaKey });
                std::string answer = Request::Send(VKAPI_AUTH_URL, ConvertParametersDataToURL(parametersData));

                // TODO (#11): Add further processing
            }

            VALIDATION_TYPES validationType = GetValidationType(response.at("validation_type"));

            if (validationType == VALIDATION_TYPES::TWOFA_SMS ||
                validationType == VALIDATION_TYPES::TWOFA_APP) {
                parametersData.push_back({ "2fa_supported", "1" });

                if (validationType == VALIDATION_TYPES::TWOFA_SMS) {
                    parametersData.push_back({ "force_sms", "1" });
                } else {
                    parametersData.push_back({ "2fa_app", "1" });
                }

                Request::Send(response.at("redirect_url").get<std::string>(), "");
                Request::Send(VKAPI_AUTH_URL, ConvertParametersDataToURL(parametersData));

                std::cout << "Enter code: ";

                std::string code;
                std::cin >> code;

                parametersData.push_back({ "code", code });
                response = json::parse(Request::Send(VKAPI_AUTH_URL, ConvertParametersDataToURL(parametersData)));

                accessToken_ = response.at("access_token").get<std::string>();
                userId_ = response.at("user_id").get<std::string>();
                m_connectedToLongPoll = true;
            }
        } else {
            accessToken_ = response.at("access_token").get<std::string>();
            userId_ = response.at("user_id").get<std::string>();
            m_connectedToLongPoll = true;
        }
    } catch (json::exception& exc) {
        std::cerr << "exception message: " << exc.what() << std::endl;
        std::cerr << "exception id: " << exc.id << std::endl;
    }

    return m_connectedToLongPoll;
}

bool UserBase::Auth(const std::string& accessToken)
{
    if (accessToken.empty()) { throw ex::EmptyArgumentException(); }
    if (IsAuthorized()) { m_connectedToLongPoll = false; }

    json parametersData = {
        { "access_token", accessToken },
        { "v",            VKAPI_API_VERSION }
    };

    const std::string method = MethodToString(METHODS::USERS_GET);
    const std::string url = VKAPI_API_URL + method;

    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(parametersData)));

    try {
        if (response.find("error") != response.end()) {
#ifdef __CPLUSPLUS_OVER_11
            for (const auto& data : response.at("response").items()) {
                userId_ = data.value().at("id").get<std::string>();
            }
#else
            json __response = response.at("response").items();
            for (json::iterator iter = __response.begin();
                 iter != __response.end();
                 iter++) {
                userId_ = iter->value().at("id").get<std::string>();
            }
#endif // __CPLUSPLUS_OVER_11
            accessToken_ = accessToken;
            m_connectedToLongPoll = true;
        } else {
            userId_.clear();
            accessToken_.clear();
            m_connectedToLongPoll = false;
        }
    } catch (json::exception& exc) {
        std::cerr << "exception message: " << exc.what() << std::endl;
        std::cerr << "exception id: " << exc.id << std::endl;
    }

    return m_connectedToLongPoll;
}

json UserBase::CheckValidationParameters(const json& parametersData)
{
    json cParametersData = parametersData;

    if (cParametersData.find("access_token") == cParametersData.end()) {
        cParametersData.push_back({ "access_token", accessToken_ });
    }

    if (cParametersData.find("v") == cParametersData.end()) {
        cParametersData.push_back({ "v", VKAPI_API_VERSION });
    }

    return cParametersData;
}

std::string UserBase::GetURLCaptcha(json& parametersData, const json& response)
{
    parametersData.push_back(
        { "captcha_sid", response.at("captcha_sid").get<std::string>() });
    return response.at("captcha_img").get<std::string>();
}

UserBase::VALIDATION_TYPES UserBase::GetValidationType(const std::string& descriptionType)
{
    if (descriptionType == "2fa_sms") {
        return VALIDATION_TYPES::TWOFA_SMS;
    } else if (descriptionType == "2fa_app") {
        return VALIDATION_TYPES::TWOFA_APP;
    } else {
        return VALIDATION_TYPES::UNKNOWN;
    }
}

json UserBase::SendRequest(METHODS method, const json& parametersData)
{
    if (!IsAuthorized()) { throw ex::NotConnectedException(); }

    std::string methodStr = MethodToString(method);
    std::string url = VKAPI_API_URL + methodStr;

    json pData = CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(pData)));

    return response;
}

json UserBase::SendRequest(const std::string& method, const json& parametersData)
{
    if (!IsAuthorized()) { throw ex::NotConnectedException(); }
    if (method.empty()) { throw ex::EmptyArgumentException(); }

    std::string url = VKAPI_API_URL + method;

    json pData = CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(pData)));

    return response;
}

std::string UserBase::MethodToString(METHODS method)
{
    switch (method) {
    case METHODS::ACCOUNT_BAN:
        return "account.Ban";
    case METHODS::ACCOUNT_CHANGE_PASSWORD:
        return "account.changePassword";
    case METHODS::ACCOUNT_GET_ACTIVE_OFFERS:
        return "account.getActiveOffers";
    case METHODS::ACCOUNT_GET_APP_PERMISSIONS:
        return "account.getAppPermissions";
    case METHODS::ACCOUNT_GET_BANNED:
        return "account.getBanned";
    case METHODS::ACCOUNT_GET_COUNTERS:
        return "account.getCounters";
    case METHODS::ACCOUNT_GET_INFO:
        return "account.getInfo";
    case METHODS::ACCOUNT_GET_PROFILE_INFO:
        return "account.getProfileInfo";
    case METHODS::ACCOUNT_GET_PUSH_SETTINGS:
        return "account.getPushSettings";
    case METHODS::ACCOUNT_REGISTER_DEVICE:
        return "account.registerDevice";
    case METHODS::ACCOUNT_SAVE_PROFILE_INFO:
        return "account.saveProfileInfo";
    case METHODS::ACCOUNT_SET_INFO:
        return "account.setInfo";
    case METHODS::ACCOUNT_SET_PUSH_SETTINGS:
        return "account.setPushSettings";
    case METHODS::ACCOUNT_UNBAN:
        return "account.unban";
    case METHODS::ACCOUNT_UNREGISTER_DEVICE:
        return "account.unregisterDevice";
    case METHODS::BOARD_ADD_TOPIC:
        return "board.addTopic";
    case METHODS::BOARD_CLOSE_TOPIC:
        return "board.closeTopic";
    case METHODS::BOARD_CREATE_COMMENT:
        return "board.createComment";
    case METHODS::BOARD_DELETE_COMMENT:
        return "board.deleteComment";
    case METHODS::BOARD_DELETE_TOPIC:
        return "board.deleteTopic";
    case METHODS::BOARD_EDIT_COMMENT:
        return "board.editComment";
    case METHODS::BOARD_EDIT_TOPIC:
        return "board.editTopic";
    case METHODS::BOARD_FIX_TOPIC:
        return "board.fixTopic";
    case METHODS::BOARD_GET_COMMENTS:
        return "board.getComments";
    case METHODS::BOARD_GET_TOPICS:
        return "board.getTopics";
    case METHODS::BOARD_OPEN_TOPIC:
        return "board.openTopic";
    case METHODS::BOARD_RESTORE_COMMENT:
        return "board.restoreComment";
    case METHODS::BOARD_UNFIX_TOPIC:
        return "board.unfixTopic";
    case METHODS::DATABASE_GET_CHAIRS:
        return "database.getChairs";
    case METHODS::DATABASE_GET_CITIES:
        return "database.getCities";
    case METHODS::DATABASE_GET_CITIES_BY_ID:
        return "database.getCitiesById";
    case METHODS::DATABASE_GET_COUNTRIES:
        return "database.getCounties";
    case METHODS::DATABASE_GET_FACULTIES:
        return "database.getFaculties";
    case METHODS::DATABASE_GET_METRO_STATIONS:
        return "database.getMetroStations";
    case METHODS::DATABASE_GET_REGIONS:
        return "database.getRegions";
    case METHODS::DATABASE_GET_SCHOOL_CLASSES:
        return "database.getSchoolClasses";
    case METHODS::DATABASE_GET_SCHOOLS:
        return "database.getSchools";
    case METHODS::DATABASE_GET_UNIVERSITIES:
        return "database.getUniversities";
    case METHODS::DOCS_ADD:
        return "docs.add";
    case METHODS::DOCS_DELETE:
        return "docs.delete";
    case METHODS::DOCS_EDIT:
        return "docs.edit";
    case METHODS::DOCS_GET:
        return "docs.get";
    case METHODS::DOCS_GET_BY_ID:
        return "docs.getById";
    case METHODS::DOCS_GET_MESSAGES_UPLOAD_SERVER:
        return "docs.getMessagesUploadServer";
    case METHODS::DOCS_GET_TYPES:
        return "docs.getTypes";
    case METHODS::DOCS_GET_UPLOAD_SERVER:
        return "docs.getUploadServer";
    case METHODS::DOCS_GET_WALL_UPLOAD_SERVER:
        return "docs.getWallUploadServer";
    case METHODS::DOCS_SAVE:
        return "docs.save";
    case METHODS::DOCS_SEARCH:
        return "docs.search";
    case METHODS::EXECUTE:
        return "execute";
    case METHODS::FAVE_ADD_ARTICLE:
        return "fave.addArticle";
    case METHODS::FAVE_ADD_LINK:
        return "fave.addLink";
    case METHODS::FAVE_ADD_PAGE:
        return "fave.addPage";
    case METHODS::FAVE_ADD_POST:
        return "fave.addPost";
    case METHODS::FAVE_ADD_PRODUCT:
        return "fave.addProduct";
    case METHODS::FAVE_ADD_TAG:
        return "fave.addTag";
    case METHODS::FAVE_ADD_VIDEO:
        return "fave.addVideo";
    case METHODS::FAVE_EDIT_TAG:
        return "fave.editTag";
    case METHODS::FAVE_GET:
        return "fave.get";
    case METHODS::FAVE_GET_PAGES:
        return "fave.getPages";
    case METHODS::FAVE_GET_TAGS:
        return "fave.getTags";
    case METHODS::FAVE_MARK_SEEN:
        return "fave.markSeen";
    case METHODS::FAVE_REMOVE_ARTICLE:
        return "fave.removeArticle";
    case METHODS::FAVE_REMOVE_LINK:
        return "fave.removeLink";
    case METHODS::FAVE_REMOVE_PAGE:
        return "fave.removePage";
    case METHODS::FAVE_REMOVE_POST:
        return "fave.removePost";
    case METHODS::FAVE_REMOVE_PRODUCT:
        return "fave.removeProduct";
    case METHODS::FAVE_REMOVE_TAG:
        return "fave.removeTag";
    case METHODS::FAVE_REMOVE_VIDEO:
        return "fave.removeVideo";
    case METHODS::FAVE_REORDER_TAGS:
        return "fave.reorderTags";
    case METHODS::FAVE_SET_PAGE_TAGS:
        return "fave.setPageTags";
    case METHODS::FAVE_SET_TAGS:
        return "fave.setTags";
    case METHODS::FAVE_TRACK_PAGE_INTERACTION:
        return "fave.trackPageInteraction";
    case METHODS::FRIENDS_ADD:
        return "friends.add";
    case METHODS::FRIENDS_ADD_LIST:
        return "friends.addList";
    case METHODS::FRIENDS_ARE_FRIENDS:
        return "friends.areFriends";
    case METHODS::FRIENDS_DELETE:
        return "friends.delete";
    case METHODS::FRIENDS_DELETE_ALL_REQUESTS:
        return "friends.deleteAllRequests";
    case METHODS::FRIENDS_DELETE_LIST:
        return "friends.deleteList";
    case METHODS::FRIENDS_EDIT:
        return "friends.edit";
    case METHODS::FRIENDS_EDIT_LIST:
        return "friends.editList";
    case METHODS::FRIENDS_GET:
        return "friends.get";
    case METHODS::FRIENDS_GET_APP_USERS:
        return "friends.getAppUsers";
    case METHODS::FRIENDS_GET_BY_PHONES:
        return "friends.getByPhones";
    case METHODS::FRIENDS_GET_LISTS:
        return "friends.getLists";
    case METHODS::FRIENDS_GET_MUTUAL:
        return "friends.getMutual";
    case METHODS::FRIENDS_GET_ONLINE:
        return "friends.getOnline";
    case METHODS::FRIENDS_GET_RECENT:
        return "friends.getRecent";
    case METHODS::FRIENDS_GET_REQUESTS:
        return "friends.getRequests";
    case METHODS::FRIENDS_GET_SUGGESTIONS:
        return "friends.getSuggestions";
    case METHODS::FRIENDS_SEARCH:
        return "friends.search";
    case METHODS::GIFTS_GET:
        return "gifts.get";
    case METHODS::GROUPS_ADD_ADDRESS:
        return "groups.addAddress";
    case METHODS::GROUPS_ADD_CALLBACK_SERVER:
        return "groups.addCallbackServer";
    case METHODS::GROUPS_ADD_LINK:
        return "groups.addLink";
    case METHODS::GROUPS_APPROVE_REQUEST:
        return "groups.approveRequest";
    case METHODS::GROUPS_BAN:
        return "groups.ban";
    case METHODS::GROUPS_CREATE:
        return "groups.create";
    case METHODS::GROUPS_DELETE_ADDRESS:
        return "groups.deleteAddress";
    case METHODS::GROUPS_DELETE_CALLBACK_SERVER:
        return "groups.deleteCallbackServer";
    case METHODS::GROUPS_DELETE_LINK:
        return "groups.deleteLink";
    case METHODS::GROUPS_DISABLE_ONLINE:
        return "groups.disableOnline";
    case METHODS::GROUPS_EDIT:
        return "groups.edit";
    case METHODS::GROUPS_EDIT_ADDRESS:
        return "groups.editAddress";
    case METHODS::GROUPS_EDIT_CALLBACK_SERVER:
        return "groups.editCallbackServer";
    case METHODS::GROUPS_EDIT_LINK:
        return "groups.editLink";
    case METHODS::GROUPS_EDIT_MANAGER:
        return "groups.editManager";
    case METHODS::GROUPS_ENABLE_ONLINE:
        return "groups.enableOnline";
    case METHODS::GROUPS_GET:
        return "groups.get";
    case METHODS::GROUPS_GET_ADDRESSES:
        return "groups.getAddresses";
    case METHODS::GROUPS_GET_BANNED:
        return "groups.getBanned";
    case METHODS::GROUPS_GET_BY_ID:
        return "groups.getById";
    case METHODS::GROUPS_GET_CALLBACK_CONFIRMATION_CODE:
        return "groups.getCallbackConfirmationCode";
    case METHODS::GROUPS_GET_CALLBACK_SERVER:
        return "groups.getCallbackServer";
    case METHODS::GROUPS_GET_CALLBACK_SETTINGS:
        return "groups.getCallbackSettings";
    case METHODS::GROUPS_GET_CATALOG:
        return "groups.getCatalog";
    case METHODS::GROUPS_GET_CATALOG_INFO:
        return "groups.getCatalogInfo";
    case METHODS::GROUPS_GET_INVITED_USERS:
        return "groups.getInvitedUsers";
    case METHODS::GROUPS_GET_INVITIES:
        return "groups.getInvities";
    case METHODS::GROUPS_GET_LONG_POLL_SERVER:
        return "groups.getLongPollServer";
    case METHODS::GROUPS_GET_LONG_POLL_SETTINGS:
        return "groups.getLongPollSettings";
    case METHODS::GROUPS_GET_MEMBERS:
        return "groups.getMembers";
    case METHODS::GROUPS_GET_ONLINE_STATUS:
        return "groups.getOnlineStatus";
    case METHODS::GROUPS_GET_REQUESTS:
        return "groups.getRequests";
    case METHODS::GROUPS_GET_SETTINGS:
        return "groups.getSettings";
    case METHODS::GROUPS_GET_TAG_LIST:
        return "groups.getTagList";
    case METHODS::GROUPS_GET_TOKEN_PERMISSIONS:
        return "groups.getTokenPermissions";
    case METHODS::GROUPS_INVITE:
        return "groups.invite";
    case METHODS::GROUPS_IS_MEMBER:
        return "groups.isMember";
    case METHODS::GROUPS_JOIN:
        return "groups.join";
    case METHODS::GROUPS_LEAVE:
        return "groups.leave";
    case METHODS::GROUPS_REMOVE_USER:
        return "groups.removeUser";
    case METHODS::GROUPS_REORDER_LINK:
        return "groups.reorderLink";
    case METHODS::GROUPS_SEARCH:
        return "groups.search";
    case METHODS::GROUPS_SET_CALLBACK_SETTINGS:
        return "groups.setCallbackSettings";
    case METHODS::GROUPS_SET_LONG_POLL_SETTINGS:
        return "groups.setLongPollSettings";
    case METHODS::GROUPS_SET_SETTINGS:
        return "groups.setSettings";
    case METHODS::GROUPS_SET_USER_NOTE:
        return "groups.setUserNote";
    case METHODS::GROUPS_TAG_ADD:
        return "groups.tagAdd";
    case METHODS::GROUPS_TAG_BIND:
        return "groups.tagBind";
    case METHODS::GROUPS_TAG_DELETE:
        return "groups.tagDelete";
    case METHODS::GROUPS_TAG_UPDATE:
        return "groups.tagUpdate";
    case METHODS::GROUPS_UNBAN:
        return "groups.unban";
    case METHODS::LEADFORMS_CREATE:
        return "leadForms.create";
    case METHODS::LEADFORMS_DELETE:
        return "leadForms.delete";
    case METHODS::LEADFORMS_GET:
        return "leadForms.get";
    case METHODS::LEADFORMS_GET_LEADS:
        return "leadForms.getLeads";
    case METHODS::LEADFORMS_GET_UPLOAD_URL:
        return "leadForms.getUploadURL";
    case METHODS::LEADFORMS_LIST:
        return "leadForms.list";
    case METHODS::LEADFORMS_UPDATE:
        return "leadForms.update";
    case METHODS::LIKES_ADD:
        return "likes.add";
    case METHODS::LIKES_DELETE:
        return "likes.delete";
    case METHODS::LIKES_GET_LIST:
        return "likes.getList";
    case METHODS::LIKES_IS_LIKED:
        return "likes.isLiked";
    case METHODS::MARKET_ADD:
        return "market.add";
    case METHODS::MARKET_ADD_ALBUM:
        return "market.addAlbum";
    case METHODS::MARKET_ADD_TO_ALBUM:
        return "market.addToAlbum";
    case METHODS::MARKET_CREATE_COMMENT:
        return "market.createComment";
    case METHODS::MARKET_DELETE:
        return "market.delete";
    case METHODS::MARKET_DELETE_ALBUM:
        return "market.deleteAlbum";
    case METHODS::MARKET_DELETE_COMMENT:
        return "market.deleteComment";
    case METHODS::MARKET_EDIT:
        return "market.edit";
    case METHODS::MARKET_EDIT_ALBUM:
        return "market.editAlbum";
    case METHODS::MARKET_EDIT_COMMENT:
        return "market.editComment";
    case METHODS::MARKET_EDIT_ORDER:
        return "market.editOrder";
    case METHODS::MARKET_GET:
        return "market.get";
    case METHODS::MARKET_GET_ALBUM_BY_ID:
        return "market.getAlbumById";
    case METHODS::MARKET_GET_ALBUMS:
        return "market.getAlbums";
    case METHODS::MARKET_GET_BY_ID:
        return "market.getById";
    case METHODS::MARKET_GET_CATEGORIES:
        return "market.getCategories";
    case METHODS::MARKET_GET_COMMENTS:
        return "market.getComments";
    case METHODS::MARKET_GET_GROUP_ORDERS:
        return "market.getGroupOrders";
    case METHODS::MARKET_GET_ORDER_BY_ID:
        return "market.getOrderById";
    case METHODS::MARKET_GET_ORDER_ITEMS:
        return "market.getOrderItems";
    case METHODS::MARKET_GET_ORDERS:
        return "market.getOrders";
    case METHODS::MARKET_REMOVE_FROM_ALBUM:
        return "market.removeFromAlbum";
    case METHODS::MARKET_REORDER_ALBUMS:
        return "market.reorderAlbums";
    case METHODS::MARKET_REORDER_ITEMS:
        return "market.reorderItems";
    case METHODS::MARKET_REPORT:
        return "market.report";
    case METHODS::MARKET_REPORT_COMMENT:
        return "market.reportComment";
    case METHODS::MARKET_RESTORE:
        return "market.restore";
    case METHODS::MARKET_RESTORE_COMMENT:
        return "market.restoreComment";
    case METHODS::MARKET_SEARCH:
        return "market.search";
    case METHODS::MESSAGES_ADD_CHAT_USER:
        return "messages.addChatUser";
    case METHODS::MESSAGES_ALLOW_MESSAGES_FROM_GROUP:
        return "messages.allowMessagesFromGroup";
    case METHODS::MESSAGES_CREATE_CHAT:
        return "messages.createChat";
    case METHODS::MESSAGES_DELETE:
        return "messages.delete";
    case METHODS::MESSAGES_DELETE_CHAT_PHOTO:
        return "messages.deleteChatPhoto";
    case METHODS::MESSAGES_DELETE_CONVERSATION:
        return "messages.deleteConversation";
    case METHODS::MESSAGES_DENY_MESSAGES_FROM_GROUP:
        return "messages.denyMessagesFromGroup";
    case METHODS::MESSAGES_EDIT:
        return "messages.edit";
    case METHODS::MESSAGES_EDIT_CHAT:
        return "messages.editChat";
    case METHODS::MESSAGES_GET_BY_CONVERSATION_MESSAGE_ID:
        return "messages.getByConversationMessageId";
    case METHODS::MESSAGES_GET_BY_ID:
        return "messages.getById";
    case METHODS::MESSAGES_GET_CHAT:
        return "messages.getChat";
    case METHODS::MESSAGES_GET_CHAT_PREVIEW:
        return "messages.getChatPreview";
    case METHODS::MESSAGES_GET_CONVERSATIONS:
        return "messages.getConversations";
    case METHODS::MESSAGES_GET_CONVERSATION_BY_ID:
        return "messages.getConversationById";
    case METHODS::MESSAGES_GET_HISTORY:
        return "messages.getHistory";
    case METHODS::MESSAGES_GET_HISTORY_ATTACHMENTS:
        return "messages.getHistoryAttachments";
    case METHODS::MESSAGES_GET_IMPORTANT_MESSAGES:
        return "messages.getImportantMessages";
    case METHODS::MESSAGES_GET_INVITE_LINK:
        return "messages.getInviteLink";
    case METHODS::MESSAGES_GET_LAST_ACTIVITY:
        return "messages.getLastActivity";
    case METHODS::MESSAGES_GET_LONG_POLL_HISTORY:
        return "messages.getLongPollHistory";
    case METHODS::MESSAGES_GET_LONG_POLL_SERVER:
        return "messages.getLongPollServer";
    case METHODS::MESSAGES_IS_MESSAGES_FROM_GROUP_ALLOWED:
        return "messages.isMessagesFromGroupAllowed";
    case METHODS::MESSAGES_JOIN_CHAT_BY_INVITE_LINK:
        return "messages.joinChatByInviteLink";
    case METHODS::MESSAGES_MARK_AS_ANSWERED_CONVERSATION:
        return "messages.markAsAnsweredConversation";
    case METHODS::MESSAGES_MARK_AS_IMPORTANT:
        return "messages.markAsImportant";
    case METHODS::MESSAGES_MARK_AS_IMPORTANT_CONVERSATION:
        return "messages.markAsImportantConversation";
    case METHODS::MESSAGES_MARK_AS_READ:
        return "messages.markAsRead";
    case METHODS::MESSAGES_PIN:
        return "messages.pin";
    case METHODS::MESSAGES_REMOVE_CHAT_USER:
        return "messages.removeChatUser";
    case METHODS::MESSAGES_RESTORE:
        return "messages.restore";
    case METHODS::MESSAGES_SEARCH:
        return "messages.search";
    case METHODS::MESSAGES_SEARCH_CONVERSATIONS:
        return "messages.searchConversations";
    case METHODS::MESSAGES_SEND:
        return "messages.send";
    case METHODS::MESSAGES_SEND_MESSAGE_EVENT_ANSWER:
        return "messages.sendMessageEventAnswer";
    case METHODS::MESSAGES_SET_ACTIVITY:
        return "messages.setActivity";
    case METHODS::MESSAGES_SET_CHAT_PHOTO:
        return "messages.setChatPhoto";
    case METHODS::MESSAGES_UNPIN:
        return "messages.unpin";
    case METHODS::NEWSFEED_ADD_BAN:
        return "newsfeed.addBan";
    case METHODS::NEWSFEED_DELETE_BAN:
        return "newsfeed.deleteBan";
    case METHODS::NEWSFEED_DELETE_LIST:
        return "newsfeed.deleteList";
    case METHODS::NEWSFEED_GET:
        return "newsfeed.get";
    case METHODS::NEWSFEED_GET_BANNED:
        return "newsfeed.getBanned";
    case METHODS::NEWSFEED_GET_COMMENTS:
        return "newsfeed.getComments";
    case METHODS::NEWSFEED_GET_LISTS:
        return "newsfeed.getLists";
    case METHODS::NEWSFEED_GET_MENTIONS:
        return "newsfeed.getMentions";
    case METHODS::NEWSFEED_GET_RECOMMENDED:
        return "newsfeed.getRecommended";
    case METHODS::NEWSFEED_GET_SUGGESTED_SOURCES:
        return "newsfeed.getSuggestedSources";
    case METHODS::NEWSFEED_IGNORE_ITEM:
        return "newsfeed.ignoreItem";
    case METHODS::NEWSFEED_SAVE_LIST:
        return "newsfeed.saveList";
    case METHODS::NEWSFEED_SEARCH:
        return "newsfeed.search";
    case METHODS::NEWSFEED_UNIGNORED_ITEM:
        return "newsfeed.unignoredItem";
    case METHODS::NEWSFEED_UNSUBSCRIBE:
        return "newsfeed.unsubscribe";
    case METHODS::NOTES_ADD:
        return "notes.add";
    case METHODS::NOTES_CREATE_COMMENT:
        return "notes.createComment";
    case METHODS::NOTES_DELETE:
        return "notes.delete";
    case METHODS::NOTES_DELETE_COMMENT:
        return "notes.deleteComment";
    case METHODS::NOTES_EDIT:
        return "notes.edit";
    case METHODS::NOTES_EDIT_COMMENT:
        return "notes.editComment";
    case METHODS::NOTES_GET:
        return "notes.get";
    case METHODS::NOTES_GET_BY_ID:
        return "notes.getById";
    case METHODS::NOTES_GET_COMMENTS:
        return "notes.getComments";
    case METHODS::NOTES_RESTORE_COMMENTS:
        return "notes.restoreComments";
    case METHODS::NOTIFICATIONS_GET:
        return "notifications.get";
    case METHODS::NOTIFICATIONS_MARK_AS_VIEWED:
        return "notifications.markAsViewed";
    case METHODS::PAGES_GET:
        return "pages.get";
    case METHODS::PAGES_GET_HISTORY:
        return "pages.getHistory";
    case METHODS::PAGES_GET_TITLES:
        return "pages.getTitles";
    case METHODS::PAGES_GET_VERSION:
        return "pages.getVersion";
    case METHODS::PAGES_PARSE_WIKI:
        return "pages.parseWiki";
    case METHODS::PAGES_SAVE:
        return "pages.save";
    case METHODS::PAGES_SAVE_ACCESS:
        return "pages.saveAccess";
    case METHODS::PHOTOS_CONFIRM_TAG:
        return "photos.confirmTag";
    case METHODS::PHOTOS_COPY:
        return "photos.copy";
    case METHODS::PHOTOS_CREATE_ALBUM:
        return "photos.createAlbum";
    case METHODS::PHOTOS_CREATE_COMMENT:
        return "photos.createComment";
    case METHODS::PHOTOS_DELETE:
        return "photos.delete";
    case METHODS::PHOTOS_DELETE_ALBUM:
        return "photos.deleteAlbum";
    case METHODS::PHOTOS_DELETE_COMMENT:
        return "photos.deleteComment";
    case METHODS::PHOTOS_EDIT:
        return "photos.edit";
    case METHODS::PHOTOS_EDIT_ALBUM:
        return "photos.editAlbum";
    case METHODS::PHOTOS_EDIT_COMMENT:
        return "photos.editComment";
    case METHODS::PHOTOS_GET:
        return "photos.get";
    case METHODS::PHOTOS_GET_ALBUM:
        return "photos.getAlbum";
    case METHODS::PHOTOS_GET_ALBUMS_COUNT:
        return "photos.getAlbumsCount";
    case METHODS::PHOTOS_GET_ALL:
        return "photos.getAll";
    case METHODS::PHOTOS_GET_ALL_COMMENTS:
        return "photos.getAllComments";
    case METHODS::PHOTOS_GET_BY_ID:
        return "photos.getById";
    case METHODS::PHOTOS_GET_CHAT_UPLOAD_SERVER:
        return "photos.getChatUploadServer";
    case METHODS::PHOTOS_GET_COMMENTS:
        return "photos.getComments";
    case METHODS::PHOTOS_GET_MARKET_ALBUM_UPLOAD_SERVER:
        return "photos.getMarketAlbumUploadServer";
    case METHODS::PHOTOS_GET_MARKET_UPLOAD_SERVER:
        return "photos.getMarketUploadServer";
    case METHODS::PHOTOS_GET_MESSAGES_UPLOAD_SERVER:
        return "photos.getMessagesUploadServer";
    case METHODS::PHOTOS_GET_NEW_TAGS:
        return "photos.getNewTags";
    case METHODS::PHOTOS_GET_OWNER_COVER_PHOTO_UPLOAD_SERVER:
        return "photos.getOnwerCoverPhotoUploadServer";
    case METHODS::PHOTOS_GET_OWNER_PHOTO_UPLOAD_SERVER:
        return "photos.getOnwerPhotoUploadServer";
    case METHODS::PHOTOS_GET_TAGS:
        return "photos.getTags";
    case METHODS::PHOTOS_GET_UPLOAD_SERVER:
        return "photos.getUploadServer";
    case METHODS::PHOTOS_GET_USER_PHOTOS:
        return "photos.getUserPhotos";
    case METHODS::PHOTOS_GET_WALL_UPLOAD_SERVER:
        return "photos.getWallUploadServer";
    case METHODS::PHOTOS_MAKE_COVER:
        return "photos.makeCover";
    case METHODS::PHOTOS_MOVE:
        return "photos.move";
    case METHODS::PHOTOS_PUT_TAG:
        return "photos.putTag";
    case METHODS::PHOTOS_REMOVE_TAG:
        return "photos.removeTag";
    case METHODS::PHOTOS_REORDER_ALBUMS:
        return "photos.reorderAlbums";
    case METHODS::PHOTOS_REODER_PHOTOS:
        return "photos.reorderPhotos";
    case METHODS::PHOTOS_REPORT:
        return "photos.report";
    case METHODS::PHOTOS_REPORT_COMMENT:
        return "photos.reportComment";
    case METHODS::PHOTOS_RESTORE:
        return "photos.restore";
    case METHODS::PHOTOS_RESTORE_COMMENT:
        return "photos.restoreComment";
    case METHODS::PHOTOS_SAVE:
        return "photos.save";
    case METHODS::PHOTOS_SAVE_MARKET_ALBUM_PHOTO:
        return "photos.saveMarketAlbumPhoto";
    case METHODS::PHOTOS_SAVE_MARKET_PHOTO:
        return "photos.saveMarketPhoto";
    case METHODS::PHOTOS_SAVE_MESSAGES_PHOTO:
        return "photos.saveMessagesPhoto";
    case METHODS::PHOTOS_SAVE_OWNER_COVER_PHOTO:
        return "photos.saveOnwerCoverPhoto";
    case METHODS::PHOTOS_SAVE_OWNER_PHOTO:
        return "photos.saveOwnerPhoto";
    case METHODS::PHOTOS_SAVE_WALL_PHOTO:
        return "photos.saveWallPhoto";
    case METHODS::PHOTOS_SEARCH:
        return "photos.search";
    case METHODS::POLLS_ADD_VOTE:
        return "polls.addVote";
    case METHODS::POLLS_CREATE:
        return "polls.create";
    case METHODS::POLLS_DELETE_VOTE:
        return "polls.deleteVote";
    case METHODS::POLLS_EDIT:
        return "polls.edit";
    case METHODS::POLLS_GET_BACKGROUNDS:
        return "polls.getBackgrounds";
    case METHODS::POLLS_GET_BY_ID:
        return "polls.getById";
    case METHODS::POLLS_GET_PHOTO_UPLOAD_SERVER:
        return "polls.getPhotoUploadServer";
    case METHODS::POLLS_GET_VOTES:
        return "polls.getVotes";
    case METHODS::POLLS_SAVE_PHOTO:
        return "polls.savePhoto";
    case METHODS::PRETTYCARDS_CREATE:
        return "prettycards.create";
    case METHODS::PRETTYCARDS_DELETE:
        return "prettycards.delete";
    case METHODS::PRETTYCARDS_EDIT:
        return "prettycards.edit";
    case METHODS::PRETTYCARDS_GET:
        return "prettycards.get";
    case METHODS::PRETTYCARDS_GET_BY_ID:
        return "prettycards.getById";
    case METHODS::PRETTYCARDS_GET_UPLOAD_URL:
        return "prettycards.getUploadURL";
    case METHODS::SEARCH_GET_HINTS:
        return "prettycards.getHints";
    case METHODS::STATS_GET:
        return "stats.get";
    case METHODS::STATS_GET_POST_REACH:
        return "stats.getPostReach";
    case METHODS::STATS_TRACK_VISITOR:
        return "stats.trackVisitor";
    case METHODS::STATUS_GET:
        return "status.get";
    case METHODS::STATUS_SET:
        return "status.set";
    case METHODS::STORIES_BAN_OWNER:
        return "stories.banOnwer";
    case METHODS::STORIES_DELETE:
        return "stories.delete";
    case METHODS::STORIES_GET:
        return "stories.get";
    case METHODS::STORIES_GET_BANNED:
        return "stories.getBanned";
    case METHODS::STORIES_GET_BY_ID:
        return "stories.getById";
    case METHODS::STORIES_GET_PHOTO_UPLOAD_SERVER:
        return "stories.getPhotoUploadServer";
    case METHODS::STORIES_GET_REPLIES:
        return "stories.getReplies";
    case METHODS::STORIES_GET_STATS:
        return "stories.getStats";
    case METHODS::STORIES_GET_VIDEO_UPLOAD_SERVER:
        return "stories.getVideoUploadServer";
    case METHODS::STORIES_GET_VIEWERS:
        return "stories.getViewers";
    case METHODS::STORIES_HIDE_ALL_REPLIES:
        return "stories.hideAllReplies";
    case METHODS::STORIES_HIDE_REPLY:
        return "stories.hideReply";
    case METHODS::STORIES_SAVE:
        return "stories.save";
    case METHODS::STORIES_SEARCH:
        return "stories.search";
    case METHODS::STORIES_UNBAN_OWNER:
        return "stories.unbanOwner";
    case METHODS::USERS_GET:
        return "users.get";
    case METHODS::USERS_GET_FOLLOWERS:
        return "users.getFollowers";
    case METHODS::USERS_GET_SUBSCRIPTIONS:
        return "users.getSubscriptions";
    case METHODS::USERS_REPORT:
        return "users.report";
    case METHODS::USERS_SEARCH:
        return "users.search";
    case METHODS::UTILS_CHECK_LINK:
        return "utils.checkLink";
    case METHODS::VIDEO_ADD:
        return "video.add";
    case METHODS::VIDEO_ADD_ALBUM:
        return "video.addAlbum";
    case METHODS::VIDEO_ADD_TO_ALBUM:
        return "video.addToAlbum";
    case METHODS::VIDEO_CREATE_COMMENT:
        return "video.createComment";
    case METHODS::VIDEO_DELETE:
        return "video.delete";
    case METHODS::VIDEO_DELETE_ALBUM:
        return "video.deleteAlbum";
    case METHODS::VIDEO_DELETE_COMMENT:
        return "video.deleteComment";
    case METHODS::VIDEO_EDIT:
        return "video.edit";
    case METHODS::VIDEO_EDIT_ALBUM:
        return "video.editAlbum";
    case METHODS::VIDEO_EDIT_COMMENT:
        return "video.editComment";
    case METHODS::VIDEO_GET:
        return "video.get";
    case METHODS::VIDEO_GET_ALBUM_BY_ID:
        return "video.getAlbumById";
    case METHODS::VIDEO_GET_ALBUMS:
        return "video.getAlbums";
    case METHODS::VIDEO_GET_ALBUMS_BY_VIDEO:
        return "video.getAlbumsByVideo";
    case METHODS::VIDEO_GET_COMMENTS:
        return "video.getComments";
    case METHODS::VIDEO_REMOVE_FROM_ALBUMS:
        return "video.removeFromAlbums";
    case METHODS::VIDEO_REORDER_ALBUMS:
        return "video.reorderAlbums";
    case METHODS::VIDEO_REORDER_VIDEOS:
        return "video.reorderVideos";
    case METHODS::VIDEO_REPORT:
        return "video.report";
    case METHODS::VIDEO_REPORT_COMMENT:
        return "video.reportComment";
    case METHODS::VIDEO_RESTORE:
        return "video.restore";
    case METHODS::VIDEO_RESTORE_COMMENT:
        return "video.restoreComment";
    case METHODS::VIDEO_SAVE:
        return "video.save";
    case METHODS::VIDEO_SEARCH:
        return "video.search";
    case METHODS::WALL_CHECK_COPYRIGHT_LINK:
        return "wall.checkCopyrightLink";
    case METHODS::WALL_CLOSE_COMMENTS:
        return "wall.closeComments";
    case METHODS::WALL_CREATE_COMMENT:
        return "wall.createComment";
    case METHODS::WALL_DELETE:
        return "wall.delete";
    case METHODS::WALL_DELETE_COMMENT:
        return "wall.deleteComment";
    case METHODS::WALL_EDIT:
        return "wall.edit";
    case METHODS::WALL_EDIT_ADS_STEALTH:
        return "wall.editAbsStealth";
    case METHODS::WALL_EDIT_COMMENT:
        return "wall.editComment";
    case METHODS::WALL_GET:
        return "wall.get";
    case METHODS::WALL_GET_BY_ID:
        return "wall.getById";
    case METHODS::WALL_GET_COMMENT:
        return "wall.getComment";
    case METHODS::WALL_GET_REPOSTS:
        return "wall.getReposts";
    case METHODS::WALL_OPEN_COMMENTS:
        return "wall.openComments";
    case METHODS::WALL_PIN:
        return "wall.pin";
    case METHODS::WALL_POST:
        return "wall.post";
    case METHODS::WALL_POST_ADS_STEALTH:
        return "wall.postAbsStealth";
    case METHODS::WALL_REPORT_COMMENT:
        return "wall.reportComment";
    case METHODS::WALL_REPORT_POST:
        return "wall.reportPost";
    case METHODS::WALL_REPOST:
        return "wall.repost";
    case METHODS::WALL_RESTORE:
        return "wall.restore";
    case METHODS::WALL_RESTORE_COMMENT:
        return "wall.restoreComment";
    case METHODS::WALL_SEARCH:
        return "wall.search";
    case METHODS::WALL_UNPIN:
        return "wall.unpin";
    }

    return "";
}

} // namespace user

} // namespace base

} // namespace vk