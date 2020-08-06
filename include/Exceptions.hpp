#pragma once

#ifndef _EXCEPTIONS_HPP_
#define _EXCEPTIONS_HPP_

#include <exception>

namespace vk {

namespace ex {

    class already_connected : public std::exception {
    public:
        already_connected() noexcept
            : exception("The client is already connected to Long Poll Server!")
        {
        }
    };

    class not_connected : public std::exception {
    public:
        not_connected() noexcept
            : exception("The client is not already connected to Long Poll Server!")
        {
        }
    };

    class empty_argument : public std::exception {
    public:
        empty_argument() noexcept
            : exception("The size of argument's symbols cannot equal zero!")
        {
        }
    };

} // namespace ex
} // namespace vk

#endif // _EXCEPTIONS_HPP_