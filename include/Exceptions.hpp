#pragma once

#ifndef _EXCEPTIONS_HPP_
#define _EXCEPTIONS_HPP_

#include <exception>

namespace vk
{

namespace ex
{

class BaseException : virtual public std::exception
{
protected:
    std::string errorMessage_;

public:
    explicit BaseException(const std::string& msg)
        : errorMessage_(msg)
    {}

    virtual ~BaseException() throw() {}

    virtual const char* what() const throw()
    {
        return errorMessage_.c_str();
    }
};

class AlreadyConnectedException : virtual public BaseException
{
public:
    explicit AlreadyConnectedException()
        : BaseException("The client is already connected to Long Poll Server!")
    {}
};

class NotConnectedException : virtual public BaseException
{
public:
    explicit NotConnectedException()
        : BaseException("The client is not already connected to Long Poll Server!")
    {}
};

class EmptyArgumentException : virtual public BaseException
{
public:
    explicit EmptyArgumentException()
        : BaseException("The size of argument's symbols cannot equal zero!")
    {}
};

class RequestError : virtual public BaseException
{
public:
    explicit RequestError()
        : BaseException("Request returned unknown errors!")
    {}
};

} // namespace ex

} // namespace vk

#endif // _EXCEPTIONS_HPP_