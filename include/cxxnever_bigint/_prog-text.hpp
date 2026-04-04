#pragma once
#include <cstdint>
#include <string>
#include <charconv>
#include <type_traits>
#include "_prog-negate.hpp"
#include "_prog-divide-small.hpp"
#include "_prog-multiply-small.hpp"
#include "_prog-add.hpp"


namespace cxxnever::details
{

template<size_t Count, typename sign_t, typename type_t, typename bigger_t>
struct bigint_prog_text
{
    typedef typename std::make_signed<type_t>::type s_type_t;

    bigint_prog_negate<Count, type_t, bigger_t> prog_negate = {};
    bigint_prog_divide_small<Count, sign_t, type_t, bigger_t> prog_divide = {};
    bigint_prog_multiply_small<Count, type_t, bigger_t> prog_multiply = {};
    bigint_prog_add<Count, type_t, bigger_t> prog_add = {};

    std::string hex(const type_t (&num)[Count])
    {
        std::string r = {};
        r.reserve(3 * sizeof(type_t) * Count);

        const uint8_t* p = reinterpret_cast<const uint8_t*>(&num[0]);
        for (size_t i = sizeof(type_t) * Count; i--; ) {
            char buf[4];
            snprintf(buf, sizeof(buf), ":%02hhx", p[i]);
            r += buf + !r.size();
        }

        return r;
    }

    size_t maximum(int base)
    {
        size_t r = 1;
        for (type_t i = type_t(~type_t()) >> 1; i / base; i /= base)
            r++;

        return r * Count;
    }

    bool is_not_null(const type_t* num)
    {
        for (size_t i = 0; i != Count; i++)
            if (num[i])
                return true;

        return false;
    }

    std::string str(const type_t* num, int base)
    {
        bool is_negative = false;
        type_t number[Count];
        type_t divided[Count];

        memcpy(number, num, sizeof number);
        if constexpr (sign_t(-1) < 0)
            if (s_type_t(num[Count - 1]) < 0)
                prog_negate.negate(number), is_negative = true;

        std::string result = {};
        result.reserve(maximum(base) + is_negative);

        s_type_t ten = 1;
        s_type_t remainder = 0;
        for (type_t i = type_t(~type_t()) >> 1; i / base; i /= base)
            ten *= base;

        while (is_not_null(number)) {
            prog_divide.divide(divided, number, ten, &remainder);

            for (type_t i = type_t(~type_t()) >> 1; i / base; i /= base) {
                char buf[2] = {};
                std::to_chars(buf, buf + sizeof buf, remainder % base, base);
                result += buf;
                remainder /= base;
            }

            memcpy(number, divided, sizeof number);
        }

        while (result.size() && result.back() == '0')
            result.pop_back();

        if (result.empty())
            result.push_back('0');

        if (is_negative)
            result.push_back('-');

        for (size_t i = 0, j = result.size() - 1; i < j; i++, j--)
            std::swap(result[i], result[j]);

        return result;
    }

    type_t power(type_t n, size_t len)
    {
        type_t r = 1;
        while (len--)
            r *= n;

        return r;
    }

    bool parse(type_t* num, std::string_view input, int base)
    {
        memset(num, 0, sizeof(type_t) * Count);

        bool is_negative = false;
        if (input.size())
            if (input[0] == '-')
                is_negative = true, input.remove_prefix(1);

        size_t max_len = 0;
        for (type_t i = type_t(~type_t()) >> 1; i / base; i /= base)
            max_len++;

        while (input.size()) {
            type_t digit = 0;
            size_t len = std::min(max_len, input.size());
            auto err = std::from_chars(&input[0], &input[0] + len, digit, base);
            if (err.ec != std::errc())
                return false;
            input.remove_prefix(len);

            type_t number[Count] = {};
            prog_multiply.multiply(number, num, power(base, len));
            memcpy(num, number, sizeof number);
            prog_add.add(num, s_type_t(digit));
        }

        if (is_negative)
            prog_negate.negate(num);

        return true;
    }
};

}
