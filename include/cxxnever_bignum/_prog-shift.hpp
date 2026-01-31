#pragma once
#include <type_traits>
#include <vector>


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_shift
{
    typedef typename std::make_unsigned<type_t>::type u_type_t;

    void shift_left(std::vector<type_t>& num, size_t n)
    {
        if (num.size() == 0)
            return;

        size_t zero_count = n / (sizeof(type_t) * 8);
        if (zero_count)
            num.insert(num.begin(), zero_count, 0);
        n -= zero_count * sizeof(type_t) * 8;

        if (n == 0)
            return;

        type_t sign = num.back() < 0 ? -1 : 0;

        type_t next = 0;
        for (size_t i = zero_count; i < num.size(); i++) {
            bigger_t x = num[i];
            x <<= n;
            x |= next;
            num[i] = x;
            x <<= sizeof(type_t) * 8 - n;
            x >>= sizeof(type_t) * 8 - n + sizeof(type_t) * 8;
            next = x;
        }

        if (sign == -1) {
            next <<= sizeof(type_t) * 8 - n;
            next >>= sizeof(type_t) * 8 - n;
        }

        if (next && next != -1)
            num.push_back(next);

        if (sign != (num.back() < 0 ? -1 : 0))
            num.push_back(sign);
    }

    void shift_right(std::vector<type_t>& num, size_t n)
    {
        type_t sign = (num.size() && num.back() < 0) ? -1 : 0;

        size_t remove_count = n / (sizeof(type_t) * 8);
        if (remove_count)
            num.erase(num.begin(), num.begin() + std::min(remove_count,
                                                          num.size()));
        n -= remove_count * sizeof(type_t) * 8;

        if (num.size() && n) {
            for (size_t i = 0; i + 1 < num.size(); i++) {
                num[i] = u_type_t(num[i]) >> n;
                num[i] |= u_type_t(num[i + 1]) << (sizeof(type_t) * 8 - n);
            }

            num.back() >>= n;
        } else {
            if (sign == -1 && num.size() == 0)
                num.push_back(-1);
        }

        if (num.size() >= 2)
            if (auto* p = &num[num.size() - 2])
                if (num.back() == 0 && *p >= 0 || num.back() == -1 && *p < 0)
                    num.pop_back();

        if (num.size() == 1 && num.back() == 0)
            num.resize(0);
    }
};

}
