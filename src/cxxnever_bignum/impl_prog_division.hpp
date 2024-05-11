#pragma once
#include <vector>
#include "impl_prog_compare.hpp"
#include "impl_prog_addition.hpp"
#include "impl_prog_multiplication.hpp"


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_division
{
	void set_bit_number(std::vector<type_t>& num, size_t i, bool value)
	{
		size_t type_t_bits = sizeof(type_t) * 8;
		size_t i8 = i / type_t_bits;
		if (num.size() < i8 + 1)
			num.resize(i8 + 1);

		type_t flag = type_t(1) << (i % type_t_bits);
		if (value)
			num[i8] |= flag;
		else
			num[i8] &= ~flag;
	}

	void divmod_(std::vector<type_t>& r, std::vector<type_t>* remainder,
	             const std::vector<type_t>& a, const std::vector<type_t>& b)
	{
		impl::prog_multiplication<type_t, bigger_t> mul;
		impl::prog_addition<type_t, bigger_t> add;
		impl::prog_compare<type_t, bigger_t> cmp;

		r.clear();
		if (cmp.compare(a, b) < 0) {
			if (remainder)
				*remainder = a;
			return;
		}

		std::vector<type_t> t;
		ssize_t i = 0;

		while (true) {
			set_bit_number(r, i, true);
			mul.mult(t, r, b);
			int cmp_r = cmp.compare(t, a);
			set_bit_number(r, i, false);
			if (cmp_r > 0)
				break;
			i++;
		}

		i--;

		r.clear();
		while (i >= 0) {
			set_bit_number(r, i, true);
			mul.mult(t, r, b);
			int cmp_r = cmp.compare(t, a);
			if (cmp_r == 0)
				break;
			if (cmp_r > 0)
				set_bit_number(r, i, false);
			i--;
		}

		if (remainder) {
			mul.mult(t, r, b);
			std::vector<type_t> m = a;
			add.add(m, -1, t);
			*remainder = m;
		}
	}

	void divmod(std::vector<type_t>& r, std::vector<type_t>* remainder,
	            const std::vector<type_t>& a, const std::vector<type_t>& b)
	{
		impl::prog_compare<type_t, bigger_t> p_cmp;
		impl::prog_addition<type_t, bigger_t> p;
		int sign_a = p_cmp.sign(a);
		int sign_b = p_cmp.sign(b);

		if (sign_b == 0)
			throw std::logic_error("division by zero");
		if (sign_a == 0) {
			r = {};
			if (remainder)
				*remainder = {};
			return;
		}

		if (sign_a > 0 && sign_b > 0)
			return divmod_(r, remainder, a, b);
		if (sign_a < 0 && sign_b < 0) {
			std::vector<type_t> a_ = a;
			std::vector<type_t> b_ = b;
			p.negate(a_);
			p.negate(b_);
			divmod_(r, remainder, a_, b_);
			if (remainder)
				p.negate(*remainder);
			return;
		}
		if (sign_a < 0 && sign_b > 0) {
			std::vector<type_t> t = a;
			p.negate(t);
			divmod_(r, remainder, t, b);
			p.negate(r);
			if (remainder)
				p.negate(*remainder);
		}
		if (sign_a > 0 && sign_b < 0) {
			std::vector<type_t> t = b;
			p.negate(t);
			divmod_(r, remainder, a, t);
			p.negate(r);
		}
	}
};

}
}
