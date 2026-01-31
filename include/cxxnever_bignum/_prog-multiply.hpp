#pragma once
#include <type_traits>
#include <vector>
#include "_prog-negate.hpp"


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct prog_multiply
{
    typedef typename std::make_unsigned<type_t>::type u_type_t;

    void multiply(std::vector<type_t>& r,
                  const std::vector<type_t>& a,
                  const std::vector<type_t>& b)
    {
        const bool A = a.size() && a.back() < 0;
        const bool B = b.size() && b.back() < 0;

        r = {};
        r.reserve(a.size() + b.size() + 1);

        type_t a_one = 1;
        for (size_t i = 0; i != a.size(); i++) {
            type_t a_i = A ? ~a[i] + a_one : a[i];
            a_one &= a_i == 0;

            type_t b_one = 1;
            for (size_t j = 0; j != b.size(); j++) {
                type_t b_j = B ? ~b[j] + b_one : b[j];
                b_one &= b_j == 0;

                auto x = bigger_t(u_type_t(a_i)) * bigger_t(u_type_t(b_j));

                for (size_t k = i + j; x; k++) {
                    if (r.size() < k + 1)
                        r.resize(k + 1);
                    x += u_type_t(r[k]);
                    r[k] = x;
                    x >>= sizeof(type_t) * 8;
                }
            }
        }

        if (r.size() && r.back() < 0)
            r.push_back(0);

        if (A ^ B) {
            prog_negate<type_t, bigger_t> p = {};
            p.negate(r);
        }
    }
};

}
