#include "UserBase.hpp"

int main(int argc, const char** argv)
{
    std::string app_id = "your_app_id_there";
    std::string app_secure_key = "your_app_secure_key_there";
    std::string login = "your_login_there";
    std::string password = "your_password_there";
    std::string access_token = "your_access_token_there";

    vk::UserBase user(app_id, app_secure_key);
    try {
        if (user.Auth(login, password) == true) {
            // The further behaviour
        } else {
            // The further behaviour
        }

        // or

        if (user.Auth(access_token) == true) {
            // The further behaviour
        } else {
            // The further behaviour
        }
    } catch (std::exception ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}