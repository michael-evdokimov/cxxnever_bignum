#pragma once
#include <vector>
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_karatsuba
{
    typedef typename std::make_signed<bigger_t>::type s_bigger_t;
    typedef typename std::make_unsigned<type_t>::type u_type_t;
    typedef std::vector<type_t> number;

    number __cut(const number& num, size_t left, size_t right)
    {
        number r = {};
        if (left <= num.size())
            r.insert(r.begin(), num.begin() + right, num.begin() + left);
        else if (right < num.size())
            r.insert(r.begin(), num.begin() + right, num.end());
        return r;
    }

    void __add(number& a, const number& b)
    {
        bigger_t carry = 0;
        for (size_t i = 0; i < a.size() || i < b.size(); i++) {
            carry += (i < a.size()) ? u_type_t(a[i]) : 0;
            carry += (i < b.size()) ? u_type_t(b[i]) : 0;
            if (i == a.size())
                a.push_back(0);
            a[i] = carry;
            carry >>= sizeof(type_t) * 8;
            if (carry == 0 && i >= b.size())
                break;
        }
        if (carry)
            a.push_back(carry);
    }

    void __subtract(number& a, const number& b)
    {
        bigger_t carry = 0;
        for (size_t i = 0; i < a.size() || i < b.size(); i++) {
            s_bigger_t x = 0;
            x += (i < a.size()) ? u_type_t(a[i]) : 0;
            x -= (i < b.size()) ? u_type_t(b[i]) : 0;
            x -= carry;
            carry = 0;
            if (x < 0)
                x += bigger_t(1) << (sizeof(type_t) * 8), carry = 1;
            if (i == a.size())
                a.push_back(0);
            a[i] = x;
            if (carry == 0 && i >= b.size())
                break;
        }
        while (a.size() && a.back() == 0)
            a.pop_back();
    }

    void __shift_left(number& num, size_t n)
    {
        if (num.size())
            num.insert(num.begin(), n, type_t(0));
    }

    number __multiply_simple(const number& a, const number& b)
    {
        number r = {};
        bigger_t x = 1;
        x *= a.size() ? bigger_t(u_type_t(a[0])) : 0;
        x *= b.size() ? bigger_t(u_type_t(b[0])) : 0;
        if (x)
            r.push_back(x), x >>= sizeof(type_t) * 8;
        if (x)
            r.push_back(x);
        return r;
    }

    number __multiply(const number& a, const number& b)
    {
        size_t length = std::max(a.size(), b.size());
        if (length == 1)
            return __multiply_simple(a, b);

        number a_left = __cut(a, length, length / 2);
        number a_right = __cut(a, length / 2, 0);
        number b_left = __cut(b, length, length / 2);
        number b_right = __cut(b, length / 2, 0);

        number z2 = __multiply(a_left, b_left);
        number z0 = __multiply(a_right, b_right);

        number a_sum = a_left;
        number b_sum = b_left;
        __add(a_sum, a_right);
        __add(b_sum, b_right);
        number z1 = __multiply(a_sum, b_sum);
        __subtract(z1, z2);
        __subtract(z1, z0);

        __shift_left(z2, length / 2 * 2);
        __shift_left(z1, length / 2);

        number result = {};
        __add(result, z2);
        __add(result, z1);
        __add(result, z0);

        return result;
    }

    void __multiply(number& r, const number& a, const number& b)
    {
        r = __multiply(a, b);
        if (r.size() && r.back() < 0)
            r.push_back(0);
    }

    void multipy(std::vector<type_t>& r,
                 const std::vector<type_t>& a,
                 const std::vector<type_t>& b)
    {
        prog_negate<type_t, bigger_t> p = {};

        type_t a_sign = (a.size() && a.back() < 0) ? -1 : 0;
        type_t b_sign = (b.size() && b.back() < 0) ? -1 : 0;

        if (a_sign == 0 && b_sign == 0)
            return __multiply(r, a, b);
        if (a_sign == 0 && b_sign == -1) {
            auto b_copy = b;
            p.negate(b_copy);
            __multiply(r, a, b_copy);
            p.negate(r);
            return;
        }
        if (a_sign == -1 && b_sign == 0) {
            auto a_copy = a;
            p.negate(a_copy);
            __multiply(r, a_copy, b);
            p.negate(r);
            return;
        }
        if (a_sign == -1 && b_sign == -1) {
            auto a_copy = a;
            auto b_copy = b;
            p.negate(a_copy);
            p.negate(b_copy);
            __multiply(r, a_copy, b_copy);
            return;
        }
    }
};

}
