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

    size_t length(const std::vector<type_t>& n)
    {
        size_t r = n.size() ? (n.size() - 1) * sizeof(type_t) * 8 : 0;

        if (n.size())
            if (type_t v = n.back())
                while (v)
                    v >>= 1, r++;

        return r;
    }

    void __divide(std::vector<type_t>* result,
                  const std::vector<type_t>& a,
                  const std::vector<type_t>& b,
                  std::vector<type_t>* remainder)
    {
        prog_compare<type_t, bigger_t> p_cmp = {};
        prog_addition<type_t, bigger_t> p_add = {};
        prog_subtract<type_t, bigger_t> p_sub = {};
        prog_shift<type_t, bigger_t> p_shift = {};

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

    void divide(std::vector<type_t>* result,
                const std::vector<type_t>& a,
                const std::vector<type_t>& b,
                std::vector<type_t>* remainder = nullptr)
    {
        prog_negate<type_t, bigger_t> p_neg = {};

        type_t a_sign = (a.size() && a.back() < 0) ? -1 : 0;
        type_t b_sign = (b.size() && b.back() < 0) ? -1 : 0;

        if (b.size() == 0)
            throw std::runtime_error("division by zero");

        if (a_sign == 0 && b_sign == 0)
            return __divide(result, a, b, remainder);

        if (a_sign == 0 && b_sign == -1) {
            auto b_copy = b;
            p_neg.negate(b_copy);
            __divide(result, a, b_copy, remainder);
            if (result)
                p_neg.negate(*result);
            return;
        }

        if (a_sign == -1 && b_sign == 0) {
            auto a_copy = a;
            p_neg.negate(a_copy);
            __divide(result, a_copy, b, remainder);
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
            __divide(result, a_copy, b_copy, remainder);
            if (remainder)
                p_neg.negate(*remainder);
            return;
        }
    }
};

}
