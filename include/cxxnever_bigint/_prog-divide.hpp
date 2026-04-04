#pragma once
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include "_prog-add.hpp"
#include "_prog-compare.hpp"
#include "_prog-shift.hpp"
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<size_t Count, typename sign_t, typename type_t, typename bigger_t>
struct bigint_prog_divide
{
    typedef typename std::make_signed<type_t>::type s_type_t;

    bigint_prog_compare<Count, sign_t, type_t, bigger_t> prog_compare = {};
    bigint_prog_shift<Count, sign_t, type_t, bigger_t> prog_shift = {};
    bigint_prog_add<Count, type_t, bigger_t> prog_add = {};
    bigint_prog_negate<Count, type_t, bigger_t> prog_negate = {};

    void memset_(type_t* ptr, int byte, size_t size)
    {
        if (ptr)
            memset(ptr, byte, size);
    }

    size_t throw_if_null(size_t r)
    {
        if (r == 0)
            throw std::runtime_error("division by zero");

        return r;
    }

    size_t highest(const type_t* num)
    {
        for (size_t i = Count; i--; )
            if (type_t n = num[i])
                for (size_t j = 1; ; j++)
                    if ((n >>= 1) == 0)
                        return i * 8 * sizeof(type_t) + j;

        return 0;
    }

    void setbit(type_t* num, size_t index)
    {
        if (num) {
            num += index / (8 * sizeof(type_t));
            *num |= type_t(1) << index % (8 * sizeof(type_t));
        }
    }

    void
    __div(type_t* result, const type_t* a, const type_t* b, type_t* remainder)
    {
        type_t sum[Count];
        memcpy(sum, a, sizeof(type_t) * Count);
        memset_(result, 0, sizeof(type_t) * Count);

        size_t highest_of_item = throw_if_null(highest(b));
        while (true) {
            type_t item[Count];
            memcpy(item, b, sizeof(type_t) * Count);
            size_t a_idx = highest(sum);
            size_t b_idx = highest_of_item;
            if (a_idx < b_idx)
                break;
            size_t index = a_idx - b_idx;
            prog_shift.shift_left(item, index);
            int cmp_r = prog_compare.compare(sum, item);
            if (cmp_r >= 0) {
                setbit(result, index);
                prog_add.sub(sum, item);
            }
            if (cmp_r == 0 || index == 0)
                break;
            if (cmp_r < 0) {
                setbit(result, --index);
                prog_shift.shift_right(item, 1);
                prog_add.sub(sum, item);
            }
        }

        if (remainder)
            memcpy(remainder, sum, sizeof(type_t) * Count);
    }

    struct negative
    {
        type_t num[Count];

        negative(const type_t* src)
        {
            memcpy(num, src, sizeof(num));
            bigint_prog_negate<Count, type_t, bigger_t>().negate(num);
        }
    };

    s_type_t sign(const type_t* num)
    {
        if constexpr(sign_t(-1) > 0)
            return +1;

        return s_type_t(num[Count - 1]) < 0 ? -1 : +1;
    }

    void __neg(type_t* num)
    {
        if (num)
            prog_negate.negate(num);
    }

    void
    divide(type_t* result, const type_t* a, const type_t* b, type_t* remainder)
    {
        s_type_t a_sign = sign(a);
        s_type_t b_sign = sign(b);

        if (a_sign == 1 && b_sign == 1)
            return __div(result, a, b, remainder);

        if (a_sign == -1 && b_sign == 1) {
            __div(result, negative(a).num, b, remainder);
            __neg(result);
            __neg(remainder);
        }

        if (a_sign == 1 && b_sign == -1) {
            __div(result, a, negative(b).num, remainder);
            __neg(result);
        }

        if (a_sign == -1 && b_sign == -1) {
            __div(result, negative(a).num, negative(b).num, remainder);
            __neg(remainder);
        }

    }
};

}
