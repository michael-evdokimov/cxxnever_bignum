#pragma once
#include "_integer-layer.hpp"


namespace cxxnever
{

template<size_t Count, typename sign_t, typename type_t, typename bigger_t>
struct integer
{
    static_assert(type_t(-1) > 0, "type must be unsigned");
    static_assert(bigger_t(-1) > 0, "bigger must be unsigned");

    typedef typename std::make_signed<type_t>::type signed_type_t;

    details::integer_layer<Count, sign_t, type_t, bigger_t> layer = {};


    integer() = default;

    integer(signed long long n)
    {
        layer.init(n);
    }

    integer(unsigned long long n)
    {
        layer.init(n);
    }

    integer(signed int n) : integer(static_cast<signed long long>(n)) {}

    integer(unsigned int n) : integer(static_cast<unsigned long long>(n)) {}

    integer(signed long n) : integer(static_cast<signed long long>(n)) {}

    integer(unsigned long n) : integer(static_cast<unsigned long long>(n)) {}

    integer(const integer&) = default;

    integer(integer&&) = default;

    integer(const integer<Count, signed, type_t, bigger_t>& that)
        requires (sign_t(-1) > 0)
    {
        layer.init(that.layer);
    }

    explicit
    integer(const integer<Count, unsigned, type_t, bigger_t>& that)
        requires (sign_t(-1) < 0)
    {
        layer.init(that.layer);
    }

    template<size_t N, typename sign_t_>
    integer(const integer<N, sign_t_, type_t, bigger_t>& that)
        requires (N < Count)
    {
        layer.init(that.layer);
    }

    template<size_t N, typename sign_t_>
    explicit
    integer(const integer<N, sign_t_, type_t, bigger_t>& that)
        requires (N > Count)
    {
        layer.init(that.layer);
    }

    integer& operator = (const integer&) = default;

    integer& operator = (integer&&) = default;

    explicit operator bool () const
    {
        return layer.to_bool();
    }

    std::string hex() const
    {
        return layer.hex();
    }

    std::string str(int base = 10) const
    {
        return layer.str(base);
    }

    integer* from_str(std::string_view input, int base = 10)
    {
        return layer.from_str(input, base) ? this : nullptr;
    }

    friend integer operator + (const integer& a, const integer& b)
    {
        integer r = a;
        r.layer.add(b.layer);
        return r;
    }

    friend integer operator - (const integer& a, const integer& b)
    {
        integer r = a;
        r.layer.sub(b.layer);
        return r;
    }

    integer& operator += (const integer& b)
    {
        layer.add(b.layer);
        return *this;
    }

    integer& operator -= (const integer& b)
    {
        layer.sub(b.layer);
        return *this;
    }

    integer& operator ++ ()
    {
        layer.add(1);
        return *this;
    }

    integer operator ++ (int)
    {
        integer r = *this;
        layer.add(1);
        return r;
    }

    integer& operator -- ()
    {
        layer.add(-1);
        return *this;
    }

    integer operator -- (int)
    {
        integer r = *this;
        layer.add(-1);
        return r;
    }

    integer& operator += (signed_type_t n)
    {
        layer.add(n);
        return *this;
    }

    integer& operator -= (signed_type_t n)
    {
        layer.sub(n);
        return *this;
    }

    integer& negate()
    {
        layer.negate();
        return *this;
    }

    integer operator - () const
    {
        integer r = *this;
        r.layer.negate();
        return r;
    }

    friend integer operator * (const integer& a, const integer& b)
    {
        integer r = {};
        r.layer.multiply(a.layer, b.layer);
        return r;
    }

    integer& operator *= (const integer& b)
    {
        integer r = {};
        r.layer.multiply(this->layer, b.layer);
        *this = r;
        return *this;
    }

    integer& multiply(const integer& a, const integer& b)
    {
        layer.init();
        layer.multiply(a.layer, b.layer);
        return *this;
    }

    integer& multiply_small(const integer& a, type_t b)
    {
        layer.init();
        layer.multiply(a.layer, b);
        return *this;
    }

    friend integer operator << (const integer& n, size_t i)
    {
        integer r = n;
        r.layer.shift_left(i);
        return r;
    }

    friend integer operator >> (const integer& n, size_t i)
    {
        integer r = n;
        r.layer.shift_right(i);
        return r;
    }

    integer& operator <<= (size_t i)
    {
        layer.shift_left(i);
        return *this;
    }

    integer& operator >>= (size_t i)
    {
        layer.shift_right(i);
        return *this;
    }

    friend integer operator / (const integer& a, const integer& b)
    {
        integer r = {};
        r.layer.divide(&r.layer, a.layer, b.layer, nullptr);
        return r;
    }

    friend integer operator % (const integer& a, const integer& b)
    {
        integer m = {};
        m.layer.divide(nullptr, a.layer, b.layer, &m.layer);
        return m;
    }

    integer& operator /= (const integer& b)
    {
        integer r = {};
        r.layer.divide(&r.layer, this->layer, b.layer, nullptr);
        *this = r;
        return *this;
    }

    integer& operator %= (const integer& b)
    {
        integer m = {};
        m.layer.divide(nullptr, this->layer, b.layer, &m.layer);
        *this = m;
        return *this;
    }

    void divide(integer* result,
                const integer& a, const integer& b, integer* remainder)
    {
        a.layer.divide(result ? &result->layer : nullptr,
                       a.layer, b.layer,
                       remainder ? &remainder->layer : nullptr);
    }

    void divide_small(integer* result,
                      const integer& a, signed_type_t b, signed_type_t* remainder)
    {
        a.layer.divide(result ? &result->layer : nullptr, a.layer, b, remainder);
    }

    friend integer operator & (const integer& a, const integer& b)
    {
        integer r = a;
        r.layer.and_(b.layer);
        return r;
    }

    friend integer operator | (const integer& a, const integer& b)
    {
        integer r = a;
        r.layer.or_(b.layer);
        return r;
    }

    friend integer operator ^ (const integer& a, const integer& b)
    {
        integer r = a;
        r.layer.xor_(b.layer);
        return r;
    }

    integer& operator &= (const integer& b)
    {
        layer.and_(b.layer);
        return *this;
    }

    integer& operator |= (const integer& b)
    {
        layer.or_(b.layer);
        return *this;
    }

    integer& operator ^= (const integer& b)
    {
        layer.xor_(b.layer);
        return *this;
    }

    friend bool operator == (const integer& a, const integer& b)
    {
        return equals(a.layer, b.layer) == true;
    }

    friend bool operator != (const integer& a, const integer& b)
    {
        return equals(a.layer, b.layer) != true;
    }

    friend bool operator < (const integer& a, const integer& b)
    {
        return compare(a.layer, b.layer) < 0;
    }

    friend bool operator <= (const integer& a, const integer& b)
    {
        return compare(a.layer, b.layer) <= 0;
    }

    friend bool operator > (const integer& a, const integer& b)
    {
        return compare(a.layer, b.layer) > 0;
    }

    friend bool operator >= (const integer& a, const integer& b)
    {
        return compare(a.layer, b.layer) >= 0;
    }
};

typedef cxxnever::integer<4, signed, uint32_t, uint64_t> int128_t;
typedef cxxnever::integer<4, unsigned, uint32_t, uint64_t> uint128_t;

typedef cxxnever::integer<8, signed, uint32_t, uint64_t> int256_t;
typedef cxxnever::integer<8, unsigned, uint32_t, uint64_t> uint256_t;

}
