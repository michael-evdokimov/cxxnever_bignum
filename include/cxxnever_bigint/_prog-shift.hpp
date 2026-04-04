#pragma once
#include <cstddef>
#include <type_traits>


namespace cxxnever::details
{

template<size_t Count, typename sign_t, typename type_t, typename bigger_t>
struct bigint_prog_shift
{
    typedef typename std::make_signed<type_t>::type s_type_t;

    void shift_left(type_t* num, size_t index)
    {
        size_t hi = index % (8 * sizeof(type_t));
        size_t lo = 8 * sizeof(type_t) - hi;
        type_t* p = num + Count - 1;
        type_t* s = p - index / (8 * sizeof(type_t));
        while (s > num)
            *p = (*s << hi) | (s[-1] >> lo), p--, s--;
        if (s >= num)
            *p-- = *s-- << hi;
        while (p >= num)
            *p-- = 0;
    }

    void shift_right(type_t* num, size_t index)
    {
        size_t lo = index % (8 * sizeof(type_t));
        size_t hi = 8 * sizeof(type_t) - lo;
        type_t* p = num;
        type_t* s = p + index / (8 * sizeof(type_t));

        while (s < num + Count - 1)
            *p = (*s >> lo) | (s[1] << hi), p++, s++;

        if constexpr(sign_t(-1) > 0) {
            if (s < num + Count)
                *p++ = *s++ >> lo;
            while (p < num + Count)
                *p++ = 0;
        } else {
            if (s < num + Count)
                *p++ = s_type_t(*s++) >> lo;
            s_type_t sign = num[Count - 1];
            sign >>= 8 * sizeof(type_t) - 1;
            while (p < num + Count)
                *p++ = sign;
          }
    }
};

}
