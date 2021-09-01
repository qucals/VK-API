/**
 * Contains exceptions and information about them of this library.
 * @file Exceptions.hpp
 * @author qucals
 * @version 0.0.8 24/08/21
 */

// TODO (#17): Add comments about exceptions

#ifndef VKAPI_EXCEPTIONS_HPP
#define VKAPI_EXCEPTIONS_HPP

#include <string>
#include <exception>

#include "Defines.hpp"

namespace vk
{

namespace ex
{

class BaseException : _VKAPI_VIRTUAL public std::exception
{
protected:
    std::string m_errorMessage;

public:
    _VKAPI_EXPLICIT BaseException(std::string msg)
        : m_errorMessage(_VKAPI_MOVE(msg))
    {}

    ~BaseException() _VKAPI_NOEXCEPT _VKAPI_OVERRIDE = default;

    const char* what() const _VKAPI_NOEXCEPT _VKAPI_OVERRIDE
    {
        return m_errorMessage.c_str();
    }
};

class AlreadyConnectedException : _VKAPI_VIRTUAL public BaseException
{
public:
    _VKAPI_EXPLICIT AlreadyConnectedException()
        : BaseException("The client is already connected to Long Poll Server!")
    {}
};

class NotConnectedException : _VKAPI_VIRTUAL public BaseException
{
public:
    _VKAPI_EXPLICIT NotConnectedException()
        : BaseException("The client is not already connected to Long Poll Server!")
    {}
};

class EmptyArgumentException : _VKAPI_VIRTUAL public BaseException
{
public:
    _VKAPI_EXPLICIT EmptyArgumentException()
        : BaseException("The size of argument's symbols cannot equal zero!")
    {}
};

class RequestError : _VKAPI_VIRTUAL public BaseException
{
public:
    _VKAPI_EXPLICIT RequestError()
        : BaseException("Request returned unknown errors!")
    {}
};

class AuthFailed : _VKAPI_VIRTUAL public BaseException
{
public:
    _VKAPI_EXPLICIT AuthFailed()
        : BaseException("Authorization completed with an error!")
    {}
};

} // namespace ex

} // namespace vk

#endif // VKAPI_EXCEPTIONS_HPP
