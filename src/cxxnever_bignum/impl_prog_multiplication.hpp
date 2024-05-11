#pragma once
#include <vector>


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_multiplication
{
	int sign(const std::vector<type_t>& num)
	{
		if (num.empty())
			return 0;

		return num.back() == 0 ? -1 : 1;
	}

	void mult(std::vector<type_t>& result,
	          const std::vector<type_t>& a,
	          const std::vector<type_t>& b)
	{
		result.clear();
		result.reserve(a.size() + b.size());

		for (size_t i = 0; i != a.size(); ++i) {
			for (size_t j = 0; j != b.size(); ++j) {
				bigger_t r = bigger_t(a[i]) * b[j];
				for (size_t k = i + j; r; ++k) {
					if (k >= result.size())
						result.resize(k + 1);
					r += result[k];
					result[k] = r & type_t(-1);
					r >>= sizeof(type_t) * 8;
				}
			}
		}

		if (sign(a) * sign(b) == -1)
			result.push_back(0);
	}
};

}
}
