#pragma once
#include <type_traits>
#include <string_view>
#include <vector>
#include <charconv>
#include "_prog-multiply.hpp"
#include "_prog-simple-add.hpp"
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_parser
{
    typedef typename std::make_unsigned<type_t>::type u_type_t;

    bool read(std::vector<type_t>& num, std::string_view str, int base)
    {
        if (!(2 <= base && base <= 36))
            return false;

        if (str.size() == 0)
            return false;

        bool is_negative = false;

        if (str.size() && str[0] == '-')
            is_negative = true, str.remove_prefix(1);

        size_t base_count = 0;
        type_t base_multi = 1;

        for (type_t i = u_type_t(-1) >> 1; i / base; i /= base)
            base_multi *= base, base_count++;

        std::vector<type_t> base_num = {base_multi}, num_res = {};
        num = {};
        while (str.size()) {
            size_t this_count = std::min(base_count, str.size());

            u_type_t value = 0;
            auto ret = std::from_chars(&str[0],
                                       &str[0] + this_count, value, base);
            if (ret.ec != std::errc() || ret.ptr != &str[0] + this_count)
                return false;

            if (this_count != base_count)
                for (int i = base_count; i != this_count; i--)
                    base_num[0] /= base;

            prog_multiply<type_t, bigger_t> p = {};
            p.multiply(num_res, num, base_num);
            num = std::move(num_res);

            prog_simple_add<type_t, bigger_t> p_add = {};
            p_add.add(num, value);

            str.remove_prefix(this_count);
        }

        if (num.size() && num.back() < 0)
            num.push_back(0);

        if (is_negative) {
            prog_negate<type_t, bigger_t> p = {};
            p.negate(num);
        }

        return true;
    }
};

}
