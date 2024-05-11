#pragma once
#include <vector>
#include "impl_prog_convert_simple.hpp"
#include "impl_prog_string.hpp"
#include "impl_prog_compare.hpp"
#include "impl_prog_increment.hpp"
#include "impl_prog_addition.hpp"
#include "impl_prog_multiplication.hpp"
#include "impl_prog_division.hpp"
#include "impl_prog_bit.hpp"
#include "impl_prog_stringify.hpp"
#include "impl_prog_convert.hpp"


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct integer
{
	static_assert(type_t(-1) > 0, "must be unsigned");
	static_assert(bigger_t(-1) < 0, "must be signed");
	static_assert(sizeof(type_t) < sizeof(bigger_t));

	std::vector<type_t> num;

	integer() = default;

	integer(const integer&) = default;

	integer(integer&&) = default;

	integer(bigger_t n)
	{
		impl::prog_convert_simple<type_t, bigger_t> p;
		p.assign(num, n);
	}

	integer(const std::string_view& str)
	{
		impl::prog_convert<type_t, bigger_t> p;
		if (!p.assign(num, str, 10))
			throw std::runtime_error("invalid number");
	}

	integer& operator = (const integer&) = default;

	integer& operator = (integer&&) = default;

	integer& operator = (bigger_t n)
	{
		impl::prog_convert_simple<type_t, bigger_t> p;
		p.assign(num, n);
		return *this;
	}

	integer& operator = (const std::string_view& str)
	{
		impl::prog_convert<type_t, bigger_t> p;
		if (p.assign(num, str, 10))
			return *this;
		throw std::runtime_error("invalid number");
	}

	std::string hex() const
	{
		impl::prog_string<type_t, bigger_t> p;
		return p.hex(num);
	}

	friend bool operator == (const integer& a, const integer& b)
	{
		return a.num == b.num;
	}

	friend bool operator != (const integer& a, const integer& b)
	{
		return a.num != b.num;
	}

	friend bool operator < (const integer& a, const integer& b)
	{
		impl::prog_compare<type_t, bigger_t> p;
		return p.compare(a.num, b.num) < 0;
	}

	friend bool operator <= (const integer& a, const integer& b)
	{
		impl::prog_compare<type_t, bigger_t> p;
		return p.compare(a.num, b.num) <= 0;
	}

	friend bool operator > (const integer& a, const integer& b)
	{
		impl::prog_compare<type_t, bigger_t> p;
		return p.compare(a.num, b.num) > 0;
	}

	friend bool operator >= (const integer& a, const integer& b)
	{
		impl::prog_compare<type_t, bigger_t> p;
		return p.compare(a.num, b.num) >= 0;
	}

	integer& operator ++ ()
	{
		impl::prog_increment<type_t, bigger_t> p;
		p.inc(num, 1);
		return *this;
	}

	integer operator ++ (int)
	{
		integer r = *this;
		impl::prog_increment<type_t, bigger_t> p;
		p.inc(num, 1);
		return r;
	}

	integer& operator -- ()
	{
		impl::prog_increment<type_t, bigger_t> p;
		p.inc(num, -1);
		return *this;
	}

	integer operator -- (int)
	{
		integer r = *this;
		impl::prog_increment<type_t, bigger_t> p;
		p.inc(num, -1);
		return r;
	}

	integer& operator += (const integer& n)
	{
		impl::prog_addition<type_t, bigger_t> p;
		p.add(num, 1, n.num);
		return *this;
	}

	integer& operator -= (const integer& n)
	{
		impl::prog_addition<type_t, bigger_t> p;
		p.add(num, -1, n.num);
		return *this;
	}

	friend integer operator + (const integer& a, const integer& b)
	{
		integer r = a;
		impl::prog_addition<type_t, bigger_t> p;
		p.add(r.num, 1, b.num);
		return r;
	}

	friend integer operator - (const integer& a, const integer& b)
	{
		integer r = a;
		impl::prog_addition<type_t, bigger_t> p;
		p.add(r.num, -1, b.num);
		return r;
	}

	integer& operator *= (const integer& n)
	{
		std::vector<type_t> r;
		impl::prog_multiplication<type_t, bigger_t> p;
		p.mult(r, num, n.num);
		num = std::move(r);
		return *this;
	}

	friend integer operator * (const integer& a, const integer& b)
	{
		integer r;
		impl::prog_multiplication<type_t, bigger_t> p;
		p.mult(r.num, a.num, b.num);
		return r;
	}

	void divmod(integer& result, integer& remainder, const integer& b)
	{
		impl::prog_division<type_t, bigger_t> p;
		p.divmod(result.num, &remainder.num, num, b.num);
	}

	integer& operator /= (const integer& n)
	{
		impl::prog_division<type_t, bigger_t> p;
		std::vector<type_t> r;
		p.divmod(r, nullptr, num, n.num);
		num = std::move(r);
		return *this;
	}

	integer& operator %= (const integer& n)
	{
		impl::prog_division<type_t, bigger_t> p;
		std::vector<type_t> r;
		std::vector<type_t> m;
		p.divmod(r, &m, num, n.num);
		num = std::move(m);
		return *this;
	}

	friend integer operator / (const integer& a, const integer& b)
	{
		impl::prog_division<type_t, bigger_t> p;
		integer r;
		p.divmod(r.num, nullptr, a.num, b.num);
		return r;
	}

	friend integer operator % (const integer& a, const integer& b)
	{
		impl::prog_division<type_t, bigger_t> p;
		integer r;
		integer m;
		p.divmod(r.num, &m.num, a.num, b.num);
		return m;
	}

	integer& operator <<= (size_t n)
	{
		impl::prog_bit<type_t, bigger_t> p;
		p.shift_left(num, n);
		return *this;
	}

	integer& operator >>= (size_t n)
	{
		impl::prog_bit<type_t, bigger_t> p;
		p.shift_right(num, n);
		return *this;
	}

	integer operator << (size_t n)
	{
		integer r = *this;
		impl::prog_bit<type_t, bigger_t> p;
		p.shift_left(r.num, n);
		return r;
	}

	integer operator >> (size_t n)
	{
		integer r = *this;
		impl::prog_bit<type_t, bigger_t> p;
		p.shift_right(r.num, n);
		return r;
	}

	std::string str(int base = 10) const
	{
		impl::prog_stringify<type_t, bigger_t> p;
		return p.str(num, base);
	}

	bool assign(const std::string_view& str, int base = 10)
	{
		impl::prog_convert<type_t, bigger_t> p;
		return p.assign(num, str, base);
	}
};

}
}
