/**
 * Contains general defines about the language.
 * @file Defines.hpp
 * @author qucals
 * @version 0.0.5 18/08/21
 */

#pragma once

#ifndef VKAPI_DEFINES_H
#define VKAPI_DEFINES_H

#ifndef __NO_TESTED
#define __NO_TESTED
#endif // __NO_TESTED

#ifndef __NO_IMPL
#define __NO_IMPL
#endif // __NO_IMPL

#ifndef __STATIC
#define __STATIC static
#endif // __STATIC

#if (__cplusplus >= 201103L)
#define __CPLUSPLUS_OVER_11

#ifndef __OVERRIDE
#define __OVERRIDE override
#endif // __OVERRIDE

#ifndef __FINAL
#define __FINAL final
#endif // __FINAL

#ifndef __NOEXCEPT
#define __NOEXCEPT noexcept
#endif // __NOEXCEPT

#ifndef __EXPLICIT
#define __EXPLICIT explicit
#endif // __EXPLICIT

#ifndef __MOVE
#include <utility>
#define __MOVE(x) std::move(x)
#endif // __MOVE(x)
#else
#ifndef __OVERRIDE
#define __OVERRIDE
#endif // __OVERRIDE

#ifndef __FINAL
#define __FINAL
#endif // __FINAL

#ifndef __NOEXCEPT
#define __NOEXCEPT
#endif // __NOEXCEPT

#ifndef __EXPLICIT
#define __EXPLICIT
#endif // __EXPLICIT

#ifndef __MOVE
#define __MOVE(x) x
#endif // __MOVE
#endif

#ifndef __VIRTUAL
#define __VIRTUAL virtual
#endif // __VIRTUAL

#ifndef __INLINE
#define __INLINE inline
#endif // __INLINE

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

#endif //VKAPI_DEFINES_H