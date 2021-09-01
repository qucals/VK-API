/**
 * Describes optional class.
 * @file Optional.hpp
 * @author qucals
 * @version 0.0.8 24/08/21
 */

#ifndef VKAPI_OPTIONAL_HPP
#define VKAPI_OPTIONAL_HPP

#include <vector>

#include "Defines.hpp"
#include "Types.hpp"

#include "nlohmann/json.hpp"

namespace vk
{

namespace base
{

#if defined(__CPLUSPLUS_OVER_17)

#include <optional>

template<typename T>
class Optional : public std::optional<T>
{};

#else

template<typename _Type>
class BasicOptional
{
public:
#if defined(__CPLUSPLUS_OVER_11)
    typedef _Type&& _RValue_Type;
    typedef const _Type& _ConstRef_Type;
#else
    typedef const _Type& _RValue_Type;
    typedef const _Type& _ConstRef_Type;
#endif // defined(__CPLUSPLUS_OVER_14)

    BasicOptional()
        : m_isSet(false)
    {}

    _VKAPI_EXPLICIT BasicOptional(_RValue_Type val)
        : m_val(_VKAPI_MOVE(val))
        , m_isSet(true)
    {}

    void Set(_ConstRef_Type val)
    {
        m_val = val;
        m_isSet = true;
    }

    _VKAPI_INLINE const _Type& Get() const& _VKAPI_NOEXCEPT
    { return m_val; }

    void Clear()
    {
        m_val = _Type();
        m_isSet = false;
    }

    bool Empty()
    { return !m_isSet; }

    BasicOptional& operator=(_ConstRef_Type val)
    {
        m_val = val;
        m_isSet = true;
        return *this;
    }

    _VKAPI_EXPLICIT operator _Type() const
    {
        return m_val;
    }

    bool operator==(const bool& val) const
    { return m_isSet == val; }

private:
    _Type m_val;
    bool m_isSet;
};

template<typename T>
class Optional : public BasicOptional<T>
{};

#endif // defined(__CPLUSPLUS_OVER_17)

typedef Optional<IdType> Opt_IdType;
typedef Optional<UIdType> Opt_UIdType;
typedef Optional<const std::string&> Opt_StrType;
typedef Optional<float> Opt_FloatType;
typedef Optional<const JsonType&> Opt_JsonType;
typedef Optional<const std::vector<IdType>&> Opt_IdArrayType;
typedef Optional<IndicatorType> Opt_IndicatorType;

} // namespace base

} // namespace vk

#endif //VKAPI_OPTIONAL_HPP
