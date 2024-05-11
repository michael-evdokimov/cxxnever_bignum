#pragma once
#include <vector>
#include "impl_prog_addition.hpp"
#include "impl_prog_multiplication.hpp"


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_convert
{
	bool assign(std::vector<type_t>& res, std::string_view str, int base)
	{
		impl::prog_convert_simple<type_t, bigger_t> cvt;
		impl::prog_addition<type_t, bigger_t> add;
		impl::prog_multiplication<type_t, bigger_t> mul;

		std::vector<type_t> tmp;
		std::vector<type_t> part;
		std::vector<type_t> base_num;
		base_num.push_back(type_t(base));
		res.clear();
		auto it = str.begin();
		auto end = str.end();
		bool negative = (it != end && *it == '-' && ++it);
		while (it != end) {
			int digit = 0;
			char sym = *it;
			if ('0' <= sym && sym <= '9')
				digit = sym - '0';
			else if ('a' <= sym && sym <= 'a' + base - 10)
				digit = sym - 'a' + 10;
			else if ('A' <= sym && sym <= 'A' + base - 10)
				digit = sym - 'A' + 10;
			else
				return false;

			mul.mult(tmp, res, base_num);
			cvt.assign(part, bigger_t(digit));
			add.add(tmp, 1, part);
			res = std::move(tmp);
			++it;
		}
		if (negative)
			res.push_back(0);

		return true;
	}
};

}
}
