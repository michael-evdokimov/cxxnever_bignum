#pragma once
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <string>
#include <charconv>
#include <cstdint>
#include "_prog-divide.hpp"
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_print
{
    typedef typename std::make_unsigned<type_t>::type u_type_t;

    std::string str_2_sign(const std::vector<type_t>& num, int sign)
    {
        std::string r = {};
        r.reserve(1 + num.size() * sizeof(type_t) * 8);
        if (sign == -1)
            r += "-";

        bool present = false;
        for (size_t i = num.size(); i--; ) {
            for (int j = sizeof(type_t) * 8; j--; ) {
                bool f = num[i] & (type_t(1) << j);
                present |= f;
                if (present)
                    r += f ? "1" : "0";
            }
        }
        if (!present)
            r += "0";

        return r;
    }

    std::string str_16_sign(const std::vector<type_t>& num, int sign)
    {
        std::string r = {};
        r.reserve(1 + num.size() * sizeof(type_t) * 2);
        if (sign == -1)
            r += "-";

        bool present = false;
        for (size_t i = num.size(); i--; ) {
            for (int j = sizeof(type_t); j--; ) {
                uint8_t v = num[i] >> (j * 8);
                if (present || v) {
                    char buf[3] = {};
                    std::to_chars(buf, buf + sizeof(buf), v, 16);
                    if (buf[1] == 0 && present)
                        buf[2] = buf[1], buf[0] = '0';
                    r += buf;
                }
                present |= bool(v);
            }
        }
        if (!present)
            r += "0";

        return r;
    }

    std::string str_2(const std::vector<type_t>& num)
    {
        if (num.size() && num.back() < 0) {
            auto copy = num;
            prog_negate<type_t, bigger_t> p = {};
            p.negate(copy);
            return str_2_sign(copy, -1);
        }
        return str_2_sign(num, 0);
    }

    std::string str_16(const std::vector<type_t>& num)
    {
        if (num.size() && num.back() < 0) {
            auto copy = num;
            prog_negate<type_t, bigger_t> p = {};
            p.negate(copy);
            return str_16_sign(copy, -1);
        }
        return str_16_sign(num, 0);
    }

    std::string str_n(const std::vector<type_t>& number, int base)
    {
        std::string r = {};
        std::vector<type_t> num = number, result, remainder;

        if (num.size() && num.back() < 0) {
            prog_negate<type_t, bigger_t> p = {};
            p.negate(num);
            r += "-";
        }

        type_t ten = 1;
        int base_count = 0;
        for (u_type_t i = u_type_t(-1) >> 1; i / base; i /= base)
            ten *= base, base_count++;

        std::vector<type_t> ten_num = {ten};
        prog_divide<type_t, bigger_t> p = {};
        while (num.size()) {
            p.divide(&result, num, ten_num, &remainder);
            type_t digits = remainder.size() ? remainder[0] : 0;
            for (int i = 0; i != base_count; i++, digits /= base) {
                char buf[2] = {};
                std::to_chars(buf, buf + sizeof(buf), digits % base, base);
                r += buf;
            }
            num = std::move(result);
        }

        while (r.size() && r.back() == '0')
            r.pop_back();

        if (r == "")
            r = "0";

        for (char* a = &r[0] + (r[0] == '-'), *b = &r.back(); a < b; a++, b--)
            std::swap(*a, *b);

        return r;
    }

    std::string str(const std::vector<type_t>& num, int base)
    {
        if (base == 2)
            return str_2(num);

        if (base == 16)
            return str_16(num);

        if (2 <= base && base <= 36)
            return str_n(num, base);

        throw std::runtime_error("unknown base");
    }
};

}
