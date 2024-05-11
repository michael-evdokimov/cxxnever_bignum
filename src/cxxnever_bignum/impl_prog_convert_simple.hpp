#pragma once
#include <vector>


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_convert_simple
{
	void assign(std::vector<type_t>& num, bigger_t n)
	{
		num.clear();

		if (n == 0)
			return;

		bool neg = (n < 0);
		if (neg) {
			bigger_t st = bigger_t(1) << (sizeof(bigger_t) * 8 - 1);
			if (n == st) {
				num.resize(sizeof(bigger_t) / sizeof(type_t));
				auto v = type_t(1) << (sizeof(type_t) * 8 - 1);
				num.back() = v;
				num.push_back(0);
				return;
			}
			n = -n;
		}

		while (n)
			num.push_back(n), n >>= (sizeof(type_t) * 8);

		if (neg)
			num.push_back(0);
	}
};

}
}
