/**
 * Describes the class for working with VK account.
 * @file UserBase.hpp
 * @author qucals
 * @version 0.0.5 18/08/21
 */

#pragma once

#ifndef _USERBASE_HPP_
#define _USERBASE_HPP_

#include <ClientBase.hpp>

namespace vk
{

namespace base
{

namespace user
{

// The class for working from users by Long Poll Server.
class UserBase : public ClientBase
{
public:
    // There are user's methods, but not all.
    // Description about below methods you can find at https://vk.com/dev/methods
    enum class METHODS
    {
        ACCOUNT_BAN,
        ACCOUNT_CHANGE_PASSWORD,
        ACCOUNT_GET_ACTIVE_OFFERS,
        ACCOUNT_GET_APP_PERMISSIONS,
        ACCOUNT_GET_BANNED,
        ACCOUNT_GET_COUNTERS,
        ACCOUNT_GET_INFO,
        ACCOUNT_GET_PROFILE_INFO,
        ACCOUNT_GET_PUSH_SETTINGS,
        ACCOUNT_REGISTER_DEVICE,
        ACCOUNT_SAVE_PROFILE_INFO,
        ACCOUNT_SET_INFO,
        ACCOUNT_SET_PUSH_SETTINGS,
        ACCOUNT_UNBAN,
        ACCOUNT_UNREGISTER_DEVICE,
        BOARD_ADD_TOPIC,
        BOARD_CLOSE_TOPIC,
        BOARD_CREATE_COMMENT,
        BOARD_DELETE_COMMENT,
        BOARD_DELETE_TOPIC,
        BOARD_EDIT_COMMENT,
        BOARD_EDIT_TOPIC,
        BOARD_FIX_TOPIC,
        BOARD_GET_COMMENTS,
        BOARD_GET_TOPICS,
        BOARD_OPEN_TOPIC,
        BOARD_RESTORE_COMMENT,
        BOARD_UNFIX_TOPIC,
        DATABASE_GET_CHAIRS,
        DATABASE_GET_CITIES,
        DATABASE_GET_CITIES_BY_ID,
        DATABASE_GET_COUNTRIES,
        DATABASE_GET_FACULTIES,
        DATABASE_GET_METRO_STATIONS,
        DATABASE_GET_REGIONS,
        DATABASE_GET_SCHOOL_CLASSES,
        DATABASE_GET_SCHOOLS,
        DATABASE_GET_UNIVERSITIES,
        DOCS_ADD,
        DOCS_DELETE,
        DOCS_EDIT,
        DOCS_GET,
        DOCS_GET_BY_ID,
        DOCS_GET_MESSAGES_UPLOAD_SERVER,
        DOCS_GET_TYPES,
        DOCS_GET_UPLOAD_SERVER,
        DOCS_GET_WALL_UPLOAD_SERVER,
        DOCS_SAVE,
        DOCS_SEARCH,
        EXECUTE,
        FAVE_ADD_ARTICLE,
        FAVE_ADD_LINK,
        FAVE_ADD_PAGE,
        FAVE_ADD_POST,
        FAVE_ADD_PRODUCT,
        FAVE_ADD_TAG,
        FAVE_ADD_VIDEO,
        FAVE_EDIT_TAG,
        FAVE_GET,
        FAVE_GET_PAGES,
        FAVE_GET_TAGS,
        FAVE_MARK_SEEN,
        FAVE_REMOVE_ARTICLE,
        FAVE_REMOVE_LINK,
        FAVE_REMOVE_PAGE,
        FAVE_REMOVE_POST,
        FAVE_REMOVE_PRODUCT,
        FAVE_REMOVE_TAG,
        FAVE_REMOVE_VIDEO,
        FAVE_REORDER_TAGS,
        FAVE_SET_PAGE_TAGS,
        FAVE_SET_TAGS,
        FAVE_TRACK_PAGE_INTERACTION,
        FRIENDS_ADD,
        FRIENDS_ADD_LIST,
        FRIENDS_ARE_FRIENDS,
        FRIENDS_DELETE,
        FRIENDS_DELETE_ALL_REQUESTS,
        FRIENDS_DELETE_LIST,
        FRIENDS_EDIT,
        FRIENDS_EDIT_LIST,
        FRIENDS_GET,
        FRIENDS_GET_APP_USERS,
        FRIENDS_GET_BY_PHONES,
        FRIENDS_GET_LISTS,
        FRIENDS_GET_MUTUAL,
        FRIENDS_GET_ONLINE,
        FRIENDS_GET_RECENT,
        FRIENDS_GET_REQUESTS,
        FRIENDS_GET_SUGGESTIONS,
        FRIENDS_SEARCH,
        GIFTS_GET,
        GROUPS_ADD_ADDRESS,
        GROUPS_ADD_CALLBACK_SERVER,
        GROUPS_ADD_LINK,
        GROUPS_APPROVE_REQUEST,
        GROUPS_BAN,
        GROUPS_CREATE,
        GROUPS_DELETE_ADDRESS,
        GROUPS_DELETE_CALLBACK_SERVER,
        GROUPS_DELETE_LINK,
        GROUPS_DISABLE_ONLINE,
        GROUPS_EDIT,
        GROUPS_EDIT_ADDRESS,
        GROUPS_EDIT_CALLBACK_SERVER,
        GROUPS_EDIT_LINK,
        GROUPS_EDIT_MANAGER,
        GROUPS_ENABLE_ONLINE,
        GROUPS_GET,
        GROUPS_GET_ADDRESSES,
        GROUPS_GET_BANNED,
        GROUPS_GET_BY_ID,
        GROUPS_GET_CALLBACK_CONFIRMATION_CODE,
        GROUPS_GET_CALLBACK_SERVER,
        GROUPS_GET_CALLBACK_SETTINGS,
        GROUPS_GET_CATALOG,
        GROUPS_GET_CATALOG_INFO,
        GROUPS_GET_INVITED_USERS,
        GROUPS_GET_INVITIES,
        GROUPS_GET_LONG_POLL_SERVER,
        GROUPS_GET_LONG_POLL_SETTINGS,
        GROUPS_GET_MEMBERS,
        GROUPS_GET_ONLINE_STATUS,
        GROUPS_GET_REQUESTS,
        GROUPS_GET_SETTINGS,
        GROUPS_GET_TAG_LIST,
        GROUPS_GET_TOKEN_PERMISSIONS,
        GROUPS_INVITE,
        GROUPS_IS_MEMBER,
        GROUPS_JOIN,
        GROUPS_LEAVE,
        GROUPS_REMOVE_USER,
        GROUPS_REORDER_LINK,
        GROUPS_SEARCH,
        GROUPS_SET_CALLBACK_SETTINGS,
        GROUPS_SET_LONG_POLL_SETTINGS,
        GROUPS_SET_SETTINGS,
        GROUPS_SET_USER_NOTE,
        GROUPS_TAG_ADD,
        GROUPS_TAG_BIND,
        GROUPS_TAG_DELETE,
        GROUPS_TAG_UPDATE,
        GROUPS_UNBAN,
        LEADFORMS_CREATE,
        LEADFORMS_DELETE,
        LEADFORMS_GET,
        LEADFORMS_GET_LEADS,
        LEADFORMS_GET_UPLOAD_URL,
        LEADFORMS_LIST,
        LEADFORMS_UPDATE,
        LIKES_ADD,
        LIKES_DELETE,
        LIKES_GET_LIST,
        LIKES_IS_LIKED,
        MARKET_ADD,
        MARKET_ADD_ALBUM,
        MARKET_ADD_TO_ALBUM,
        MARKET_CREATE_COMMENT,
        MARKET_DELETE,
        MARKET_DELETE_ALBUM,
        MARKET_DELETE_COMMENT,
        MARKET_EDIT,
        MARKET_EDIT_ALBUM,
        MARKET_EDIT_COMMENT,
        MARKET_EDIT_ORDER,
        MARKET_GET,
        MARKET_GET_ALBUM_BY_ID,
        MARKET_GET_ALBUMS,
        MARKET_GET_BY_ID,
        MARKET_GET_CATEGORIES,
        MARKET_GET_COMMENTS,
        MARKET_GET_GROUP_ORDERS,
        MARKET_GET_ORDER_BY_ID,
        MARKET_GET_ORDER_ITEMS,
        MARKET_GET_ORDERS,
        MARKET_REMOVE_FROM_ALBUM,
        MARKET_REORDER_ALBUMS,
        MARKET_REORDER_ITEMS,
        MARKET_REPORT,
        MARKET_REPORT_COMMENT,
        MARKET_RESTORE,
        MARKET_RESTORE_COMMENT,
        MARKET_SEARCH,
        MESSAGES_ADD_CHAT_USER,
        MESSAGES_ALLOW_MESSAGES_FROM_GROUP,
        MESSAGES_CREATE_CHAT,
        MESSAGES_DELETE,
        MESSAGES_DELETE_CHAT_PHOTO,
        MESSAGES_DELETE_CONVERSATION,
        MESSAGES_DENY_MESSAGES_FROM_GROUP,
        MESSAGES_EDIT,
        MESSAGES_EDIT_CHAT,
        MESSAGES_GET_BY_CONVERSATION_MESSAGE_ID,
        MESSAGES_GET_BY_ID,
        MESSAGES_GET_CHAT,
        MESSAGES_GET_CHAT_PREVIEW,
        MESSAGES_GET_CONVERSATIONS,
        MESSAGES_GET_CONVERSATION_BY_ID,
        MESSAGES_GET_HISTORY,
        MESSAGES_GET_HISTORY_ATTACHMENTS,
        MESSAGES_GET_IMPORTANT_MESSAGES,
        MESSAGES_GET_INVITE_LINK,
        MESSAGES_GET_LAST_ACTIVITY,
        MESSAGES_GET_LONG_POLL_HISTORY,
        MESSAGES_GET_LONG_POLL_SERVER,
        MESSAGES_IS_MESSAGES_FROM_GROUP_ALLOWED,
        MESSAGES_JOIN_CHAT_BY_INVITE_LINK,
        MESSAGES_MARK_AS_ANSWERED_CONVERSATION,
        MESSAGES_MARK_AS_IMPORTANT,
        MESSAGES_MARK_AS_IMPORTANT_CONVERSATION,
        MESSAGES_MARK_AS_READ,
        MESSAGES_PIN,
        MESSAGES_REMOVE_CHAT_USER,
        MESSAGES_RESTORE,
        MESSAGES_SEARCH,
        MESSAGES_SEARCH_CONVERSATIONS,
        MESSAGES_SEND,
        MESSAGES_SEND_MESSAGE_EVENT_ANSWER,
        MESSAGES_SET_ACTIVITY,
        MESSAGES_SET_CHAT_PHOTO,
        MESSAGES_UNPIN,
        NEWSFEED_ADD_BAN,
        NEWSFEED_DELETE_BAN,
        NEWSFEED_DELETE_LIST,
        NEWSFEED_GET,
        NEWSFEED_GET_BANNED,
        NEWSFEED_GET_COMMENTS,
        NEWSFEED_GET_LISTS,
        NEWSFEED_GET_MENTIONS,
        NEWSFEED_GET_RECOMMENDED,
        NEWSFEED_GET_SUGGESTED_SOURCES,
        NEWSFEED_IGNORE_ITEM,
        NEWSFEED_SAVE_LIST,
        NEWSFEED_SEARCH,
        NEWSFEED_UNIGNORED_ITEM,
        NEWSFEED_UNSUBSCRIBE,
        NOTES_ADD,
        NOTES_CREATE_COMMENT,
        NOTES_DELETE,
        NOTES_DELETE_COMMENT,
        NOTES_EDIT,
        NOTES_EDIT_COMMENT,
        NOTES_GET,
        NOTES_GET_BY_ID,
        NOTES_GET_COMMENTS,
        NOTES_RESTORE_COMMENTS,
        NOTIFICATIONS_GET,
        NOTIFICATIONS_MARK_AS_VIEWED,
        PAGES_GET,
        PAGES_GET_HISTORY,
        PAGES_GET_TITLES,
        PAGES_GET_VERSION,
        PAGES_PARSE_WIKI,
        PAGES_SAVE,
        PAGES_SAVE_ACCESS,
        PHOTOS_CONFIRM_TAG,
        PHOTOS_COPY,
        PHOTOS_CREATE_ALBUM,
        PHOTOS_CREATE_COMMENT,
        PHOTOS_DELETE,
        PHOTOS_DELETE_ALBUM,
        PHOTOS_DELETE_COMMENT,
        PHOTOS_EDIT,
        PHOTOS_EDIT_ALBUM,
        PHOTOS_EDIT_COMMENT,
        PHOTOS_GET,
        PHOTOS_GET_ALBUM,
        PHOTOS_GET_ALBUMS_COUNT,
        PHOTOS_GET_ALL,
        PHOTOS_GET_ALL_COMMENTS,
        PHOTOS_GET_BY_ID,
        PHOTOS_GET_CHAT_UPLOAD_SERVER,
        PHOTOS_GET_COMMENTS,
        PHOTOS_GET_MARKET_ALBUM_UPLOAD_SERVER,
        PHOTOS_GET_MARKET_UPLOAD_SERVER,
        PHOTOS_GET_MESSAGES_UPLOAD_SERVER,
        PHOTOS_GET_NEW_TAGS,
        PHOTOS_GET_OWNER_COVER_PHOTO_UPLOAD_SERVER,
        PHOTOS_GET_OWNER_PHOTO_UPLOAD_SERVER,
        PHOTOS_GET_TAGS,
        PHOTOS_GET_UPLOAD_SERVER,
        PHOTOS_GET_USER_PHOTOS,
        PHOTOS_GET_WALL_UPLOAD_SERVER,
        PHOTOS_MAKE_COVER,
        PHOTOS_MOVE,
        PHOTOS_PUT_TAG,
        PHOTOS_REMOVE_TAG,
        PHOTOS_REORDER_ALBUMS,
        PHOTOS_REODER_PHOTOS,
        PHOTOS_REPORT,
        PHOTOS_REPORT_COMMENT,
        PHOTOS_RESTORE,
        PHOTOS_RESTORE_COMMENT,
        PHOTOS_SAVE,
        PHOTOS_SAVE_MARKET_ALBUM_PHOTO,
        PHOTOS_SAVE_MARKET_PHOTO,
        PHOTOS_SAVE_MESSAGES_PHOTO,
        PHOTOS_SAVE_OWNER_COVER_PHOTO,
        PHOTOS_SAVE_OWNER_PHOTO,
        PHOTOS_SAVE_WALL_PHOTO,
        PHOTOS_SEARCH,
        POLLS_ADD_VOTE,
        POLLS_CREATE,
        POLLS_DELETE_VOTE,
        POLLS_EDIT,
        POLLS_GET_BACKGROUNDS,
        POLLS_GET_BY_ID,
        POLLS_GET_PHOTO_UPLOAD_SERVER,
        POLLS_GET_VOTES,
        POLLS_SAVE_PHOTO,
        PRETTYCARDS_CREATE,
        PRETTYCARDS_DELETE,
        PRETTYCARDS_EDIT,
        PRETTYCARDS_GET,
        PRETTYCARDS_GET_BY_ID,
        PRETTYCARDS_GET_UPLOAD_URL,
        SEARCH_GET_HINTS,
        STATS_GET,
        STATS_GET_POST_REACH,
        STATS_TRACK_VISITOR,
        STATUS_GET,
        STATUS_SET,
        STORIES_BAN_OWNER,
        STORIES_DELETE,
        STORIES_GET,
        STORIES_GET_BANNED,
        STORIES_GET_BY_ID,
        STORIES_GET_PHOTO_UPLOAD_SERVER,
        STORIES_GET_REPLIES,
        STORIES_GET_STATS,
        STORIES_GET_VIDEO_UPLOAD_SERVER,
        STORIES_GET_VIEWERS,
        STORIES_HIDE_ALL_REPLIES,
        STORIES_HIDE_REPLY,
        STORIES_SAVE,
        STORIES_SEARCH,
        STORIES_UNBAN_OWNER,
        USERS_GET,
        USERS_GET_FOLLOWERS,
        USERS_GET_SUBSCRIPTIONS,
        USERS_REPORT,
        USERS_SEARCH,
        UTILS_CHECK_LINK,
        VIDEO_ADD,
        VIDEO_ADD_ALBUM,
        VIDEO_ADD_TO_ALBUM,
        VIDEO_CREATE_COMMENT,
        VIDEO_DELETE,
        VIDEO_DELETE_ALBUM,
        VIDEO_DELETE_COMMENT,
        VIDEO_EDIT,
        VIDEO_EDIT_ALBUM,
        VIDEO_EDIT_COMMENT,
        VIDEO_GET,
        VIDEO_GET_ALBUM_BY_ID,
        VIDEO_GET_ALBUMS,
        VIDEO_GET_ALBUMS_BY_VIDEO,
        VIDEO_GET_COMMENTS,
        VIDEO_REMOVE_FROM_ALBUMS,
        VIDEO_REORDER_ALBUMS,
        VIDEO_REORDER_VIDEOS,
        VIDEO_REPORT,
        VIDEO_REPORT_COMMENT,
        VIDEO_RESTORE,
        VIDEO_RESTORE_COMMENT,
        VIDEO_SAVE,
        VIDEO_SEARCH,
        WALL_CHECK_COPYRIGHT_LINK,
        WALL_CLOSE_COMMENTS,
        WALL_CREATE_COMMENT,
        WALL_DELETE,
        WALL_DELETE_COMMENT,
        WALL_EDIT,
        WALL_EDIT_ADS_STEALTH,
        WALL_EDIT_COMMENT,
        WALL_GET,
        WALL_GET_BY_ID,
        WALL_GET_COMMENT,
        WALL_GET_REPOSTS,
        WALL_OPEN_COMMENTS,
        WALL_PIN,
        WALL_POST,
        WALL_POST_ADS_STEALTH,
        WALL_REPORT_COMMENT,
        WALL_REPORT_POST,
        WALL_REPOST,
        WALL_RESTORE,
        WALL_RESTORE_COMMENT,
        WALL_SEARCH,
        WALL_UNPIN,
    };

    // There are types' messages which the VK server can return during authorization.
    enum class VALIDATION_TYPES
    {
        TWOFA_SMS,
        TWOFA_APP,
        UNKNOWN
    };

public:
    /**
     * @param  appId: the id of the application.
     * @param  appSecureKey: the secure key of the application.
     */
    UserBase(std::string  appId, std::string  appSecureKey);

    ~UserBase() = default;

    /**
     * @brief  The authorization function by login and password.
     * @param  login: your login.
     * @param  password: your password.
     * @retval 'true' if auth is successfully and 'false' in another case.
     */
    __VIRTUAL bool Auth(std::string& login, std::string& password);

    /**
     * @brief  The authorization function by access token.
     * @param  accessToken: your access token.
     * @retval 'true' if auth is successfully and 'false' in another case.
     */
    bool Auth(const std::string& accessToken) __FINAL;

    /**
     * @brief  The function of sending any request to the VK server.
     * @param  method: the enum's method.
     * @param  parametersData: the data of parameters for the request.
     * @retval the answer of the request in json format.
     */
    json SendRequest(METHODS method, const json& parametersData);

    /**
     * @brief  The function sends any request to the VK serve.
     * @param  method: the method in str format.
     * @param  parametersData: the data of parameters for the request.
     * @retval the answer of the request in json format.
     */
    json SendRequest(const std::string& method, const json& parametersData);

    /**
     * @brief  The function of converting an enum's method to a string (URL).
     * @param  method: enum's method.
     * @retval a string of this method in URL format.
     */
    __STATIC std::string MethodToString(METHODS method);

    /**
     * @brief  Get the validation type for auth by its description in string format.
     * @param  descriptionType: the description of the type.
     * @retval the validation type in enum format (VALIDATION_TYPES).
     */
    __STATIC VALIDATION_TYPES GetValidationType(const std::string& descriptionType);

    UserBase& operator=(const UserBase&) = delete;

protected:
    /**
     * @brief  Checking validation parameters on having items like access_token and others.
     * @note   If the data of parameters won't have the function will add it.
     * @param  parametersData: the data of parameters that you want to check.
     * @retval the correctly data of parameters.
     */
    json CheckValidationParameters(const json& parametersData) __OVERRIDE;

    /**
     * @brief  Get the URL to the captcha.
     * @param  parametersData: the parameters data which was sent in the previous request.
     * @param  answerData: the answer which was received in the previous request.
     * @retval the URL to the captcha.
     */
    __STATIC std::string GetURLCaptcha(json& parametersData, const json& answerData);

private:
    const std::string appId_;
    const std::string appSecureKey_;

    std::string accessToken_;
    std::string userId_;
};

} // namespace user

} // namespace base

} // namespace vk

#endif // _USERBASE_HPP_