#include "BotBase.hpp"

int main(int argc, const char** argv)
{
    std::string access_token = "your_token_there";
    std::string group_id = "your_group_id_there";

    vk::base::bot::BotBase bot(group_id);

    std::cout << "I'm created vk::BotBase!" << std::endl;
    std::cout << "The address is: " << &bot << std::endl;
    try {
        if (bot.Auth(access_token)) {
            std::cout << "Auth is ended successfully" << std::endl;
        } else {
            std::cout << "Auth is failed!" << std::endl;
        }
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}