#include "UserBase.hpp"

namespace vk
{

UserBase::UserBase(const std::string& appId, const std::string& appSecureKey)
    : ClientBase()
    , appId_(appId)
    , appSecureKey_(appSecureKey)
    , accessToken_("")
    , userId_("")
{
    if (appId_.empty() || appSecureKey_.empty()) { throw ex::EmptyArgumentException(); }
}

bool UserBase::Auth(std::string& login, std::string& password)
{
    if (login.empty() || password.empty()) { throw ex::EmptyArgumentException(); }
    if (IsAuthorized()) { connectedToLongPoll_ = false; }

    std::string scope;

    if (!scope_.empty()) {
        for (const auto& i : scope_) { scope += "," + i; }
        scope = scope.substr(0, scope.size() - 1);
    }

    json parametersData = {
        { "client_id", appId_ },
        { "grant_type", "password" },
        { "client_secret", appSecureKey_ },
        { "scope", scope },
        { "username", login },
        { "password", password }
    };

    json response = json::parse(Request::Send(AUTH_URL,
                                              ConvertParametersDataToURL(parametersData)));

    try {
        if (response.find("error") != response.end()) {
            auto errorTypeStr = response["error"].get<std::string>();
            VK_REQUEST_ERROR_TYPES errorType = GetRequestErrorType(errorTypeStr);

            if (errorType == VK_REQUEST_ERROR_TYPES::NEED_CAPTCHA) {
                std::cout << GetURLCaptcha(parametersData, response);

                std::string captchaKey;
                std::cin >> captchaKey;

                parametersData.push_back(
                    { "captcha_key", captchaKey });
                Request::Send(AUTH_URL, ConvertParametersDataToURL(parametersData));

                // TODO (#11): Add further processing
            }

            VALIDATION_TYPES validationType = GetValidationType(response.at("validation_type"));

            if (validationType == VALIDATION_TYPES::TWOFA_SMS || validationType == VALIDATION_TYPES::TWOFA_APP) {
                parametersData.push_back({ "2fa_supported", "1" });

                if (validationType == VALIDATION_TYPES::TWOFA_SMS) {
                    parametersData.push_back({ "force_sms", "1" });
                } else {
                    parametersData.push_back({ "2fa_app", "1" });
                }

                Request::Send(response.at("redirect_url").get<std::string>(), "");
                Request::Send(AUTH_URL, ConvertParametersDataToURL(parametersData));

                std::cout << "Enter code: ";

                std::string code;
                std::cin >> code;

                parametersData.push_back({ "code", code });
                response = json::parse(Request::Send(AUTH_URL, ConvertParametersDataToURL(parametersData)));

                accessToken_ = response.at("access_token").get<std::string>();
                userId_ = response.at("user_id").get<std::string>();
                connectedToLongPoll_ = true;
            }
        } else {
            accessToken_ = response.at("access_token").get<std::string>();
            userId_ = response.at("user_id").get<std::string>();
            connectedToLongPoll_ = true;
        }
    } catch (json::exception& exc) {
        std::cerr << "exception message: " << exc.what() << std::endl;
        std::cerr << "exception id: " << exc.id << std::endl;
    }

    return connectedToLongPoll_;
}

bool UserBase::Auth(const std::string& accessToken)
{
    if (accessToken.empty()) { throw ex::EmptyArgumentException(); }
    if (IsAuthorized()) { connectedToLongPoll_ = false; }

    json parametersData = {
        { "access_token", accessToken },
        { "v", API_VERSION }
    };

    const std::string method = MethodToString(METHODS::USERS_GET);
    const std::string url = API_URL + method;

    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(parametersData)));

    try {
        if (response.find("error") != response.end()) {
            for (const auto& data : response.at("response").items()) {
                userId_ = data.value().at("id").get<std::string>();
            }
            accessToken_ = accessToken;
            connectedToLongPoll_ = true;
        } else {
            userId_.clear();
            accessToken_.clear();
            connectedToLongPoll_ = false;
        }
    } catch (json::exception& exc) {
        std::cerr << "exception message: " << exc.what() << std::endl;
        std::cerr << "exception id: " << exc.id << std::endl;
    }

    return connectedToLongPoll_;
}

json UserBase::CheckValidationParameters(const json& parametersData)
{
    json cParametersData = parametersData;

    if (cParametersData.find("access_token") == cParametersData.end())
        cParametersData.push_back({ "access_token", accessToken_ });

    if (cParametersData.find("v") == cParametersData.end())
        cParametersData.push_back({ "v", API_VERSION });

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

json UserBase::SendRequest(const METHODS method, const json& parametersData)
{
    if (!IsAuthorized()) { throw ex::NotConnectedException(); }

    std::string methodStr = MethodToString(method);
    std::string url = API_URL + methodStr;

    json pData = CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(pData)));

    return response;
}

json UserBase::SendRequest(const std::string& method, const json& parametersData)
{
    if (!IsAuthorized()) { throw ex::NotConnectedException(); }
    if (method.empty()) { throw ex::EmptyArgumentException(); }

    std::string url = API_URL + method;

    json pData = CheckValidationParameters(parametersData);
    json response = json::parse(Request::Send(url, ConvertParametersDataToURL(pData)));

    return response;
}

std::string UserBase::MethodToString(const METHODS method)
{
    switch (method) {
    case METHODS::ACCOUNT_BAN:
        return "account.Ban";
        break;
    case METHODS::ACCOUNT_CHANGE_PASSWORD:
        return "account.changePassword";
        break;
    case METHODS::ACCOUNT_GET_ACTIVE_OFFERS:
        return "account.getActiveOffers";
        break;
    case METHODS::ACCOUNT_GET_APP_PERMISSIONS:
        return "account.getAppPermissions";
        break;
    case METHODS::ACCOUNT_GET_BANNED:
        return "account.getBanned";
        break;
    case METHODS::ACCOUNT_GET_COUNTERS:
        return "account.getCounters";
        break;
    case METHODS::ACCOUNT_GET_INFO:
        return "account.getInfo";
        break;
    case METHODS::ACCOUNT_GET_PROFILE_INFO:
        return "account.getProfileInfo";
        break;
    case METHODS::ACCOUNT_GET_PUSH_SETTINGS:
        return "account.getPushSettings";
        break;
    case METHODS::ACCOUNT_REGISTER_DEVICE:
        return "account.registerDevice";
        break;
    case METHODS::ACCOUNT_SAVE_PROFILE_INFO:
        return "account.saveProfileInfo";
        break;
    case METHODS::ACCOUNT_SET_INFO:
        return "account.setInfo";
        break;
    case METHODS::ACCOUNT_SET_PUSH_SETTINGS:
        return "account.setPushSettings";
        break;
    case METHODS::ACCOUNT_UNBAN:
        return "account.unban";
        break;
    case METHODS::ACCOUNT_UNREGISTER_DEVICE:
        return "account.unregisterDevice";
        break;
    case METHODS::BOARD_ADD_TOPIC:
        return "board.addTopic";
        break;
    case METHODS::BOARD_CLOSE_TOPIC:
        return "board.closeTopic";
        break;
    case METHODS::BOARD_CREATE_COMMENT:
        return "board.createComment";
        break;
    case METHODS::BOARD_DELETE_COMMENT:
        return "board.deleteComment";
        break;
    case METHODS::BOARD_DELETE_TOPIC:
        return "board.deleteTopic";
        break;
    case METHODS::BOARD_EDIT_COMMENT:
        return "board.editComment";
        break;
    case METHODS::BOARD_EDIT_TOPIC:
        return "board.editTopic";
        break;
    case METHODS::BOARD_FIX_TOPIC:
        return "board.fixTopic";
        break;
    case METHODS::BOARD_GET_COMMENTS:
        return "board.getComments";
        break;
    case METHODS::BOARD_GET_TOPICS:
        return "board.getTopics";
        break;
    case METHODS::BOARD_OPEN_TOPIC:
        return "board.openTopic";
        break;
    case METHODS::BOARD_RESTORE_COMMENT:
        return "board.restoreComment";
        break;
    case METHODS::BOARD_UNFIX_TOPIC:
        return "board.unfixTopic";
        break;
    case METHODS::DATABASE_GET_CHAIRS:
        return "database.getChairs";
        break;
    case METHODS::DATABASE_GET_CITIES:
        return "database.getCities";
        break;
    case METHODS::DATABASE_GET_CITIES_BY_ID:
        return "database.getCitiesById";
        break;
    case METHODS::DATABASE_GET_COUNTRIES:
        return "database.getCounties";
        break;
    case METHODS::DATABASE_GET_FACULTIES:
        return "database.getFaculties";
        break;
    case METHODS::DATABASE_GET_METRO_STATIONS:
        return "database.getMetroStations";
        break;
    case METHODS::DATABASE_GET_REGIONS:
        return "database.getRegions";
        break;
    case METHODS::DATABASE_GET_SCHOOL_CLASSES:
        return "database.getSchoolClasses";
        break;
    case METHODS::DATABASE_GET_SCHOOLS:
        return "database.getSchools";
        break;
    case METHODS::DATABASE_GET_UNIVERSITIES:
        return "database.getUniversities";
        break;
    case METHODS::DOCS_ADD:
        return "docs.add";
        break;
    case METHODS::DOCS_DELETE:
        return "docs.delete";
        break;
    case METHODS::DOCS_EDIT:
        return "docs.edit";
        break;
    case METHODS::DOCS_GET:
        return "docs.get";
        break;
    case METHODS::DOCS_GET_BY_ID:
        return "docs.getById";
        break;
    case METHODS::DOCS_GET_MESSAGES_UPLOAD_SERVER:
        return "docs.getMessagesUploadServer";
        break;
    case METHODS::DOCS_GET_TYPES:
        return "docs.getTypes";
        break;
    case METHODS::DOCS_GET_UPLOAD_SERVER:
        return "docs.getUploadServer";
        break;
    case METHODS::DOCS_GET_WALL_UPLOAD_SERVER:
        return "docs.getWallUploadServer";
        break;
    case METHODS::DOCS_SAVE:
        return "docs.save";
        break;
    case METHODS::DOCS_SEARCH:
        return "docs.search";
        break;
    case METHODS::EXECUTE:
        return "execute";
        break;
    case METHODS::FAVE_ADD_ARTICLE:
        return "fave.addArticle";
        break;
    case METHODS::FAVE_ADD_LINK:
        return "fave.addLink";
        break;
    case METHODS::FAVE_ADD_PAGE:
        return "fave.addPage";
        break;
    case METHODS::FAVE_ADD_POST:
        return "fave.addPost";
        break;
    case METHODS::FAVE_ADD_PRODUCT:
        return "fave.addProduct";
        break;
    case METHODS::FAVE_ADD_TAG:
        return "fave.addTag";
        break;
    case METHODS::FAVE_ADD_VIDEO:
        return "fave.addVideo";
        break;
    case METHODS::FAVE_EDIT_TAG:
        return "fave.editTag";
        break;
    case METHODS::FAVE_GET:
        return "fave.get";
        break;
    case METHODS::FAVE_GET_PAGES:
        return "fave.getPages";
        break;
    case METHODS::FAVE_GET_TAGS:
        return "fave.getTags";
        break;
    case METHODS::FAVE_MARK_SEEN:
        return "fave.markSeen";
        break;
    case METHODS::FAVE_REMOVE_ARTICLE:
        return "fave.removeArticle";
        break;
    case METHODS::FAVE_REMOVE_LINK:
        return "fave.removeLink";
        break;
    case METHODS::FAVE_REMOVE_PAGE:
        return "fave.removePage";
        break;
    case METHODS::FAVE_REMOVE_POST:
        return "fave.removePost";
        break;
    case METHODS::FAVE_REMOVE_PRODUCT:
        return "fave.removeProduct";
        break;
    case METHODS::FAVE_REMOVE_TAG:
        return "fave.removeTag";
        break;
    case METHODS::FAVE_REMOVE_VIDEO:
        return "fave.removeVideo";
        break;
    case METHODS::FAVE_REORDER_TAGS:
        return "fave.reorderTags";
        break;
    case METHODS::FAVE_SET_PAGE_TAGS:
        return "fave.setPageTags";
        break;
    case METHODS::FAVE_SET_TAGS:
        return "fave.setTags";
        break;
    case METHODS::FAVE_TRACK_PAGE_INTERACTION:
        return "fave.trackPageInteraction";
        break;
    case METHODS::FRIENDS_ADD:
        return "friends.add";
        break;
    case METHODS::FRIENDS_ADD_LIST:
        return "friends.addList";
        break;
    case METHODS::FRIENDS_ARE_FRIENDS:
        return "friends.areFriends";
        break;
    case METHODS::FRIENDS_DELETE:
        return "friends.delete";
        break;
    case METHODS::FRIENDS_DELETE_ALL_REQUESTS:
        return "friends.deleteAllRequests";
        break;
    case METHODS::FRIENDS_DELETE_LIST:
        return "friends.deleteList";
        break;
    case METHODS::FRIENDS_EDIT:
        return "friends.edit";
        break;
    case METHODS::FRIENDS_EDIT_LIST:
        return "friends.editList";
        break;
    case METHODS::FRIENDS_GET:
        return "friends.get";
        break;
    case METHODS::FRIENDS_GET_APP_USERS:
        return "friends.getAppUsers";
        break;
    case METHODS::FRIENDS_GET_BY_PHONES:
        return "friends.getByPhones";
        break;
    case METHODS::FRIENDS_GET_LISTS:
        return "friends.getLists";
        break;
    case METHODS::FRIENDS_GET_MUTUAL:
        return "friends.getMutual";
        break;
    case METHODS::FRIENDS_GET_ONLINE:
        return "friends.getOnline";
        break;
    case METHODS::FRIENDS_GET_RECENT:
        return "friends.getRecent";
        break;
    case METHODS::FRIENDS_GET_REQUESTS:
        return "friends.getRequests";
        break;
    case METHODS::FRIENDS_GET_SUGGESTIONS:
        return "friends.getSuggestions";
        break;
    case METHODS::FRIENDS_SEARCH:
        return "friends.search";
        break;
    case METHODS::GIFTS_GET:
        return "gifts.get";
        break;
    case METHODS::GROUPS_ADD_ADDRESS:
        return "groups.addAddress";
        break;
    case METHODS::GROUPS_ADD_CALLBACK_SERVER:
        return "groups.addCallbackServer";
        break;
    case METHODS::GROUPS_ADD_LINK:
        return "groups.addLink";
        break;
    case METHODS::GROUPS_APPROVE_REQUEST:
        return "groups.approveRequest";
        break;
    case METHODS::GROUPS_BAN:
        return "groups.ban";
        break;
    case METHODS::GROUPS_CREATE:
        return "groups.create";
        break;
    case METHODS::GROUPS_DELETE_ADDRESS:
        return "groups.deleteAddress";
        break;
    case METHODS::GROUPS_DELETE_CALLBACK_SERVER:
        return "groups.deleteCallbackServer";
        break;
    case METHODS::GROUPS_DELETE_LINK:
        return "groups.deleteLink";
        break;
    case METHODS::GROUPS_DISABLE_ONLINE:
        return "groups.disableOnline";
        break;
    case METHODS::GROUPS_EDIT:
        return "groups.edit";
        break;
    case METHODS::GROUPS_EDIT_ADDRESS:
        return "groups.editAddress";
        break;
    case METHODS::GROUPS_EDIT_CALLBACK_SERVER:
        return "groups.editCallbackServer";
        break;
    case METHODS::GROUPS_EDIT_LINK:
        return "groups.editLink";
        break;
    case METHODS::GROUPS_EDIT_MANAGER:
        return "groups.editManager";
        break;
    case METHODS::GROUPS_ENABLE_ONLINE:
        return "groups.enableOnline";
        break;
    case METHODS::GROUPS_GET:
        return "groups.get";
        break;
    case METHODS::GROUPS_GET_ADDRESSES:
        return "groups.getAddresses";
        break;
    case METHODS::GROUPS_GET_BANNED:
        return "groups.getBanned";
        break;
    case METHODS::GROUPS_GET_BY_ID:
        return "groups.getById";
        break;
    case METHODS::GROUPS_GET_CALLBACK_CONFIRMATION_CODE:
        return "groups.getCallbackConfirmationCode";
        break;
    case METHODS::GROUPS_GET_CALLBACK_SERVER:
        return "groups.getCallbackServer";
        break;
    case METHODS::GROUPS_GET_CALLBACK_SETTINGS:
        return "groups.getCallbackSettings";
        break;
    case METHODS::GROUPS_GET_CATALOG:
        return "groups.getCatalog";
        break;
    case METHODS::GROUPS_GET_CATALOG_INFO:
        return "groups.getCatalogInfo";
        break;
    case METHODS::GROUPS_GET_INVITED_USERS:
        return "groups.getInvitedUsers";
        break;
    case METHODS::GROUPS_GET_INVITIES:
        return "groups.getInvities";
        break;
    case METHODS::GROUPS_GET_LONG_POLL_SERVER:
        return "groups.getLongPollServer";
        break;
    case METHODS::GROUPS_GET_LONG_POLL_SETTINGS:
        return "groups.getLongPollSettings";
        break;
    case METHODS::GROUPS_GET_MEMBERS:
        return "groups.getMembers";
        break;
    case METHODS::GROUPS_GET_ONLINE_STATUS:
        return "groups.getOnlineStatus";
        break;
    case METHODS::GROUPS_GET_REQUESTS:
        return "groups.getRequests";
        break;
    case METHODS::GROUPS_GET_SETTINGS:
        return "groups.getSettings";
        break;
    case METHODS::GROUPS_GET_TAG_LIST:
        return "groups.getTagList";
        break;
    case METHODS::GROUPS_GET_TOKEN_PERMISSIONS:
        return "groups.getTokenPermissions";
        break;
    case METHODS::GROUPS_INVITE:
        return "groups.invite";
        break;
    case METHODS::GROUPS_IS_MEMBER:
        return "groups.isMember";
        break;
    case METHODS::GROUPS_JOIN:
        return "groups.join";
        break;
    case METHODS::GROUPS_LEAVE:
        return "groups.leave";
        break;
    case METHODS::GROUPS_REMOVE_USER:
        return "groups.removeUser";
        break;
    case METHODS::GROUPS_REORDER_LINK:
        return "groups.reorderLink";
        break;
    case METHODS::GROUPS_SEARCH:
        return "groups.search";
        break;
    case METHODS::GROUPS_SET_CALLBACK_SETTINGS:
        return "groups.setCallbackSettings";
        break;
    case METHODS::GROUPS_SET_LONG_POLL_SETTINGS:
        return "groups.setLongPollSettings";
        break;
    case METHODS::GROUPS_SET_SETTINGS:
        return "groups.setSettings";
        break;
    case METHODS::GROUPS_SET_USER_NOTE:
        return "groups.setUserNote";
        break;
    case METHODS::GROUPS_TAG_ADD:
        return "groups.tagAdd";
        break;
    case METHODS::GROUPS_TAG_BIND:
        return "groups.tagBind";
        break;
    case METHODS::GROUPS_TAG_DELETE:
        return "groups.tagDelete";
        break;
    case METHODS::GROUPS_TAG_UPDATE:
        return "groups.tagUpdate";
        break;
    case METHODS::GROUPS_UNBAN:
        return "groups.unban";
        break;
    case METHODS::LEADFORMS_CREATE:
        return "leadForms.create";
        break;
    case METHODS::LEADFORMS_DELETE:
        return "leadForms.delete";
        break;
    case METHODS::LEADFORMS_GET:
        return "leadForms.get";
        break;
    case METHODS::LEADFORMS_GET_LEADS:
        return "leadForms.getLeads";
        break;
    case METHODS::LEADFORMS_GET_UPLOAD_URL:
        return "leadForms.getUploadURL";
        break;
    case METHODS::LEADFORMS_LIST:
        return "leadForms.list";
        break;
    case METHODS::LEADFORMS_UPDATE:
        return "leadForms.update";
        break;
    case METHODS::LIKES_ADD:
        return "likes.add";
        break;
    case METHODS::LIKES_DELETE:
        return "likes.delete";
        break;
    case METHODS::LIKES_GET_LIST:
        return "likes.getList";
        break;
    case METHODS::LIKES_IS_LIKED:
        return "likes.isLiked";
        break;
    case METHODS::MARKET_ADD:
        return "market.add";
        break;
    case METHODS::MARKET_ADD_ALBUM:
        return "market.addAlbum";
        break;
    case METHODS::MARKET_ADD_TO_ALBUM:
        return "market.addToAlbum";
        break;
    case METHODS::MARKET_CREATE_COMMENT:
        return "market.createComment";
        break;
    case METHODS::MARKET_DELETE:
        return "market.delete";
        break;
    case METHODS::MARKET_DELETE_ALBUM:
        return "market.deleteAlbum";
        break;
    case METHODS::MARKET_DELETE_COMMENT:
        return "market.deleteComment";
        break;
    case METHODS::MARKET_EDIT:
        return "market.edit";
        break;
    case METHODS::MARKET_EDIT_ALBUM:
        return "market.editAlbum";
        break;
    case METHODS::MARKET_EDIT_COMMENT:
        return "market.editComment";
        break;
    case METHODS::MARKET_EDIT_ORDER:
        return "market.editOrder";
        break;
    case METHODS::MARKET_GET:
        return "market.get";
        break;
    case METHODS::MARKET_GET_ALBUM_BY_ID:
        return "market.getAlbumById";
        break;
    case METHODS::MARKET_GET_ALBUMS:
        return "market.getAlbums";
        break;
    case METHODS::MARKET_GET_BY_ID:
        return "market.getById";
        break;
    case METHODS::MARKET_GET_CATEGORIES:
        return "market.getCategories";
        break;
    case METHODS::MARKET_GET_COMMENTS:
        return "market.getComments";
        break;
    case METHODS::MARKET_GET_GROUP_ORDERS:
        return "market.getGroupOrders";
        break;
    case METHODS::MARKET_GET_ORDER_BY_ID:
        return "market.getOrderById";
        break;
    case METHODS::MARKET_GET_ORDER_ITEMS:
        return "market.getOrderItems";
        break;
    case METHODS::MARKET_GET_ORDERS:
        return "market.getOrders";
        break;
    case METHODS::MARKET_REMOVE_FROM_ALBUM:
        return "market.removeFromAlbum";
        break;
    case METHODS::MARKET_REORDER_ALBUMS:
        return "market.reorderAlbums";
        break;
    case METHODS::MARKET_REORDER_ITEMS:
        return "market.reorderItems";
        break;
    case METHODS::MARKET_REPORT:
        return "market.report";
        break;
    case METHODS::MARKET_REPORT_COMMENT:
        return "market.reportComment";
        break;
    case METHODS::MARKET_RESTORE:
        return "market.restore";
        break;
    case METHODS::MARKET_RESTORE_COMMENT:
        return "market.restoreComment";
        break;
    case METHODS::MARKET_SEARCH:
        return "market.search";
        break;
    case METHODS::MESSAGES_ADD_CHAT_USER:
        return "messages.addChatUser";
        break;
    case METHODS::MESSAGES_ALLOW_MESSAGES_FROM_GROUP:
        return "messages.allowMessagesFromGroup";
        break;
    case METHODS::MESSAGES_CREATE_CHAT:
        return "messages.createChat";
        break;
    case METHODS::MESSAGES_DELETE:
        return "messages.delete";
        break;
    case METHODS::MESSAGES_DELETE_CHAT_PHOTO:
        return "messages.deleteChatPhoto";
        break;
    case METHODS::MESSAGES_DELETE_CONVERSATION:
        return "messages.deleteConversation";
        break;
    case METHODS::MESSAGES_DENY_MESSAGES_FROM_GROUP:
        return "messages.denyMessagesFromGroup";
        break;
    case METHODS::MESSAGES_EDIT:
        return "messages.edit";
        break;
    case METHODS::MESSAGES_EDIT_CHAT:
        return "messages.editChat";
        break;
    case METHODS::MESSAGES_GET_BY_CONVERSATION_MESSAGE_ID:
        return "messages.getByConversationMessageId";
        break;
    case METHODS::MESSAGES_GET_BY_ID:
        return "messages.getById";
        break;
    case METHODS::MESSAGES_GET_CHAT:
        return "messages.getChat";
        break;
    case METHODS::MESSAGES_GET_CHAT_PREVIEW:
        return "messages.getChatPreview";
        break;
    case METHODS::MESSAGES_GET_CONVERSATIONS:
        return "messages.getConversations";
        break;
    case METHODS::MESSAGES_GET_CONVERSATION_BY_ID:
        return "messages.getConversationById";
        break;
    case METHODS::MESSAGES_GET_HISTORY:
        return "messages.getHistory";
        break;
    case METHODS::MESSAGES_GET_HISTORY_ATTACHMENTS:
        return "messages.getHistoryAttachments";
        break;
    case METHODS::MESSAGES_GET_IMPORTANT_MESSAGES:
        return "messages.getImportantMessages";
        break;
    case METHODS::MESSAGES_GET_INVITE_LINK:
        return "messages.getInviteLink";
        break;
    case METHODS::MESSAGES_GET_LAST_ACTIVITY:
        return "messages.getLastActivity";
        break;
    case METHODS::MESSAGES_GET_LONG_POLL_HISTORY:
        return "messages.getLongPollHistory";
        break;
    case METHODS::MESSAGES_GET_LONG_POLL_SERVER:
        return "messages.getLongPollServer";
        break;
    case METHODS::MESSAGES_IS_MESSAGES_FROM_GROUP_ALLOWED:
        return "messages.isMessagesFromGroupAllowed";
        break;
    case METHODS::MESSAGES_JOIN_CHAT_BY_INVITE_LINK:
        return "messages.joinChatByInviteLink";
        break;
    case METHODS::MESSAGES_MARK_AS_ANSWERED_CONVERSATION:
        return "messages.markAsAnsweredConversation";
        break;
    case METHODS::MESSAGES_MARK_AS_IMPORTANT:
        return "messages.markAsImportant";
        break;
    case METHODS::MESSAGES_MARK_AS_IMPORTANT_CONVERSATION:
        return "messages.markAsImportantConversation";
        break;
    case METHODS::MESSAGES_MARK_AS_READ:
        return "messages.markAsRead";
        break;
    case METHODS::MESSAGES_PIN:
        return "messages.pin";
        break;
    case METHODS::MESSAGES_REMOVE_CHAT_USER:
        return "messages.removeChatUser";
        break;
    case METHODS::MESSAGES_RESTORE:
        return "messages.restore";
        break;
    case METHODS::MESSAGES_SEARCH:
        return "messages.search";
        break;
    case METHODS::MESSAGES_SEARCH_CONVERSATIONS:
        return "messages.searchConversations";
        break;
    case METHODS::MESSAGES_SEND:
        return "messages.send";
        break;
    case METHODS::MESSAGES_SEND_MESSAGE_EVENT_ANSWER:
        return "messages.sendMessageEventAnswer";
        break;
    case METHODS::MESSAGES_SET_ACTIVITY:
        return "messages.setActivity";
        break;
    case METHODS::MESSAGES_SET_CHAT_PHOTO:
        return "messages.setChatPhoto";
        break;
    case METHODS::MESSAGES_UNPIN:
        return "messages.unpin";
        break;
    case METHODS::NEWSFEED_ADD_BAN:
        return "newsfeed.addBan";
        break;
    case METHODS::NEWSFEED_DELETE_BAN:
        return "newsfeed.deleteBan";
        break;
    case METHODS::NEWSFEED_DELETE_LIST:
        return "newsfeed.deleteList";
        break;
    case METHODS::NEWSFEED_GET:
        return "newsfeed.get";
        break;
    case METHODS::NEWSFEED_GET_BANNED:
        return "newsfeed.getBanned";
        break;
    case METHODS::NEWSFEED_GET_COMMENTS:
        return "newsfeed.getComments";
        break;
    case METHODS::NEWSFEED_GET_LISTS:
        return "newsfeed.getLists";
        break;
    case METHODS::NEWSFEED_GET_MENTIONS:
        return "newsfeed.getMentions";
        break;
    case METHODS::NEWSFEED_GET_RECOMMENDED:
        return "newsfeed.getRecommended";
        break;
    case METHODS::NEWSFEED_GET_SUGGESTED_SOURCES:
        return "newsfeed.getSuggestedSources";
        break;
    case METHODS::NEWSFEED_IGNORE_ITEM:
        return "newsfeed.ignoreItem";
        break;
    case METHODS::NEWSFEED_SAVE_LIST:
        return "newsfeed.saveList";
        break;
    case METHODS::NEWSFEED_SEARCH:
        return "newsfeed.search";
        break;
    case METHODS::NEWSFEED_UNIGNORED_ITEM:
        return "newsfeed.unignoredItem";
        break;
    case METHODS::NEWSFEED_UNSUBSCRIBE:
        return "newsfeed.unsubscribe";
        break;
    case METHODS::NOTES_ADD:
        return "notes.add";
        break;
    case METHODS::NOTES_CREATE_COMMENT:
        return "notes.createComment";
        break;
    case METHODS::NOTES_DELETE:
        return "notes.delete";
        break;
    case METHODS::NOTES_DELETE_COMMENT:
        return "notes.deleteComment";
        break;
    case METHODS::NOTES_EDIT:
        return "notes.edit";
        break;
    case METHODS::NOTES_EDIT_COMMENT:
        return "notes.editComment";
        break;
    case METHODS::NOTES_GET:
        return "notes.get";
        break;
    case METHODS::NOTES_GET_BY_ID:
        return "notes.getById";
        break;
    case METHODS::NOTES_GET_COMMENTS:
        return "notes.getComments";
        break;
    case METHODS::NOTES_RESTORE_COMMENTS:
        return "notes.restoreComments";
        break;
    case METHODS::NOTIFICATIONS_GET:
        return "notifications.get";
        break;
    case METHODS::NOTIFICATIONS_MARK_AS_VIEWED:
        return "notifications.markAsViewed";
        break;
    case METHODS::PAGES_GET:
        return "pages.get";
        break;
    case METHODS::PAGES_GET_HISTORY:
        return "pages.getHistory";
        break;
    case METHODS::PAGES_GET_TITLES:
        return "pages.getTitles";
        break;
    case METHODS::PAGES_GET_VERSION:
        return "pages.getVersion";
        break;
    case METHODS::PAGES_PARSE_WIKI:
        return "pages.parseWiki";
        break;
    case METHODS::PAGES_SAVE:
        return "pages.save";
        break;
    case METHODS::PAGES_SAVE_ACCESS:
        return "pages.saveAccess";
        break;
    case METHODS::PHOTOS_CONFIRM_TAG:
        return "photos.confirmTag";
        break;
    case METHODS::PHOTOS_COPY:
        return "photos.copy";
        break;
    case METHODS::PHOTOS_CREATE_ALBUM:
        return "photos.createAlbum";
        break;
    case METHODS::PHOTOS_CREATE_COMMENT:
        return "photos.createComment";
        break;
    case METHODS::PHOTOS_DELETE:
        return "photos.delete";
        break;
    case METHODS::PHOTOS_DELETE_ALBUM:
        return "photos.deleteAlbum";
        break;
    case METHODS::PHOTOS_DELETE_COMMENT:
        return "photos.deleteComment";
        break;
    case METHODS::PHOTOS_EDIT:
        return "photos.edit";
        break;
    case METHODS::PHOTOS_EDIT_ALBUM:
        return "photos.editAlbum";
        break;
    case METHODS::PHOTOS_EDIT_COMMENT:
        return "photos.editComment";
        break;
    case METHODS::PHOTOS_GET:
        return "photos.get";
        break;
    case METHODS::PHOTOS_GET_ALBUM:
        return "photos.getAlbum";
        break;
    case METHODS::PHOTOS_GET_ALBUMS_COUNT:
        return "photos.getAlbumsCount";
        break;
    case METHODS::PHOTOS_GET_ALL:
        return "photos.getAll";
        break;
    case METHODS::PHOTOS_GET_ALL_COMMENTS:
        return "photos.getAllComments";
        break;
    case METHODS::PHOTOS_GET_BY_ID:
        return "photos.getById";
        break;
    case METHODS::PHOTOS_GET_CHAT_UPLOAD_SERVER:
        return "photos.getChatUploadServer";
        break;
    case METHODS::PHOTOS_GET_COMMENTS:
        return "photos.getComments";
        break;
    case METHODS::PHOTOS_GET_MARKET_ALBUM_UPLOAD_SERVER:
        return "photos.getMarketAlbumUploadServer";
        break;
    case METHODS::PHOTOS_GET_MARKET_UPLOAD_SERVER:
        return "photos.getMarketUploadServer";
        break;
    case METHODS::PHOTOS_GET_MESSAGES_UPLOAD_SERVER:
        return "photos.getMessagesUploadServer";
        break;
    case METHODS::PHOTOS_GET_NEW_TAGS:
        return "photos.getNewTags";
        break;
    case METHODS::PHOTOS_GET_OWNER_COVER_PHOTO_UPLOAD_SERVER:
        return "photos.getOnwerCoverPhotoUploadServer";
        break;
    case METHODS::PHOTOS_GET_OWNER_PHOTO_UPLOAD_SERVER:
        return "photos.getOnwerPhotoUploadServer";
        break;
    case METHODS::PHOTOS_GET_TAGS:
        return "photos.getTags";
        break;
    case METHODS::PHOTOS_GET_UPLOAD_SERVER:
        return "photos.getUploadServer";
        break;
    case METHODS::PHOTOS_GET_USER_PHOTOS:
        return "photos.getUserPhotos";
        break;
    case METHODS::PHOTOS_GET_WALL_UPLOAD_SERVER:
        return "photos.getWallUploadServer";
        break;
    case METHODS::PHOTOS_MAKE_COVER:
        return "photos.makeCover";
        break;
    case METHODS::PHOTOS_MOVE:
        return "photos.move";
        break;
    case METHODS::PHOTOS_PUT_TAG:
        return "photos.putTag";
        break;
    case METHODS::PHOTOS_REMOVE_TAG:
        return "photos.removeTag";
        break;
    case METHODS::PHOTOS_REORDER_ALBUMS:
        return "photos.reorderAlbums";
        break;
    case METHODS::PHOTOS_REODER_PHOTOS:
        return "photos.reorderPhotos";
        break;
    case METHODS::PHOTOS_REPORT:
        return "photos.report";
        break;
    case METHODS::PHOTOS_REPORT_COMMENT:
        return "photos.reportComment";
        break;
    case METHODS::PHOTOS_RESTORE:
        return "photos.restore";
        break;
    case METHODS::PHOTOS_RESTORE_COMMENT:
        return "photos.restoreComment";
        break;
    case METHODS::PHOTOS_SAVE:
        return "photos.save";
        break;
    case METHODS::PHOTOS_SAVE_MARKET_ALBUM_PHOTO:
        return "photos.saveMarketAlbumPhoto";
        break;
    case METHODS::PHOTOS_SAVE_MARKET_PHOTO:
        return "photos.saveMarketPhoto";
        break;
    case METHODS::PHOTOS_SAVE_MESSAGES_PHOTO:
        return "photos.saveMessagesPhoto";
        break;
    case METHODS::PHOTOS_SAVE_OWNER_COVER_PHOTO:
        return "photos.saveOnwerCoverPhoto";
        break;
    case METHODS::PHOTOS_SAVE_OWNER_PHOTO:
        return "photos.saveOwnerPhoto";
        break;
    case METHODS::PHOTOS_SAVE_WALL_PHOTO:
        return "photos.saveWallPhoto";
        break;
    case METHODS::PHOTOS_SEARCH:
        return "photos.search";
        break;
    case METHODS::POLLS_ADD_VOTE:
        return "polls.addVote";
        break;
    case METHODS::POLLS_CREATE:
        return "polls.create";
        break;
    case METHODS::POLLS_DELETE_VOTE:
        return "polls.deleteVote";
        break;
    case METHODS::POLLS_EDIT:
        return "polls.edit";
        break;
    case METHODS::POLLS_GET_BACKGROUNDS:
        return "polls.getBackgrounds";
        break;
    case METHODS::POLLS_GET_BY_ID:
        return "polls.getById";
        break;
    case METHODS::POLLS_GET_PHOTO_UPLOAD_SERVER:
        return "polls.getPhotoUploadServer";
        break;
    case METHODS::POLLS_GET_VOTES:
        return "polls.getVotes";
        break;
    case METHODS::POLLS_SAVE_PHOTO:
        return "polls.savePhoto";
        break;
    case METHODS::PRETTYCARDS_CREATE:
        return "prettycards.create";
        break;
    case METHODS::PRETTYCARDS_DELETE:
        return "prettycards.delete";
        break;
    case METHODS::PRETTYCARDS_EDIT:
        return "prettycards.edit";
        break;
    case METHODS::PRETTYCARDS_GET:
        return "prettycards.get";
        break;
    case METHODS::PRETTYCARDS_GET_BY_ID:
        return "prettycards.getById";
        break;
    case METHODS::PRETTYCARDS_GET_UPLOAD_URL:
        return "prettycards.getUploadURL";
        break;
    case METHODS::SEARCH_GET_HINTS:
        return "prettycards.getHints";
        break;
    case METHODS::STATS_GET:
        return "stats.get";
        break;
    case METHODS::STATS_GET_POST_REACH:
        return "stats.getPostReach";
        break;
    case METHODS::STATS_TRACK_VISITOR:
        return "stats.trackVisitor";
        break;
    case METHODS::STATUS_GET:
        return "status.get";
        break;
    case METHODS::STATUS_SET:
        return "status.set";
        break;
    case METHODS::STORIES_BAN_OWNER:
        return "stories.banOnwer";
        break;
    case METHODS::STORIES_DELETE:
        return "stories.delete";
        break;
    case METHODS::STORIES_GET:
        return "stories.get";
        break;
    case METHODS::STORIES_GET_BANNED:
        return "stories.getBanned";
        break;
    case METHODS::STORIES_GET_BY_ID:
        return "stories.getById";
        break;
    case METHODS::STORIES_GET_PHOTO_UPLOAD_SERVER:
        return "stories.getPhotoUploadServer";
        break;
    case METHODS::STORIES_GET_REPLIES:
        return "stories.getReplies";
        break;
    case METHODS::STORIES_GET_STATS:
        return "stories.getStats";
        break;
    case METHODS::STORIES_GET_VIDEO_UPLOAD_SERVER:
        return "stories.getVideoUploadServer";
        break;
    case METHODS::STORIES_GET_VIEWERS:
        return "stories.getViewers";
        break;
    case METHODS::STORIES_HIDE_ALL_REPLIES:
        return "stories.hideAllReplies";
        break;
    case METHODS::STORIES_HIDE_REPLY:
        return "stories.hideReply";
        break;
    case METHODS::STORIES_SAVE:
        return "stories.save";
        break;
    case METHODS::STORIES_SEARCH:
        return "stories.search";
        break;
    case METHODS::STORIES_UNBAN_OWNER:
        return "stories.unbanOwner";
        break;
    case METHODS::USERS_GET:
        return "users.get";
        break;
    case METHODS::USERS_GET_FOLLOWERS:
        return "users.getFollowers";
        break;
    case METHODS::USERS_GET_SUBSCRIPTIONS:
        return "users.getSubscriptions";
        break;
    case METHODS::USERS_REPORT:
        return "users.report";
        break;
    case METHODS::USERS_SEARCH:
        return "users.search";
        break;
    case METHODS::UTILS_CHECK_LINK:
        return "utils.checkLink";
        break;
    case METHODS::VIDEO_ADD:
        return "video.add";
        break;
    case METHODS::VIDEO_ADD_ALBUM:
        return "video.addAlbum";
        break;
    case METHODS::VIDEO_ADD_TO_ALBUM:
        return "video.addToAlbum";
        break;
    case METHODS::VIDEO_CREATE_COMMENT:
        return "video.createComment";
        break;
    case METHODS::VIDEO_DELETE:
        return "video.delete";
        break;
    case METHODS::VIDEO_DELETE_ALBUM:
        return "video.deleteAlbum";
        break;
    case METHODS::VIDEO_DELETE_COMMENT:
        return "video.deleteComment";
        break;
    case METHODS::VIDEO_EDIT:
        return "video.edit";
        break;
    case METHODS::VIDEO_EDIT_ALBUM:
        return "video.editAlbum";
        break;
    case METHODS::VIDEO_EDIT_COMMENT:
        return "video.editComment";
        break;
    case METHODS::VIDEO_GET:
        return "video.get";
        break;
    case METHODS::VIDEO_GET_ALBUM_BY_ID:
        return "video.getAlbumById";
        break;
    case METHODS::VIDEO_GET_ALBUMS:
        return "video.getAlbums";
        break;
    case METHODS::VIDEO_GET_ALBUMS_BY_VIDEO:
        return "video.getAlbumsByVideo";
        break;
    case METHODS::VIDEO_GET_COMMENTS:
        return "video.getComments";
        break;
    case METHODS::VIDEO_REMOVE_FROM_ALBUMS:
        return "video.removeFromAlbums";
        break;
    case METHODS::VIDEO_REORDER_ALBUMS:
        return "video.reorderAlbums";
        break;
    case METHODS::VIDEO_REORDER_VIDEOS:
        return "video.reorderVideos";
        break;
    case METHODS::VIDEO_REPORT:
        return "video.report";
        break;
    case METHODS::VIDEO_REPORT_COMMENT:
        return "video.reportComment";
        break;
    case METHODS::VIDEO_RESTORE:
        return "video.restore";
        break;
    case METHODS::VIDEO_RESTORE_COMMENT:
        return "video.restoreComment";
        break;
    case METHODS::VIDEO_SAVE:
        return "video.save";
        break;
    case METHODS::VIDEO_SEARCH:
        return "video.search";
        break;
    case METHODS::WALL_CHECK_COPYRIGHT_LINK:
        return "wall.checkCopyrightLink";
        break;
    case METHODS::WALL_CLOSE_COMMENTS:
        return "wall.closeComments";
        break;
    case METHODS::WALL_CREATE_COMMENT:
        return "wall.createComment";
        break;
    case METHODS::WALL_DELETE:
        return "wall.delete";
        break;
    case METHODS::WALL_DELETE_COMMENT:
        return "wall.deleteComment";
        break;
    case METHODS::WALL_EDIT:
        return "wall.edit";
        break;
    case METHODS::WALL_EDIT_ADS_STEALTH:
        return "wall.editAbsStealth";
        break;
    case METHODS::WALL_EDIT_COMMENT:
        return "wall.editComment";
        break;
    case METHODS::WALL_GET:
        return "wall.get";
        break;
    case METHODS::WALL_GET_BY_ID:
        return "wall.getById";
        break;
    case METHODS::WALL_GET_COMMENT:
        return "wall.getComment";
        break;
    case METHODS::WALL_GET_REPOSTS:
        return "wall.getReposts";
        break;
    case METHODS::WALL_OPEN_COMMENTS:
        return "wall.openComments";
        break;
    case METHODS::WALL_PIN:
        return "wall.pin";
        break;
    case METHODS::WALL_POST:
        return "wall.post";
        break;
    case METHODS::WALL_POST_ADS_STEALTH:
        return "wall.postAbsStealth";
        break;
    case METHODS::WALL_REPORT_COMMENT:
        return "wall.reportComment";
        break;
    case METHODS::WALL_REPORT_POST:
        return "wall.reportPost";
        break;
    case METHODS::WALL_REPOST:
        return "wall.repost";
        break;
    case METHODS::WALL_RESTORE:
        return "wall.restore";
        break;
    case METHODS::WALL_RESTORE_COMMENT:
        return "wall.restoreComment";
        break;
    case METHODS::WALL_SEARCH:
        return "wall.search";
        break;
    case METHODS::WALL_UNPIN:
        return "wall.unpin";
        break;
    }

    return "";
}

}