#pragma once
#include <vector>


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_compare
{
	int sign(const std::vector<type_t>& num)
	{
		if (num.size() == 0)
			return 0;
		if (num.size() && num.back() == 0)
			return -1;
		return 1;
	}

	int compare(const std::vector<type_t>& a, const std::vector<type_t>& b)
	{
		int a_sign = sign(a);
		int b_sign = sign(b);

		if (a_sign != b_sign)
			return (a_sign < b_sign) ? -1 : 1;

		if (a.size() != b.size())
			return ( (a.size() < b.size()) ? -1 : 1 ) * a_sign;

		for (size_t i = a.size(); i--; )
			if (a[i] != b[i])
				return ( (a[i] < b[i]) ? -1 : 1 ) * a_sign;

		return 0;
	}
};

}
}
