#pragma once
#include <cstddef>
#include <type_traits>
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<size_t Count, typename sign_t, typename type_t, typename bigger_t>
struct bigint_prog_divide_small
{
    typedef typename std::make_signed<type_t>::type s_type_t;

    bigint_prog_negate<Count, type_t, bigger_t> prog_negate = {};

    void __div(type_t* result, const type_t* a, type_t b, s_type_t* remainder)
    {
        bigger_t carry = 0;
        size_t r_idx = Count;

        for (size_t i = Count; i--; ) {
            carry <<= 8 * sizeof(type_t);
            carry += a[i];

            if (result)
                result[--r_idx] = carry / b;

            carry %= b;
        }

        if (result)
            for (size_t i = 0; i != Count; i++)
                result[i] = (r_idx != Count) ? result[r_idx++] : 0;

        if (remainder)
            *remainder = carry;
    }

    struct negative
    {
        type_t num[Count];

        negative(const type_t* src)
        {
            memcpy(num, src, sizeof num);
            bigint_prog_negate<Count, type_t, bigger_t>().negate(num);
        }
    };

    void __neg(type_t* num)
    {
        if (num)
            prog_negate.negate(num);
    }

    void __neg(s_type_t* n)
    {
        if (n)
            *n *= -1;
    }

    void divide(type_t* result, const type_t* a, s_type_t b, s_type_t* remainder)
    {
        int a_sign = (sign_t(-1) < 0 && s_type_t(a[Count - 1]) < 0) ? -1 : +1;
        int b_sign = (b < 0) ? -1 : +1;

        if (result)
            memset(result, 0, sizeof(type_t) * Count);

        if (a_sign == 1 && b_sign == 1)
            return __div(result, a, b, remainder);

        if (a_sign == -1 && b_sign == 1) {
            __div(result, negative(a).num, b, remainder);
            __neg(result);
            __neg(remainder);
        }

        if (a_sign == 1 && b_sign == -1) {
            __div(result, a, b * -1, remainder);
            __neg(result);
        }

        if (a_sign == -1 && b_sign == -1) {
            __div(result, negative(a).num, b * -1, remainder);
            __neg(remainder);
        }
    }
};

}
