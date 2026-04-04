#pragma once
#include <type_traits>
#include <cstddef>


namespace cxxnever::details
{

template<size_t Count, typename sign_t, typename type_t, typename bigger_t>
struct bigint_prog_compare
{
    typedef typename std::make_signed<type_t>::type s_type_t;

    int compare(const type_t* a, const type_t* b)
    {
        if constexpr(sign_t(-1) < 0) {
            int a_sign = s_type_t(a[Count - 1]) < 0 ? -1 : +1;
            int b_sign = s_type_t(b[Count - 1]) < 0 ? -1 : +1;
            if (a_sign != b_sign)
                return a_sign < b_sign ? -1 : +1;
        }
        for (size_t i = Count; i--; )
            if (a[i] != b[i])
                return a[i] < b[i] ? -1 : +1;
        return 0;
    }
};

}
