#pragma once
#include <cstddef>


namespace cxxnever::details
{

template<size_t Count, typename type_t, typename bigger_t>
struct bigint_prog_multiply_small
{
    void multiply(type_t* result, const type_t* a, type_t b)
    {
        for (size_t i = 0; i != Count; i++) {
            bigger_t x = bigger_t(a[i]) * b;

            for (size_t j = i; j != Count && x; j++) {
                x += result[j];
                result[j] = x;
                x >>= 8 * sizeof(type_t);
            }
        }
    }
};

}
