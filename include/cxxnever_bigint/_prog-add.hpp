#pragma once
#include <type_traits>
#include <cstddef>


namespace cxxnever::details
{

template<size_t Count, typename type_t, typename bigger_t>
struct bigint_prog_add
{
    typedef typename std::make_signed<type_t>::type s_type_t;

    void add(type_t (&a)[Count], const type_t (&b)[Count])
    {
        bigger_t carry = 0;
        for (size_t i = 0; i != Count; i++) {
            carry += a[i];
            carry += b[i];
            a[i] = carry;
            carry >>= 8 * sizeof(type_t);
        }
    }

    void sub(type_t (&a)[Count], const type_t (&b)[Count])
    {
        bigger_t carry = 0;
        type_t one = 1;
        for (size_t i = 0; i != Count; i++) {
            type_t b_i = ~b[i] + one;
            one &= b_i == 0;
            carry += a[i];
            carry += b_i;
            a[i] = carry;
            carry >>= 8 * sizeof(type_t);
        }
    }

    void add(type_t* a, s_type_t b)
    {
        type_t b_sign = (b < 0) ? -1 : 0;

        bigger_t carry = 0;
        carry += a[0];
        carry += type_t(b);
        a[0] = carry;
        carry >>= 8 * sizeof(type_t);

        size_t i = 0;
        while (++i < Count) {
            carry += a[i];
            carry += b_sign;
            a[i] = carry;
            carry >>= 8 * sizeof(type_t);
            if (carry == 0 && b_sign == 0)
                break;
        }
    }
};

}
