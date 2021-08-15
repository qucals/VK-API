/**
 * Containts general defines about the language.
 * @file Defines.hpp
 * @author qucals
 * @version 0.0.3 15/08/21
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
#else
#ifndef __OVERRIDE
#define __OVERRIDE
#endif // __OVERRIDE

#ifndef __FINAL
#define __FINAL
#endif // __FINAL
#endif

#ifndef __VIRTUAL
#define __VIRTUAL virtual
#endif // __VIRTUAL

#ifndef __INLINE
#define __INLINE inline
#endif // __INLINE

#endif //VKAPI_DEFINES_H
