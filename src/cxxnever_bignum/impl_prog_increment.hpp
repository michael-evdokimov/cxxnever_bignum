#pragma once
#include <vector>


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_increment
{
	void do_inc(std::vector<type_t>& num)
	{
		bool negative = (num.back() == 0);
		for (size_t i = 0, len = num.size() - negative; i < len; ++i)
			if (++num[i] != 0)
				return;

		if (negative)
			++num.back(), num.push_back(0);
		else
			num.push_back(1);
	}

	void do_dec(std::vector<type_t>& num)
	{
		bool negative = (num.back() == 0);
		for (size_t i = 0; i < num.size(); ++i)
			if (num[i]-- != 0)
				break;
		if (negative)
			if (num.size() >= 2)
			       if (num.back() == 0 && num[num.size() - 2] == 0)
				       num.pop_back();
		if (!negative)
			if (num.back() == 0)
				num.pop_back();
	}

	void inc(std::vector<type_t>& num, int step)
	{
		if (num.size() == 0) {
			num.push_back(1);
			if (step == -1)
				num.push_back(0);
			return;
		}

		if (step == 1  && num.size() == 2 && num[0] == 1 && num[1] == 0)
			return (void) (num = {});
		if (step == -1 && num.size() == 1 && num[0] == 1)
			return (void) (num = {});

		int num_sign = num.back() ? 1 : -1;
		if (num_sign == step)
			return do_inc(num);
		else
			return do_dec(num);
	}
};

}
}
