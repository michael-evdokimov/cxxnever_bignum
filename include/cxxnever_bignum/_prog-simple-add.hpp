#pragma once
#include <type_traits>
#include <vector>


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_simple_add
{
    typedef typename std::make_unsigned<type_t>::type u_type_t;

    void add(std::vector<type_t>& a, type_t b)
    {
        type_t a_sign = (a.size() && a.back() < 0) ? -1 : 0;
        type_t b_sign = (b < 0) ? -1 : 0;

        if (a.size() == 0)
            a.push_back(0);

        bigger_t carry = 0;
        carry += u_type_t(a[0]);
        carry += u_type_t(b);
        a[0] = carry;
        carry >>= sizeof(type_t) * 8;

        for (size_t i = 1; i < a.size(); i++) {
            carry += u_type_t(a[i]);
            carry += u_type_t(b_sign);
            a[i] = carry;
            carry >>= sizeof(type_t) * 8;
            if (carry == 0 && b_sign == 0)
                break;
        }

        if (carry)
            if (a_sign == 0 && b_sign == 0)
                a.push_back(carry);

        if (a_sign == b_sign)
            if (a_sign != ((a.size() && a.back() < 0) ? -1 : 0))
                a.push_back(a_sign);

        while (a.size() >= 2 && a.back() == -1 && a[a.size() - 2] < 0)
            a.pop_back();

        while (a.size() >= 2 && a.back() == 0 && a[a.size() - 2] >= 0)
            a.pop_back();

        if (a.size() == 1 && a.back() == 0)
            a.pop_back();
    }
};

}
