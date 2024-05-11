#pragma once
#include <vector>
#include "impl_prog_compare.hpp"


namespace cxxnever
{
namespace impl
{

template<typename type_t, typename bigger_t>
struct prog_addition
{
	void negate(std::vector<type_t>& num)
	{
		if (num.empty())
			return;

		if (num.back() == 0)
			num.pop_back();
		else
			num.push_back(0);
	}

	void recover_sign(std::vector<type_t>& num, int result_sign)
	{
		size_t i = num.size();
		while (i && num[i - 1] == 0)
			--i;

		if (result_sign == -1)
			++i;

		num.resize(i);
	}

	void do_add(std::vector<type_t>& r, const std::vector<type_t>& n, int s)
	{
		int result_sign = s;
		bigger_t carry = 0;
		for (size_t i = 0; i < n.size() || carry; ++i) {
			carry += (i < r.size()) ? r[i] : 0;
			carry += (i < n.size()) ? n[i] : 0;
			if (i == r.size())
				r.push_back(0);
			r[i] = carry;
			carry >>= sizeof(type_t) * 8;
		}
		recover_sign(r, result_sign);
	}

	void do_sub(std::vector<type_t>& r, const std::vector<type_t>& n, int s)
	{
		int result_sign = s;
		bigger_t carry = 0;
		for (size_t i = 0; i < n.size() || carry; ++i) {
			carry += (i < r.size()) ? r[i] : 0;
			carry -= (i < n.size()) ? n[i] : 0;
			if (i == r.size())
				r.push_back(0);
			r[i] = carry;
			carry >>= sizeof(type_t) * 8;
		}
		recover_sign(r, result_sign);
	}

	int cmp_abs(const std::vector<type_t>& a, const std::vector<type_t>& b)
	{
		size_t a_len = a.size();
		size_t b_len = b.size();

		if (a_len && a[a_len - 1] == 0) a_len--;
		if (b_len && b[b_len - 1] == 0) b_len--;

		if (a_len != b_len)
			return a_len < b_len ? -1 : 1;

		for (size_t i = a_len; i--; )
			if (a[i] != b[i])
				return (a[i] < b[i]) ? -1 : 1;
		return 0;
	}

	void sub_cmp(std::vector<type_t>& r, const std::vector<type_t>& n,int s)
	{
		int cmp_r = cmp_abs(r, n);

		if (cmp_r == 0)
			return (void) (r = {});

		if (cmp_r == 1)
			return do_sub(r, n, s);

		if (cmp_r == -1) {
			std::vector<type_t> t = n;
			do_sub(t, r, s * (-1));
			r = std::move(t);
			return;
		}
	}

	void add(std::vector<type_t>& r, int op, const std::vector<type_t>& n)
	{
		if (n.empty())
			return;

		if (r.empty()) {
			r = n;
			if (op == -1)
				negate(r);
			return;
		}

		impl::prog_compare<type_t, bigger_t> cmp;

		int r_sign = cmp.sign(r);
		int n_sign = cmp.sign(n);

		if (false)
			;
		else if (r_sign == 1 && op == 1 && n_sign == 1)
			return do_add(r, n, 1);
		else if (r_sign == 1 && op == 1 && n_sign == -1)
			return sub_cmp(r, n, 1);
		else if (r_sign == 1 && op == -1 && n_sign == 1)
			return sub_cmp(r, n, 1);
		else if (r_sign == 1 && op == -1 && n_sign == -1)
			return do_add(r, n, 1);

		else if (r_sign == -1 && op == 1 && n_sign == 1)
			return sub_cmp(r, n, -1);
		else if (r_sign == -1 && op == 1 && n_sign == -1)
			return do_add(r, n, -1);
		else if (r_sign == -1 && op == -1 && n_sign == 1)
			return do_add(r, n, -1);
		else if (r_sign == -1 && op == -1 && n_sign == -1)
			return sub_cmp(r, n, -1);
	}
};

}
}
