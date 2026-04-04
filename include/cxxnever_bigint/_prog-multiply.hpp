#pragma once
#include <cstddef>


namespace cxxnever::details
{

template<size_t Count, typename type_t, typename bigger_t>
struct bigint_prog_multiply
{
    void multiply(type_t (&r)[Count], const type_t* a, const type_t* b)
    {
        for (size_t i = 0; i != Count; i++) {
            for (size_t j = 0; j != Count; j++) {
                bigger_t x = bigger_t(a[i]) * bigger_t(b[j]);

                for (size_t k = i + j; k < Count && x; k++) {
                    x += bigger_t(r[k]);
                    r[k] = x;
                    x >>= 8 * sizeof(type_t);
                }
            }
        }
    }
};

}
