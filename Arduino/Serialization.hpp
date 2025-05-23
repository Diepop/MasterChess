#pragma once
#include <type_traits>
#include <string_view>
#include <vector>
#include <array>
#include <tuple>
#include <format>

namespace fmt
{
    template<std::ranges::range Range, class Delim>
    struct Join
    {
        Range range;
        Delim delim;
    };

    template<std::ranges::range Range, class Delim>
    auto join(Range range, Delim delim) -> Join<Range, Delim>
    {
        return { std::move(range), std::move(delim) };
    }
}

namespace std
{
    template<class Range, class Delim>
    struct formatter<fmt::Join<Range, Delim>>
    {
        using element_type = ranges::range_value_t<Range>;

        formatter<element_type> form;
        formatter<Delim> dForm;

        constexpr auto parse(auto& ctx)
        {
            return form.parse(ctx);
        }

        auto format(fmt::Join<Range, Delim> p, auto& ctx) const
        {
            auto it = ranges::begin(p.range);
            auto end = ranges::end(p.range);
            if (it != end)
                form.format(*it++, ctx);
            for (; it != end; ++it)
            {
                dForm.format(p.delim, ctx);
                form.format(*it, ctx);
            }
            return ctx.out();
        }

    };

}

namespace Arduino
{
    using std::string_view;
    using std::vector;
    using std::string;
    using std::array;
    using std::tuple;

    template<class T, class = void>
    struct Serializer
    {
        static void Serialize(vector<uint8_t>& buffer, T&& in) = delete;
        static T DeSerialize(const uint8_t* in, size_t size) = delete;
        static const string_view Id;
    };

    template<>
    struct Serializer<void>
    {
        static const string_view Id;
    };

    template<class T>
    struct Serializer<T, std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, float>>>
    {
        static void Serialize(vector<uint8_t>& buffer, T in);

        static T DeSerialize(const uint8_t* in, size_t size);

        static const string_view Id;

    };

    template<>
    struct Serializer<string>
    {
        static string DeSerialize(const uint8_t* in, size_t size);

        static const string_view Id;
        
    };

    template<>
    struct Serializer<string_view>
    {
        static void Serialize(vector<uint8_t>& buffer, string_view in);

        static const string_view Id;
        
    };

    template<class T, class Alloc>
    struct Serializer<vector<T, Alloc>>
    {

        static void Serialize(std::vector<uint8_t>& buffer, vector<T, Alloc> in)
        {
            
        }

        static string DeSerialize(const uint8_t* in, size_t size)
        {
            return {};
        }

        static const string Id;

    };

    template<class... Args>
    struct Serializer<tuple<Args...>>
    {
        static void Serialize(vector<uint8_t>& buffer, tuple<Args...>& in)
        {
            
        }

        static string DeSerialize(const uint8_t* in, size_t size)
        {
            return {};
        }
        
        static const string Id;
        
    };

    template<class Fn>
    struct MangledSignature;

    template<class Result, class... Args>
    struct MangledSignature<Result(*)(Args...)>
    {
        static inline const string Id =
            sizeof...(Args) > 0 ?
            std::format("{}: {};", Serializer<Result>::Id, fmt::join(vector<string_view>{ Serializer<Args>::Id... }, " ")) :
            std::format("{}:;", Serializer<Result>::Id);
    };
    
}

#include "Serialization.inl"
