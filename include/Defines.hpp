/**
 * Contains general defines about the language.
 * @file Defines.hpp
 * @author qucals
 * @version 0.0.6 20/08/21
 */

#pragma once

#ifndef VKAPI_DEFINES_HPP
#define VKAPI_DEFINES_HPP

#include <Config.hpp>

#ifndef _VKAPI_NO_IMPL
#define _VKAPI_NO_IMPL
#endif // _VKAPI_NO_IMPL

#ifndef _VKAPI_STATIC
#define _VKAPI_STATIC static
#endif // _VKAPI_STATIC

#if (__cplusplus >= 201103L)
#define __CPLUSPLUS_OVER_11
#endif // (__cplusplus >= 201103L)

#if (__cplusplus >= 201402L)
#define __CPLUSPLUS_OVER_14
#endif // ((__cplusplus >= 201402L))

#if (__cplusplus >= 201703L)
#define __CPLUSPLUS_OVER_17
#endif // (__cplusplus >= 201703L)

#if (__cplusplus >= 202002L)
#define __CPLUSPLUS_OVER_20
#endif // (__cplusplus >= 202002L)

#ifdef __CPLUSPLUS_OVER_11
#ifndef _VKAPI_OVERRIDE
#define _VKAPI_OVERRIDE override
#endif // _VKAPI_OVERRIDE

#ifndef _VKAPI_FINAL
#define _VKAPI_FINAL final
#endif // _VKAPI_FINAL

#ifndef _VKAPI_NOEXCEPT
#define _VKAPI_NOEXCEPT noexcept
#endif // _VKAPI_NOEXCEPT

#ifndef _VKAPI_EXPLICIT
#define _VKAPI_EXPLICIT explicit
#endif // _VKAPI_EXPLICIT

#ifndef _VKAPI_MOVE
#include <utility>
#define _VKAPI_MOVE(x) std::move(x)
#endif // _VKAPI_MOVE(x)
#else
#ifndef _VKAPI_OVERRIDE
#define _VKAPI_OVERRIDE
#endif // _VKAPI_OVERRIDE

#ifndef _VKAPI_FINAL
#define _VKAPI_FINAL
#endif // _VKAPI_FINAL

#ifndef _VKAPI_NOEXCEPT
#define _VKAPI_NOEXCEPT
#endif // _VKAPI_NOEXCEPT

#ifndef _VKAPI_EXPLICIT
#define _VKAPI_EXPLICIT
#endif // _VKAPI_EXPLICIT

#ifndef _VKAPI_MOVE
#define _VKAPI_MOVE(x) x
#endif // _VKAPI_MOVE
#endif

#ifndef _VKAPI_VIRTUAL
#define _VKAPI_VIRTUAL virtual
#endif // _VKAPI_VIRTUAL

#ifndef _VKAPI_INLINE
#define _VKAPI_INLINE inline
#endif // _VKAPI_INLINE

#if defined(_MSC_VER)
#define __DISABLE_WARNING_PUSH           __pragma(warning( push ))
#define __DISABLE_WARNING_POP            __pragma(warning( pop ))
#define __DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))
#elif defined(__GNUC__) || defined(__clang__)
#define __DO_PRAGMA(X)                   _Pragma(#X)
#define __DISABLE_WARNING_PUSH           __DO_PRAGMA(GCC diagnostic push)
#define __DISABLE_WARNING_POP            __DO_PRAGMA(GCC diagnostic pop)
#define __DISABLE_WARNING(warningName)   __DO_PRAGMA(ide diagnostic ignored #warningName)
#else
#define __DISABLE_WARNING_PUSH
#define __DISABLE_WARNING_POP
#define __DISABLE_WARNING
#endif

#ifndef __VKAPI_VERSION_ADDED_OPTIONAL
#define __VKAPI_VERSION_ADDED_OPTIONAL __VKAPI_VERSION_NUM(0, 0, 7)
#endif // __VKAPI_VERSION_ADDED_OPTIONAL

#endif //VKAPI_DEFINES_HPP
