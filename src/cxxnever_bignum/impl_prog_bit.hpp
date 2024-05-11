#pragma once
#include <vector>


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_bit
{
	void shift_left(std::vector<type_t>& r, size_t n)
	{
		if (r.empty())
			return;

		if (size_t to_insert = n / (sizeof(type_t) * 8)) {
			r.insert(r.begin(), to_insert, type_t(0));
			n %= sizeof(type_t) * 8;
		}

		if (n == 0)
			return;

		typedef std::make_signed_t<type_t> stype_t;
		type_t lastbit = type_t(1) << (sizeof(type_t) * 8 - 1);
		type_t mask = stype_t(lastbit) >> (n - 1);
		type_t umask = stype_t(lastbit) >> (sizeof(type_t) * 8 - n);

		size_t i = r.size();
		if (r[i - 1] == 0)
			i--;
		if (r[i - 1] & mask)
			r.insert(r.begin() + i, 0);
		while (i--) {
			type_t x = (r[i] >> (sizeof(type_t) * 8 - n));
			if (x) {
				r[i + 1] &= umask;
				r[i + 1] |= x;
			}
			r[i] <<= n;
		}
	}

	void shift_right(std::vector<type_t>& r, size_t n)
	{
		bool negative = r.size() && r.back() == 0;

		if (size_t to_remove = n / (sizeof(type_t) * 8)) {
			if (to_remove > r.size())
				to_remove = r.size();
			if (to_remove == r.size() - 1 && r.back() == 0)
				to_remove += 1;
			r.erase(r.begin(), r.begin() + to_remove);
			n %= sizeof(type_t) * 8;
		}

		if (n == 0 || r.empty())
			return;

		typedef std::make_signed_t<type_t> stype_t;
		type_t last_bit = stype_t(1) << (sizeof(type_t) * 8 - 1);
		type_t clear_mask = stype_t(last_bit) >> (n - 1);

		size_t nulls = 0;
		for (size_t i = 0; i != r.size(); ++i) {
			r[i] >>= n;
			r[i] &= ~clear_mask;
			if (i + 1 != r.size())
				r[i] |= (r[i + 1] << (sizeof(type_t) * 8 - n));
			if (r[i])
				nulls = 0;
			else
				nulls++;
		}

		r.resize(r.size() - nulls + negative);
		if (r.size() == 1 && r[0] == 0)
			r.clear();
	}
};

}
}
