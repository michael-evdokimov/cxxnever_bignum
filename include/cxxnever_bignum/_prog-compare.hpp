#pragma once
#include <type_traits>
#include <vector>


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_compare
{
    typedef typename std::make_unsigned<type_t>::type u_type_t;

    int compare(const std::vector<type_t>& a, const std::vector<type_t>& b)
    {
        int a_sign = !!a.size() * ((a.size() && a.back() < 0) ? -1 : +1);
        int b_sign = !!b.size() * ((b.size() && b.back() < 0) ? -1 : +1);

        if (a_sign != b_sign)
            return a_sign < b_sign ? -1 : +1;

        if (a.size() != b.size())
            return (a.size() < b.size() ? -1 : +1) * a_sign;

        if (size_t i = a.size()) {
            i--;
            if (a[i] != b[i])
                return a[i] < b[i] ? -1 : +1;

            while (i--)
                if (a[i] != b[i])
                    return u_type_t(a[i]) < u_type_t(b[i]) ? -1 : +1;
        }

        return 0;
    }
};

}
