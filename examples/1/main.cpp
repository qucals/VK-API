#include "Bot.hpp"

int main(int argc, const char** argv)
{
    std::string access_token = "your_token_there";
    std::string group_id = "your_group_id_there";

    VK::Bot bot(group_id);

    if (bot.Auth(access_token) == true) {
        // The further behaviour
    } else {
        // The further behaviour
    }

    return 0;
}