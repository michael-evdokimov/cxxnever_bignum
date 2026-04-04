#pragma once
#include <stdexcept>
#include <vector>
#include "_prog-addition.hpp"
#include "_prog-subtract.hpp"
#include "_prog-compare.hpp"
#include "_prog-shift.hpp"
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_divide
{
    typedef std::vector<type_t> number;

    prog_compare<type_t, bigger_t> p_cmp = {};
    prog_addition<type_t, bigger_t> p_add = {};
    prog_subtract<type_t, bigger_t> p_sub = {};
    prog_shift<type_t, bigger_t> p_shift = {};
    prog_negate<type_t, bigger_t> p_neg = {};

    std::vector<type_t> sum = {};
    std::vector<type_t> item = {};

    void setbit(std::vector<type_t>& num, size_t n)
    {
        size_t i = n / (sizeof(type_t) * 8);
        size_t j = n % (sizeof(type_t) * 8);

        if (num.size() < i + 1)
            num.resize(i + 1);

        num[i] |= type_t(1) << j;

        if (num.back() < 0)
            num.push_back(0);
    }

    void setbit(std::vector<type_t>* num, size_t n)
    {
        if (num)
            setbit(*num, n);
    }

    size_t length(const std::vector<type_t>& n)
    {
        size_t r = n.size() ? (n.size() - 1) * sizeof(type_t) * 8 : 0;

        if (n.size())
            if (type_t v = n.back())
                while (v)
                    v >>= 1, r++;

        return r;
    }

    void copy_make_positive(number& r, const number& num)
    {
        r = num;
        if (r.size())
            if (r.back() < 0)
                p_neg.negate(r);
    }

    void
    __div(number* result, const number& a, const number& b, number* remainder)
    {
        copy_make_positive(sum, a);
        copy_make_positive(item, b);

        size_t length_of_item = length(item);
        size_t last = 0;
        while (true) {
            size_t a_idx = length(sum);
            size_t b_idx = length_of_item;
            if (a_idx < b_idx)
                break;
            size_t index = a_idx - b_idx;
            if (index >= last)
                p_shift.shift_left(item, index - last);
            else
                p_shift.shift_right(item, last - index);
            int r_cmp = p_cmp.compare(sum, item);
            if (r_cmp >= 0) {
                setbit(result, index);
                p_sub.subtract(sum, item);
            }
            if (r_cmp == 0)
                break;
            if (r_cmp < 0) {
                if (index-- == 0)
                    break;
                setbit(result, index);
                p_shift.shift_right(item, 1);
                p_sub.subtract(sum, item);
            }
            last = index;
        }

        if (remainder)
            *remainder = sum;
    }

    void
    divide(number* result, const number& a, const number& b, number* remainder)
    {
        int a_sign = (a.size() && a.back() < 0) ? -1 : +1;
        int b_sign = (b.size() && b.back() < 0) ? -1 : +1;

        if (b.size() == 0)
            throw std::runtime_error("division by zero");

        if (result)
            *result = {};

        __div(result, a, b, remainder);

        if (a_sign * b_sign == -1)
            if (result)
                p_neg.negate(*result);

        if (a_sign == -1)
            if (remainder)
                p_neg.negate(*remainder);
    }

    void
    __div_old(number* result, const number& a, const number& b, number* remainder)
    {
        size_t a_len = length(a);
        size_t b_len = length(b);

        if (result)
            *result = {};

        if (a_len < b_len) {
            if (remainder)
                *remainder = a;
            return;
        }

        sum = {};
        item = b;

        size_t index = a_len - b_len + 1;
        p_shift.shift_left(item, index - 1);
        int r_cmp = p_cmp.compare(a, item);
        if (r_cmp < 0) {
            p_shift.shift_right(item, 1);
            index--;
        }

        while (index) {
            p_add.add(sum, item);
            int r_cmp = p_cmp.compare(a, sum);
            if (r_cmp >= 0)
                if (result)
                    setbit(*result, index - 1);
            if (r_cmp == 0)
                break;
            if (r_cmp < 0)
                p_sub.subtract(sum, item);
            index--;
            if (index)
                p_shift.shift_right(item, 1);
        }

        if (remainder) {
            *remainder = a;
            p_sub.subtract(*remainder, sum);
        }
    }

    void
    divide_old(number* result, const number& a, const number& b, number* remainder)
    {
        type_t a_sign = (a.size() && a.back() < 0) ? -1 : 0;
        type_t b_sign = (b.size() && b.back() < 0) ? -1 : 0;

        if (b.size() == 0)
            throw std::runtime_error("division by zero");

        if (a_sign == 0 && b_sign == 0)
            return __div(result, a, b, remainder);

        if (a_sign == 0 && b_sign == -1) {
            auto b_copy = b;
            p_neg.negate(b_copy);
            __div(result, a, b_copy, remainder);
            if (result)
                p_neg.negate(*result);
            return;
        }

        if (a_sign == -1 && b_sign == 0) {
            auto a_copy = a;
            p_neg.negate(a_copy);
            __div(result, a_copy, b, remainder);
            if (result)
                p_neg.negate(*result);
            if (remainder)
                p_neg.negate(*remainder);
            return;
        }

        if (a_sign == -1 && b_sign == -1) {
            auto a_copy = a;
            auto b_copy = b;
            p_neg.negate(a_copy);
            p_neg.negate(b_copy);
            __div(result, a_copy, b_copy, remainder);
            if (remainder)
                p_neg.negate(*remainder);
            return;
        }
    }
};

}
