#pragma once
#include <vector>


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_negate
{
    void negate(std::vector<type_t>& num)
    {
        for (type_t& x: num)
            x = ~x;

        for (size_t i = 0; i + 2 < num.size(); ++i)
            if (++num[i])
                return;

        if (2 <= num.size()) {
            type_t next = ++num[num.size() - 2];
            if (next != 0) {
                if (next < 0)
                    if (num.back() == -1)
                        num.pop_back();
                return;
            }
        }

        if (1 <= num.size()) {
            type_t before = num.back();
            type_t after = ++num.back();
            if (before >= 0 && after < 0)
                num.push_back(0);
        }
    }
};

}
