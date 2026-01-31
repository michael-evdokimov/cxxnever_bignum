#pragma once
#include <type_traits>
#include <vector>


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_addition
{
    typedef typename std::make_unsigned<type_t>::type u_type_t;

    void add(std::vector<type_t>& a, const std::vector<type_t>& b)
    {
        type_t a_sign = (a.size() && a.back() < 0) ? -1 : 0;
        type_t b_sign = (b.size() && b.back() < 0) ? -1 : 0;

        bigger_t carry = 0;
        for (size_t i = 0; i < a.size() || i < b.size(); i++) {
            carry += u_type_t((i < a.size()) ? a[i] : a_sign);
            carry += u_type_t((i < b.size()) ? b[i] : b_sign);
            if (i == a.size())
                a.push_back(0);
            a[i] = carry;
            carry >>= sizeof(type_t) * 8;
            if (carry == 0 && i >= b.size())
                break;
        }

        if (carry)
            if (a_sign == 0 && b_sign == 0)
                a.push_back(carry);

        if (a_sign == b_sign)
            if (b_sign != ((a.size() && a.back() < 0) ? -1 : 0))
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
