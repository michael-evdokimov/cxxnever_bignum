#pragma once
#include <cstddef>
#include "_prog-add.hpp"
#include "_prog-text.hpp"
#include "_prog-negate.hpp"
#include "_prog-multiply.hpp"
#include "_prog-multiply-small.hpp"
#include "_prog-compare.hpp"
#include "_prog-shift.hpp"
#include "_prog-divide.hpp"
#include "_prog-divide-small.hpp"


namespace cxxnever::details
{

template<size_t Count, typename sign_t, typename type_t, typename bigger_t>
struct integer_layer
{
    typedef integer_layer layer;
    typedef typename std::make_signed<type_t>::type s_type_t;

    type_t num[Count] = {};

    void init()
    {
        memset(num, 0, sizeof num);
    }

    void init(signed long long n)
    {
        for (size_t i = 0; i != Count && n; i++)
            num[i] = n, n >>= 8 * sizeof(type_t);
    }

    void init(unsigned long long n)
    {
        for (size_t i = 0; i != Count && n; i++)
            num[i] = n, n >>= 8 * sizeof(type_t);
    }

    template<size_t N, typename sign_t_>
    void init(const integer_layer<N, sign_t_, type_t, bigger_t>& that)
    {
        memcpy(num, that.num, sizeof(type_t) * std::min(Count, N));
        if constexpr(N < Count)
            if constexpr(sign_t_(-1) < 0)
                if (type_t s = (s_type_t(that.num[N - 1]) < 0 ? -1 : 0))
                    memset(&num[N], -1, sizeof(type_t) * (Count - N));
    }

    bool to_bool() const
    {
        for (auto i: num)
            if (i)
                return true;

        return false;
    }

    std::string hex() const
    {
        return bigint_prog_text<Count, sign_t, type_t, bigger_t>().hex(num);
    }

    std::string str(int base) const
    {
        bigint_prog_text<Count, sign_t, type_t, bigger_t> p = {};
        return p.str(num, base);
    }

    bool from_str(std::string_view input, int base)
    {
        bigint_prog_text<Count, sign_t, type_t, bigger_t> p = {};
        return p.parse(num, input, base);
    }

    void add(const layer& that)
    {
        bigint_prog_add<Count, type_t, bigger_t> p = {};
        p.add(num, that.num);
    }

    void sub(const layer& that)
    {
        bigint_prog_add<Count, type_t, bigger_t> p = {};
        p.sub(num, that.num);
    }

    void add(s_type_t n)
    {
        bigint_prog_add<Count, type_t, bigger_t> p = {};
        p.add(num, n);
    }

    void sub(s_type_t n)
    {
        bigint_prog_add<Count, type_t, bigger_t> p = {};
        if (n && n == s_type_t(-n)) {
            integer_layer l = {};
            l.init(static_cast<long long>(n));
            p.sub(num, l.num);
        } else {
            p.add(num, -n);
        }
    }

    void negate()
    {
        bigint_prog_negate<Count, type_t, bigger_t> p = {};
        p.negate(num);
    }

    void multiply(const layer& a, const layer& b)
    {
        bigint_prog_multiply<Count, type_t, bigger_t> p = {};
        p.multiply(num, a.num, b.num);
    }

    void multiply(const layer& a, type_t b)
    {
        bigint_prog_multiply_small<Count, type_t, bigger_t> p = {};
        p.multiply(num, a.num, b);
    }

    void shift_left(size_t index)
    {
        bigint_prog_shift<Count, sign_t, type_t, bigger_t> p = {};
        p.shift_left(num, index);
    }

    void shift_right(size_t index)
    {
        bigint_prog_shift<Count, sign_t, type_t, bigger_t> p = {};
        p.shift_right(num, index);
    }

    static
    void divide(layer* result, const layer& a, const layer& b, layer* remainder)
    {
        bigint_prog_divide<Count, sign_t, type_t, bigger_t> p = {};
        p.divide(result ? result->num : nullptr, a.num, b.num,
                 remainder ? remainder->num : nullptr);
    }

    static
    void divide(layer* result, const layer& a, s_type_t b, s_type_t* remainder)
    {
        bigint_prog_divide_small<Count, sign_t, type_t, bigger_t> p = {};
        p.divide(result ? result->num : nullptr, a.num, b, remainder);
    }

    void and_(const layer& b)
    {
        for (size_t i = 0; i != Count; i++)
            num[i] &= b.num[i];
    }

    void or_(const layer& b)
    {
        for (size_t i = 0; i != Count; i++)
            num[i] |= b.num[i];
    }

    void xor_(const layer& b)
    {
        for (size_t i = 0; i != Count; i++)
            num[i] ^= b.num[i];
    }

    friend bool equals(const layer& a, const layer& b)
    {
        return memcmp(a.num, b.num, sizeof a.num) == 0;
    }

    friend int compare(const layer& a, const layer& b)
    {
        bigint_prog_compare<Count, sign_t, type_t, bigger_t> p = {};
        return p.compare(a.num, b.num);
    }
};

}
