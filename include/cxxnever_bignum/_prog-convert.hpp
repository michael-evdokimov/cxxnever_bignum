#pragma once
#include <vector>


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_convert
{
    void convert(std::vector<type_t>& num, long long n)
    {
        num = {};

        while (n && n != -1) {
            type_t x = n;
            num.push_back(x);
            n >>= sizeof(type_t) * 8;
        }

        if (num.size() == 0 && n == -1)
            num.push_back(-1);

        if (num.size() && num.back() < 0 && n == 0)
            num.push_back(0);

        if (num.size() && num.back() >= 0 && n == -1)
            num.push_back(-1);
    }
};

}
