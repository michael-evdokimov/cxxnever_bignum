#include <gtest/gtest.h>
#include <cxxnever_bignum/number.hpp>


namespace {
typedef cxxnever::impl::integer<uint8_t, int64_t> int_t;
}

TEST(test, test_constr)
{
	int_t a;
	EXPECT_EQ("00", a.hex());

	int_t b = 0;
	EXPECT_EQ("00", b.hex());

	int_t c = 7;
	EXPECT_EQ("07", c.hex());

	EXPECT_EQ("0a", int_t(10).hex());
	EXPECT_EQ("0f", int_t(15).hex());
	EXPECT_EQ("10", int_t(16).hex());
	EXPECT_EQ("7f", int_t(127).hex());
	EXPECT_EQ("80", int_t(128).hex());
	EXPECT_EQ("81", int_t(129).hex());
	EXPECT_EQ("fe", int_t(254).hex());
	EXPECT_EQ("ff", int_t(255).hex());
	EXPECT_EQ("01:00", int_t(256).hex());
	EXPECT_EQ("01:01", int_t(257).hex());
	EXPECT_EQ("01:02", int_t(258).hex());
	EXPECT_EQ("03:e7", int_t(999).hex());
	EXPECT_EQ("7f:ff", int_t(0x7f'ff).hex());
	EXPECT_EQ("80:00", int_t(0x80'00).hex());
	EXPECT_EQ("ff:ff", int_t(0xff'ff).hex());
	EXPECT_EQ("01:00:00", int_t(0x1'00'00).hex());

	EXPECT_EQ("00:01", int_t(-1).hex());
	EXPECT_EQ("00:02", int_t(-2).hex());
	EXPECT_EQ("00:07", int_t(-7).hex());
	EXPECT_EQ("00:0a", int_t(-10).hex());
	EXPECT_EQ("00:7f", int_t(-127).hex());
	EXPECT_EQ("00:80", int_t(-128).hex());
	EXPECT_EQ("00:ff", int_t(-255).hex());
	EXPECT_EQ("00:01:00", int_t(-256).hex());
	EXPECT_EQ("00:01:01", int_t(-257).hex());
	EXPECT_EQ("00:01:02", int_t(-258).hex());
	EXPECT_EQ("00:ff:ff", int_t(-0xff'ff).hex());
	EXPECT_EQ("00:01:00:00", int_t(-0x1'00'00).hex());

	cxxnever::impl::integer<uint16_t, int64_t> d = 123456789;
	EXPECT_EQ("07:5b:cd:15", d.hex());
}

static std::vector<int> make_the_list()
{
	std::vector<int> list = {
		0, 1, 2, 3, 4, 5, 6, 7,
		15, 16, 17,
		63, 64, 65,
		127, 128, 129,
		254, 255, 256,
		999, 1000,
		0x7fff, 0x8000, 0x8001,
		0xffff, 0x10000, 0x10001,
	};

	for (int i: std::vector<int>(list))
		list.push_back(-i);

	return list;
}

TEST(test, test_compare)
{
	std::vector<int> list = make_the_list();

	for (int a: list) {
		for (int b: list) {
			char log[0x100] = {};
			snprintf(log, sizeof(log), "a = %d, b = %d", a, b);

			EXPECT_EQ(a == b, int_t(a) == int_t(b)) << log;
			EXPECT_EQ(a != b, int_t(a) != int_t(b)) << log;
			EXPECT_EQ(a < b,  int_t(a) < int_t(b))  << log;
			EXPECT_EQ(a <= b, int_t(a) <= int_t(b)) << log;
			EXPECT_EQ(a > b,  int_t(a) > int_t(b))  << log;
			EXPECT_EQ(a >= b, int_t(a) >= int_t(b)) << log;
		}
	}
}

TEST(test, test_increment)
{
	std::vector<int> list = make_the_list();

	for (int i: list) {
		char log[0x100] = {};
		snprintf(log, sizeof(log), "i = %d", i);

		EXPECT_EQ(int_t(i+1).hex(), (++int_t(i)).hex()) << log;
		EXPECT_EQ(int_t(i+1), (++int_t(i))) << log;

		EXPECT_EQ(int_t(i-1).hex(), (--int_t(i)).hex()) << log;
		EXPECT_EQ(int_t(i-1), (--int_t(i))) << log;

		int_t n = i;
		int_t m = n++;
		EXPECT_EQ(int_t(i+1).hex(), n.hex()) << log;
		EXPECT_EQ(int_t(i).hex(), m.hex()) << log;
		EXPECT_EQ(int_t(i+1), n) << log;
		EXPECT_EQ(int_t(i), m) << log;

		int_t x = i;
		int_t y = x--;
		EXPECT_EQ(int_t(i-1).hex(), x.hex()) << log;
		EXPECT_EQ(int_t(i).hex(), y.hex()) << log;
		EXPECT_EQ(int_t(i-1), x) << log;
		EXPECT_EQ(int_t(i), y) << log;
	}
}

TEST(test, test_addition)
{
	std::vector<int> list = make_the_list();

	for (int a: list) {
		for (int b: list) {
			char log[0x100] = {};
			snprintf(log, sizeof(log), "a = %d, b = %d", a, b);

			EXPECT_EQ(int_t(a + b).hex(),
			                   (int_t(a) += int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a - b).hex(),
			                   (int_t(a) -= int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a + b).hex(),
			                    (int_t(a) + int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a - b).hex(),
			                    (int_t(a) - int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a + b), (int_t(a) += int_t(b))) << log;
			EXPECT_EQ(int_t(a - b), (int_t(a) -= int_t(b))) << log;
			EXPECT_EQ(int_t(a + b), (int_t(a) + int_t(b))) << log;
			EXPECT_EQ(int_t(a - b), (int_t(a) - int_t(b))) << log;
		}
	}
}

TEST(test, test_multiplication)
{
	std::vector<int> list = make_the_list();

	for (long long a: list) {
		for (long long b: list) {
			char log[0x100] = {};
			snprintf(log, sizeof(log), "a = %lld, b = %lld", a, b);

			EXPECT_EQ(int_t(a * b).hex(),
			                   (int_t(a) *= int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a * b).hex(),
			                    (int_t(a) * int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a * b), (int_t(a) *= int_t(b))) << log;
			EXPECT_EQ(int_t(a * b), (int_t(a) * int_t(b))) << log;
		}
	}
}

TEST(test, test_division)
{
	std::vector<int> list = make_the_list();

	for (int a: list) {
		for (int b: list) {
			char log[0x100] = {};
			snprintf(log, sizeof(log), "a = %d, b = %d", a, b);

			if (b == 0)
				continue;

			EXPECT_EQ(int_t(a / b).hex(),
			                   (int_t(a) /= int_t(b)).hex()) << log;
			EXPECT_EQ(int_t(a / b).hex(),
			                    (int_t(a) / int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a % b).hex(),
			                   (int_t(a) %= int_t(b)).hex()) << log;
			EXPECT_EQ(int_t(a % b).hex(),
			                    (int_t(a) % int_t(b)).hex()) << log;


			EXPECT_EQ(int_t(a / b), (int_t(a) /= int_t(b))) << log;
			EXPECT_EQ(int_t(a / b), (int_t(a) / int_t(b))) << log;

			EXPECT_EQ(int_t(a % b), (int_t(a) %= int_t(b))) << log;
			EXPECT_EQ(int_t(a % b), (int_t(a) % int_t(b))) << log;

			int_t r;
			int_t m;
			int_t(a).divmod(r, m, b);
			EXPECT_EQ(int_t(a / b).hex(), r.hex());
			EXPECT_EQ(int_t(a % b).hex(), m.hex());
			EXPECT_EQ(int_t(a / b), r);
			EXPECT_EQ(int_t(a % b), m);
		}
	}

	EXPECT_THROW(int_t(1) / int_t(0), std::logic_error);
}

TEST(test, test_division_short)
{
	typedef cxxnever::impl::integer<uint16_t, int64_t> int_t;

	std::vector<int> list = make_the_list();

	for (int a: list) {
		for (int b: list) {
			char log[0x100] = {};
			snprintf(log, sizeof(log), "a = %d, b = %d", a, b);

			if (b == 0)
				continue;

			EXPECT_EQ(int_t(a / b).hex(),
			                    (int_t(a) / int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a % b).hex(),
			                    (int_t(a) % int_t(b)).hex()) << log;

			EXPECT_EQ(int_t(a / b), (int_t(a) / int_t(b))) << log;
			EXPECT_EQ(int_t(a % b), (int_t(a) % int_t(b))) << log;
		}
	}
}

TEST(test, test_bit_shift)
{
	std::vector<int> list = make_the_list();
	std::vector<int> shifts = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 15, 16, 17};

	for (long long i: list) {
		for (int op: shifts) {
			char log[0x100] = {};
			snprintf(log, sizeof(log), "i = %lld, op = %d", i, op);

			if (i < 0)
				continue;

			EXPECT_EQ(int_t(i << op).hex(),
			                         (int_t(i) << op).hex()) << log;

			EXPECT_EQ(int_t(i << op).hex(),
			                        (int_t(i) <<= op).hex()) << log;

			EXPECT_EQ(int_t(i >> op).hex(),
			                         (int_t(i) >> op).hex()) << log;

			EXPECT_EQ(int_t(i >> op).hex(),
			                        (int_t(i) >>= op).hex()) << log;

			EXPECT_EQ(int_t(i << op), int_t(i) << op) << log;
			EXPECT_EQ(int_t(i << op), int_t(i) <<= op) << log;
			EXPECT_EQ(int_t(i >> op), int_t(i) >> op) << log;
			EXPECT_EQ(int_t(i >> op), int_t(i) >>= op) << log;
		}
	}

	for (long long i: list) {
		for (int op: shifts) {
			char log[0x100] = {};
			snprintf(log, sizeof(log), "i = %lld, op = %d", i, op);

			if (i > 0)
				continue;

			EXPECT_EQ(int_t(  ((-i) << op)  * (-1) ).hex(),
			                         (int_t(i) << op).hex()) << log;

			EXPECT_EQ(int_t(  ((-i) >> op)  * (-1) ).hex(),
			                         (int_t(i) >> op).hex()) << log;

			EXPECT_EQ(int_t(  ((-i) << op)  * (-1) ),
			                                 int_t(i) << op) << log;
			EXPECT_EQ(int_t(  ((-i) >> op)  * (-1) ),
			                                 int_t(i) >> op) << log;
		}
	}
}

TEST(test, test_str_base2)
{
	EXPECT_EQ("0", int_t().str(2));
	EXPECT_EQ("1", int_t(1).str(2));
	EXPECT_EQ("10", int_t(2).str(2));
	EXPECT_EQ("11", int_t(3).str(2));
	EXPECT_EQ("100", int_t(4).str(2));
	EXPECT_EQ("101", int_t(5).str(2));
	EXPECT_EQ("1111111", int_t(127).str(2));
	EXPECT_EQ("10000000", int_t(128).str(2));
	EXPECT_EQ("10000001", int_t(129).str(2));
	EXPECT_EQ("11111111", int_t(255).str(2));
	EXPECT_EQ("100000000", int_t(256).str(2));
	EXPECT_EQ("100000001", int_t(257).str(2));
	EXPECT_EQ("1111111111111111", int_t(0xffff).str(2));
	EXPECT_EQ("-1", int_t(-1).str(2));
	EXPECT_EQ("-10", int_t(-2).str(2));
	EXPECT_EQ("-11", int_t(-3).str(2));
	EXPECT_EQ("-100", int_t(-4).str(2));
	EXPECT_EQ("-1111111", int_t(-127).str(2));
	EXPECT_EQ("-10000000", int_t(-128).str(2));
	EXPECT_EQ("-10000001", int_t(-129).str(2));
	EXPECT_EQ("-11111111", int_t(-255).str(2));
	EXPECT_EQ("-100000000", int_t(-256).str(2));
	EXPECT_EQ("-100000001", int_t(-257).str(2));
}

TEST(test, test_str_base16)
{
	EXPECT_EQ("0", int_t(0).str(16));
	EXPECT_EQ("1", int_t(1).str(16));
	EXPECT_EQ("2", int_t(2).str(16));
	EXPECT_EQ("3", int_t(3).str(16));
	EXPECT_EQ("f", int_t(15).str(16));
	EXPECT_EQ("10", int_t(16).str(16));
	EXPECT_EQ("7f", int_t(127).str(16));
	EXPECT_EQ("80", int_t(128).str(16));
	EXPECT_EQ("81", int_t(129).str(16));
	EXPECT_EQ("ff", int_t(255).str(16));
	EXPECT_EQ("100", int_t(256).str(16));
	EXPECT_EQ("101", int_t(257).str(16));
	EXPECT_EQ("-1", int_t(-1).str(16));
	EXPECT_EQ("-2", int_t(-2).str(16));
	EXPECT_EQ("-3", int_t(-3).str(16));
	EXPECT_EQ("-ff", int_t(-255).str(16));
	EXPECT_EQ("-100", int_t(-256).str(16));
	EXPECT_EQ("-101", int_t(-257).str(16));
}

TEST(test, test_str_base_10)
{
	std::vector<int> list = make_the_list();

	for (int i: list) {
		char log[0x100] = {};
		snprintf(log, sizeof(log), "i = %d", i);

		char name[0x100] = {};
		snprintf(name, sizeof(name), "%d", i);

		EXPECT_EQ(name, int_t(i).str()) << log;
	}
}

TEST(test, test_str_base_36)
{
	EXPECT_EQ("a", int_t(10).str(36));
	EXPECT_EQ("z", int_t(35).str(36));
	EXPECT_EQ("10", int_t(36).str(36));
	EXPECT_EQ("11", int_t(37).str(36));
}

TEST(test, test_convert_from_string)
{
	EXPECT_EQ(int_t(0).hex(), (int_t() = "0").hex());
	EXPECT_EQ(int_t(1).hex(), (int_t() = "1").hex());
	EXPECT_EQ(int_t(2).hex(), (int_t() = "2").hex());
	EXPECT_EQ(int_t(3).hex(), (int_t() = "3").hex());
	EXPECT_EQ(int_t(34).hex(), (int_t() = "34").hex());
	EXPECT_EQ(int_t(1234567).hex(), (int_t() = "1234567").hex());

	EXPECT_THROW(int_t() = "123ff", std::runtime_error);

	EXPECT_EQ(int_t(-1).hex(),      (int_t() = "-1").hex());
	EXPECT_EQ(int_t(-123456).hex(), (int_t() = "-123456").hex());

	EXPECT_EQ(int_t(7).hex(), (int_t() = std::string_view("7")).hex());
	EXPECT_EQ(int_t(7).hex(), (int_t() = std::string("7")).hex());

	int_t x = 0;
	EXPECT_TRUE(x.assign("111", 2));
	EXPECT_EQ(int_t(7).hex(), x.hex());

	EXPECT_TRUE(x.assign("100", 16));
	EXPECT_EQ(int_t(256).hex(), x.hex());

	EXPECT_TRUE(x.assign("fF", 16));
	EXPECT_EQ(int_t(255).hex(), x.hex());

	EXPECT_TRUE(x.assign("z", 36));
	EXPECT_EQ("23", x.hex());
	EXPECT_TRUE(x.assign("10", 36));
	EXPECT_EQ("24", x.hex());
	EXPECT_TRUE(x.assign("1z", 36));
	EXPECT_EQ("47", x.hex());
}
