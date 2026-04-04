#pragma once
#include <vector>
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_divide_small
{
    typedef std::vector<type_t> number;

    typedef std::make_unsigned<type_t>::type u_type_t;

    prog_negate<type_t, bigger_t> p_neg = {};

    void __div(number* result, const number& a, u_type_t b, type_t* remainder)
    {
        bigger_t carry = 0;
        if (result)
            *result = {};

        for (size_t i = a.size(); i--; ) {
            carry <<= 8 * sizeof(type_t);
            carry += u_type_t(a[i]);

            if (result)
                result->push_back(carry / b);

            carry %= b;
        }

        if (result) {
            if (result->size())
                for (size_t i = 0, j = result->size() - 1; i < j; i++, j--)
                    std::swap((*result)[i], (*result)[j]);

            while (result->size() && result->back() == 0)
                result->pop_back();

            if (result->size() && result->back() < 0)
                result->push_back(0);
        }

        if (remainder)
            *remainder = carry;
    }

    void divide(number* result, const number& a, type_t b, type_t* remainder)
    {
        int a_sign = (a.size() && a.back() < 0) ? -1 : +1;
        int b_sign = (b < 0) ? -1 : +1;

        u_type_t b_copy = (b_sign == +1) ? b : b * -1;

        if (a_sign == +1) {
            __div(result, a, b_copy, remainder);
        } else {
            number a_copy = a;
            p_neg.negate(a_copy);
            __div(result, a_copy, b_copy, remainder);
        }

        if (a_sign * b_sign == -1)
            if (result)
                p_neg.negate(*result);

        if (a_sign == -1)
            if (remainder)
                *remainder *= -1;
    }
};

}
