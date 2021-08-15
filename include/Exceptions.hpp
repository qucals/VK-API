/**
 * Containts exceptions and information about them of this library.
 * @file Exceptions.hpp
 * @author qucals
 * @version 0.0.3 15/08/21
 */

#pragma once

#ifndef _EXCEPTIONS_HPP_
#define _EXCEPTIONS_HPP_

#include <exception>

#include <Defines.hpp>

namespace vk
{

namespace ex
{

class BaseException : __VIRTUAL public std::exception
{
protected:
    std::string errorMessage_;

public:
    explicit BaseException(const std::string& msg)
        : errorMessage_(msg)
    {}

    __VIRTUAL ~BaseException() throw() {}

    __VIRTUAL const char* what() const throw()
    {
        return errorMessage_.c_str();
    }
};

class AlreadyConnectedException : __VIRTUAL public BaseException
{
public:
    explicit AlreadyConnectedException()
        : BaseException("The client is already connected to Long Poll Server!")
    {}
};

class NotConnectedException : __VIRTUAL public BaseException
{
public:
    explicit NotConnectedException()
        : BaseException("The client is not already connected to Long Poll Server!")
    {}
};

class EmptyArgumentException : __VIRTUAL public BaseException
{
public:
    explicit EmptyArgumentException()
        : BaseException("The size of argument's symbols cannot equal zero!")
    {}
};

class RequestError : __VIRTUAL public BaseException
{
public:
    explicit RequestError()
        : BaseException("Request returned unknown errors!")
    {}
};

} // namespace ex

} // namespace vk

#endif // _EXCEPTIONS_HPP_