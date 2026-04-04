#pragma once
#include <cstddef>


namespace cxxnever::details
{

template<size_t Count, typename type_t, typename bigger_t>
struct bigint_prog_negate
{
    void negate(type_t* num)
    {
        for (size_t i = 0; i != Count; i++)
            num[i] = ~num[i];

        for (size_t i = 0; i != Count; i++)
            if (++num[i])
                return;
    }
};

}
