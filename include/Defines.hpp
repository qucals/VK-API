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

#endif //VKAPI_DEFINES_H
