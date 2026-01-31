#pragma once
#include <vector>
#include <string>
#include "_prog-convert.hpp"
#include "_prog-show.hpp"
#include "_prog-read.hpp"
#include "_prog-addition.hpp"
#include "_prog-subtract.hpp"
#include "_prog-negate.hpp"
#include "_prog-multiply.hpp"
#include "_prog-divide.hpp"
#include "_prog-compare.hpp"
#include "_prog-shift.hpp"
#include "_prog-print.hpp"
#include "_prog-simple-add.hpp"
#include "_prog-parser.hpp"
#include "_prog-parser16.hpp"
#include "_prog-karatsuba.hpp"


namespace cxxnever::details
{

template<typename type_t, typename bigger_t>
struct number
{
    static_assert(type_t(-1) < 0, "type must be signed");
    static_assert(bigger_t(-1) > 0, "bigger must be unsigned");
    static_assert(sizeof(type_t) < sizeof(bigger_t));

    std::vector<type_t> num = {};

    number() = default;

    number(const number&) = default;

    number(number&&) = default;

    number(long long n)
    {
        prog_convert<type_t, bigger_t> p = {};
        p.convert(num, n);
    }

    number(std::string_view str)
    {
        prog_parser<type_t, bigger_t> p = {};
        if (!p.read(num, str, 10))
            throw std::runtime_error("invalid argument");
    }

    number& operator = (const number&) = default;

    number& operator = (number&&) = default;

    number& operator = (long long n)
    {
        prog_convert<type_t, bigger_t> p = {};
        p.convert(num, n);
        return *this;
    }

    number& operator = (std::string_view str)
    {
        prog_parser<type_t, bigger_t> p = {};
        if (!p.read(num, str, 10))
            throw std::runtime_error("invalid argument");
        return *this;
    }

    bool assign(std::string_view str, int base = 10)
    {
        if (base == 16) {
            prog_parser16<type_t, bigger_t> p = {};
            return p.read(num, str);
        } else {
            prog_parser<type_t, bigger_t> p = {};
            return p.read(num, str, base);
        }
    }

    std::string hex() const
    {
        prog_show<type_t, bigger_t> p = {};
        return p.show(num);
    }

    number& from_hex(std::string_view str)
    {
        num = {};
        prog_read<type_t, bigger_t> p = {};
        p.read(num, str);
        return *this;
    }

    number operator - () const
    {
        number r = *this;
        prog_negate<type_t, bigger_t> p = {};
        p.negate(r.num);
        return r;
    }

    void negate()
    {
        prog_negate<type_t, bigger_t> p = {};
        p.negate(num);
    }

    friend number operator + (const number& a, const number& b)
    {
        number r = a;
        prog_addition<type_t, bigger_t> p = {};
        p.add(r.num, b.num);
        return r;
    }

    friend number operator - (const number& a, const number& b)
    {
        number r = a;
        prog_subtract<type_t, bigger_t> p = {};
        p.subtract(r.num, b.num);
        return r;
    }

    friend number operator * (const number& a, const number& b)
    {
        number r = {};
        prog_multiply<type_t, bigger_t> p = {};
        p.multiply(r.num, a.num, b.num);
        return r;
    }

    friend number operator / (const number& a, const number& b)
    {
        number r = {};
        prog_divide<type_t, bigger_t> p = {};
        p.divide(&r.num, a.num, b.num);
        return r;
    }

    friend number operator % (const number& a, const number& b)
    {
        number m = {};
        prog_divide<type_t, bigger_t> p = {};
        p.divide(nullptr, a.num, b.num, &m.num);
        return m;
    }

    friend bool operator == (const number& a, const number& b)
    {
        return a.num == b.num;
    }

    friend bool operator != (const number& a, const number& b)
    {
        return a.num != b.num;
    }

    friend bool operator < (const number& a, const number& b)
    {
        prog_compare<type_t, bigger_t> p = {};
        return p.compare(a.num, b.num) < 0;
    }

    friend bool operator <= (const number& a, const number& b)
    {
        prog_compare<type_t, bigger_t> p = {};
        return p.compare(a.num, b.num) <= 0;
    }

    friend bool operator > (const number& a, const number& b)
    {
        prog_compare<type_t, bigger_t> p = {};
        return p.compare(a.num, b.num) > 0;
    }

    friend bool operator >= (const number& a, const number& b)
    {
        prog_compare<type_t, bigger_t> p = {};
        return p.compare(a.num, b.num) >= 0;
    }

    friend number operator << (const number& num, size_t n)
    {
        number r = num;
        prog_shift<type_t, bigger_t> p = {};
        p.shift_left(r.num, n);
        return r;
    }

    friend number operator >> (const number& num, size_t n)
    {
        number r = num;
        prog_shift<type_t, bigger_t> p = {};
        p.shift_right(r.num, n);
        return r;
    }

    number& operator <<= (size_t n)
    {
        prog_shift<type_t, bigger_t> p = {};
        p.shift_left(num, n);
        return *this;
    }

    number& operator >>= (size_t n)
    {
        prog_shift<type_t, bigger_t> p = {};
        p.shift_right(num, n);
        return *this;
    }

    std::string str(int base = 10) const
    {
        prog_print<type_t, bigger_t> p = {};
        return p.str(num, base);
    }

    number* from_str(std::string_view str, int base = 10)
    {
        if (base == 16) {
            prog_parser16<type_t, bigger_t> p = {};
            return p.read(num, str) ? this : nullptr;
        } else {
            prog_parser<type_t, bigger_t> p = {};
            return p.read(num, str, base) ? this : nullptr;
        }
    }

    number& operator += (type_t b)
    {
        prog_simple_add<type_t, bigger_t> p = {};
        p.add(num, b);
        return *this;
    }

    number& operator -= (type_t b)
    {
        if (b && b == type_t(-b)) {
            std::vector<type_t> b_copy = {b};
            prog_subtract<type_t, bigger_t> p = {};
            p.subtract(num, b_copy);
        } else {
            prog_simple_add<type_t, bigger_t> p = {};
            p.add(num, b * -1);
        }
        return *this;
    }

    number& operator ++ ()
    {
        prog_simple_add<type_t, bigger_t> p = {};
        p.add(num, 1);
        return *this;
    }

    number operator ++ (int)
    {
        number r = *this;
        prog_simple_add<type_t, bigger_t> p = {};
        p.add(num, 1);
        return r;
    }

    number& operator -- ()
    {
        prog_simple_add<type_t, bigger_t> p = {};
        p.add(num, -1);
        return *this;
    }

    number operator -- (int)
    {
        number r = *this;
        prog_simple_add<type_t, bigger_t> p = {};
        p.add(num, -1);
        return r;
    }

    number& operator += (const number& b)
    {
        prog_addition<type_t, bigger_t> p = {};
        p.add(num, b.num);
        return *this;
    }

    number& operator -= (const number& b)
    {
        prog_subtract<type_t, bigger_t> p = {};
        p.subtract(num, b.num);
        return *this;
    }

    number& operator *= (const number& b)
    {
        std::vector<type_t> result = {};
        prog_multiply<type_t, bigger_t> p = {};
        p.multiply(result, num, b.num);
        num = std::move(result);
        return *this;
    }

    number& operator /= (const number& b)
    {
        std::vector<type_t> result = {};
        prog_divide<type_t, bigger_t> p = {};
        p.divide(&result, num, b.num);
        num = std::move(result);
        return *this;
    }

    number& operator %= (const number& b)
    {
        std::vector<type_t> remainder = {};
        prog_divide<type_t, bigger_t> p = {};
        p.divide(nullptr, num, b.num, &remainder);
        num = std::move(remainder);
        return *this;
    }

    number& divide(const number& a, const number& b,
                   number* remainder = nullptr)
    {
        prog_divide<type_t, bigger_t> p = {};
        p.divide(&num, a.num, b.num, remainder ? &remainder->num : nullptr);
        return *this;
    }

    number& multiply(const number& a, const number& b)
    {
        prog_multiply<type_t, bigger_t> p = {};
        p.multiply(num, a.num, b.num);
        return *this;
    }

    number& multiply_karatsuba(const number& a, const number& b)
    {
        prog_karatsuba<type_t, bigger_t> p = {};
        p.multipy(num, a.num, b.num);
        return *this;
    }
};

}

namespace cxxnever
{

typedef details::number<int32_t, uint64_t> number;

}
