#pragma once
#include <vector>
#include <string>


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_stringify
{
	std::string str_base2(const std::vector<type_t>& num)
	{
		std::string r;
		r.reserve(num.size() * sizeof(type_t) * 8);

		ssize_t i = num.size();
		if (i && num[i - 1] == 0)
			r += "-", i--;

		type_t mask = type_t(1) << (sizeof(type_t) * 8 - 1);
		if (i--) {
			type_t m = mask;
			while (m && (num[i] & m) == 0)
				m >>= 1;
			while (m)
				r += '0' + bool(num[i] & m), m >>= 1;
		}

		while (i-- > 0)
			for (type_t m = mask; m; m >>= 1)
				r += '0' + bool(num[i] & m);

		if (r.empty())
			r = "0";

		return r;
	}

	std::string str_base16(const std::vector<type_t>& num)
	{
		std::string r;
		r.reserve(num.size() * sizeof(type_t) * 2);

		ssize_t i = num.size();
		if (i && num[i - 1] == 0)
			r += "-", i--;

		bool present = false;
		while (i-- > 0) {
			for (size_t j = sizeof(type_t); j--; ) {
				char buf[3] = {};
				type_t v = num[i] >> (j * 8);
				if (present == false) {
					snprintf(buf, sizeof(buf), "%hhx", v);
					if (*buf == '0')
						*buf = 0;
				} else {
					snprintf(buf, sizeof(buf), "%02hhx", v);
				}
				r += buf;
				present = present || *buf;
			}
		}

		if (r.empty())
			r = "0";

		return r;
	}

	std::string str_base_n(std::vector<type_t> num, int base)
	{
		impl::prog_division<type_t, bigger_t> div;
		std::vector<type_t> base_num;
		std::vector<type_t> tmp;
		std::vector<type_t> remainder;
		std::string r;

		base_num.push_back(base);

		bool negative = num.size() && num.back() == 0;
		if (negative)
			r += '-', num.pop_back();

		while (num.size()) {
			div.divmod(tmp, &remainder, num, base_num);
			num = std::move(tmp);
			int digit = remainder.size() ? remainder[0] : 0;
			if (0 <= digit && digit <= 9)
				r += '0' + digit;
			else
				r += 'a' + digit - 10;
		}

		ssize_t i = (r.size() && r[0] == '-') ? 1 : 0;
		ssize_t j = r.size() - 1;
		while (i < j)
			std::swap(r[i++], r[j--]);

		if (r.empty())
			r = "0";

		return r;
	}

	std::string str(const std::vector<type_t>& num, int base)
	{
		if (base == 2)
			return str_base2(num);
		if (base == 16)
			return str_base16(num);
		if (2 <= base && base <= 36)
			return str_base_n(num, base);

		throw std::runtime_error("invalid base");
	}
};

}
}
