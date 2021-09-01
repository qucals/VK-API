/**
 * Contains general defines about the language.
 * @file Defines.hpp
 * @author qucals
 * @version 0.0.8 24/08/21
 */

#ifndef VKAPI_DEFINES_HPP
#define VKAPI_DEFINES_HPP

#include "Config.hpp"

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
#define _VKAPI_OVERRIDE     override
#define _VKAPI_FINAL        final
#define _VKAPI_NOEXCEPT     noexcept
#define _VKAPI_EXPLICIT     explicit
#define _VKAPI_CONSTEXPR    constexpr

#include <utility>
#define _VKAPI_MOVE(x)      std::move(x)
#else
#define _VKAPI_OVERRIDE
#define _VKAPI_FINAL
#define _VKAPI_NOEXCEPT
#define _VKAPI_EXPLICIT
#define _VKAPI_CONSTEXPR
#define _VKAPI_MOVE(x)      x
#endif

#define _VKAPI_VIRTUAL      virtual
#define _VKAPI_INLINE       inline

#define _VKAPI_UNUSED(x) (void)(x)

#if defined(_MSC_VER)
#define _VKAPI_MSVC 1

#define __DISABLE_WARNING_PUSH           __pragma(warning(push))
#define __DISABLE_WARNING_POP            __pragma(warning(pop))
#define __DISABLE_WARNING(warningNumber) __pragma(warning(disable : warningNumber))
#elif defined(__GNUC__) || defined(__clang__)
#define _VKAPI_CLANG 1

#define __DO_PRAGMA(X)                   _Pragma(#X)
#define __DISABLE_WARNING_PUSH           __DO_PRAGMA("GCC diagnostic push")
#define __DISABLE_WARNING_POP            __DO_PRAGMA("GCC diagnostic pop")
#define __DISABLE_WARNING(warningName)   __DO_PRAGMA("GCC diagnostic ignored \"#warningName\"")
#else
#define __DISABLE_WARNING_PUSH
#define __DISABLE_WARNING_POP
#define __DISABLE_WARNING
#endif

#ifndef _VKAPI_MSVC
#define _VKAPI_MSVC 0
#endif // _VKAPI_MSVC

#ifndef _VKAPI_CLANG
#define _VKAPI_CLANG 0
#endif // _VKAPI_CLANG

#if _VKAPI_MSVC
#define _VKAPI_UNUSED_FOR_ANALYZER
#elif _VKAPI_CLANG
#define _VKAPI_UNUSED_FOR_ANALYZER __attribute__((unused))
#endif

#ifndef _VKAPI_DEBUG_PRINT
#if __DEBUG__
#define _VKAPI_DEBUG_STDCERR(x) (std::cerr << (x))
#define _VKAPI_DEBUG_STDCOUT(x) (std::cout << (x))
#else
#define _VKAPI_DEBUG_STDCERR(x)
#define _VKAPI_DEBUG_STDCOUT(x)
#endif // __DEBUG__
#endif // _VKAPI_DEBUG_PRINT

#ifndef __VKAPI_VERSION_ADDED_OPTIONAL
#define __VKAPI_VERSION_ADDED_OPTIONAL __VKAPI_VERSION_NUM(0, 0, 7)
#endif // __VKAPI_VERSION_ADDED_OPTIONAL

#endif //VKAPI_DEFINES_HPP
