#pragma once
#include <vector>
#include <string>


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_string
{
	std::string hex(const std::vector<type_t>& num)
	{
		std::string r;
		if (size_t n = num.size())
			r.reserve(n * sizeof(type_t) * 2 + n + 1);

		char buf[4];
		const char* prefix = "";
		for (size_t i = num.size(); i--; ) {
			for (size_t j = sizeof(type_t); j--; ) {
				uint8_t x = num[i] >> (j * 8);
				snprintf(buf, sizeof(num), "%s%02hhx",
				                                     prefix, x);
				prefix = ":";
				r += buf;
			}
		}

		if (r.empty())
			r = "00";

		return r;
	}
};

}
}
