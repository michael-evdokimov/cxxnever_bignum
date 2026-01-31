#pragma once
#include <type_traits>
#include <string_view>
#include <vector>
#include <charconv>
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_parser16
{
    typedef typename std::make_unsigned<type_t>::type u_type_t;

    bool read(std::vector<type_t>& num, std::string_view str)
    {
        if (str.size() == 0)
            return false;

        num = {};
        num.reserve(str.size() / 2 / sizeof(type_t) + 1 + 1);

        while (str.size() && str != "-") {
            int this_count = std::min(sizeof(type_t) * 2, str.size());
            this_count -= str[str.size() - this_count] == '-';
            if (this_count == 0)
                return false;

            u_type_t value = 0;
            const char* from = &str[0] + str.size() - this_count;
            auto ret = std::from_chars(from, from + this_count, value, 16);
            if (ret.ec != std::errc() || ret.ptr != from + this_count)
                return false;

            num.push_back(value);
            str.remove_suffix(this_count);
        }

        while (num.size() && num.back() == 0)
            num.pop_back();

        if (num.size() && num.back() < 0)
            num.push_back(0);

        if (str == "-") {
            prog_negate<type_t, bigger_t> p = {};
            p.negate(num);
        }

        return true;
    }
};

}
