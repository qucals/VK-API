/**
 * Contains defined types.
 * @file Types.hpp
 * @author qucals
 * @version 0.0.8 1/09/21
 */

#ifndef VKAPI_TYPES_HPP
#define VKAPI_TYPES_HPP

#include "Defines.hpp"

#ifndef __CPLUSPLUS_OVER_11
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
#endif // __CPLUSPLUS_OVER_11

#include "nlohmann/json.hpp" // nlohmann::json

namespace vk
{

namespace base
{

typedef long long int IdType;
typedef unsigned long long UIdType;
typedef bool IndicatorType;

typedef nlohmann::json JsonType;


} // namespace base

} // namespace vk

#endif //VKAPI_TYPES_HPP
