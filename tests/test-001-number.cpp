#include <gtest/gtest.h>
#include <cxxnever_bignum/number.hpp>


namespace {

typedef cxxnever::details::number<int16_t, uint32_t> num_t;

}


TEST(test_bignum, smoke)
{
    cxxnever::number n = 13;

    EXPECT_EQ("13", n.str());
}

TEST(test_bignum, ctor_simple)
{
    EXPECT_EQ("", num_t().hex());
    EXPECT_EQ("00:01", num_t(1).hex());
    EXPECT_EQ("00:07", num_t(7).hex());
    EXPECT_EQ("00:0f", num_t(15).hex());
    EXPECT_EQ("00:10", num_t(16).hex());
    EXPECT_EQ("00:7f", num_t(127).hex());
    EXPECT_EQ("00:80", num_t(128).hex());
    EXPECT_EQ("01:00", num_t(256).hex());
    EXPECT_EQ("01:03", num_t(259).hex());
    EXPECT_EQ("7f:ff", num_t(32767).hex());
    EXPECT_EQ("00:00:80:00", num_t(32768).hex());
    EXPECT_EQ("01:02:03:04", num_t(16909060).hex());

    EXPECT_EQ("ff:ff", num_t(-1).hex());
    EXPECT_EQ("ff:fe", num_t(-2).hex());
    EXPECT_EQ("ff:80", num_t(-128).hex());
    EXPECT_EQ("80:00", num_t(-32768).hex());
    EXPECT_EQ("ff:ff:7f:ff", num_t(-32769).hex());
    EXPECT_EQ("ff:ff:7f:fe", num_t(-32770).hex());
    EXPECT_EQ("ff:ff:00:00", num_t(-65536).hex());
    EXPECT_EQ("ff:00:00:00", num_t(-16777216).hex());
    EXPECT_EQ("80:00:00:00", num_t(-2147483648ll).hex());
}

std::vector<long long> numbers()
{
    std::vector<long long> numbers = {
        0, 1, 2, 3, 5, 7,
        15, 16, 17,
        31, 32, 33,
        63, 64, 65,
        100,
        127, 128, 129,
        255, 256, 257,
        511, 512, 513,
        1000,
        1023, 1024, 1025,
        0x7f'ff, 0x80'00, 0x80'01,
        0xff'ff, 0x1'00'00, 0x1'00'01,
        0x7f'00'00'00,
        0x7f'ff'ff'ff, 0x80'00'00'00ll, 0x80'00'00'01ll,
    };

    auto copy = numbers;
    for (long long i: copy)
        numbers.push_back(-i);

    return numbers;
}

TEST(test_bignum, negate)
{
    for (long long n: numbers())
        EXPECT_EQ(num_t(-n).hex(), (-num_t(n)).hex()) << "n=" << n;
}

TEST(test_bignum, negate_function)
{
    num_t x = -1;
    x.negate();
    EXPECT_EQ(num_t(1).hex(), x.hex());
}

TEST(test_bignum, add_subtract)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {0};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            EXPECT_EQ(num_t(a + b).hex(), (num_t(a) + num_t(b)).hex()) << text;
            EXPECT_EQ(num_t(a - b).hex(), (num_t(a) - num_t(b)).hex()) << text;
        }
    }
}

TEST(test_bignum, multiply)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {0};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            EXPECT_EQ(num_t(a * b).hex(), (num_t(a) * num_t(b)).hex()) << text;
        }
    }
}

TEST(test_bignum, divide)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {0};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            if (b == 0)
                continue;

            EXPECT_EQ(num_t(a / b).hex(), (num_t(a) / num_t(b)).hex()) << text;
            EXPECT_EQ(num_t(a % b).hex(), (num_t(a) % num_t(b)).hex()) << text;
        }
    }
}

TEST(test_bignum, divide_zero)
{
    EXPECT_THROW(num_t(1) / num_t(0), std::runtime_error);
}

TEST(test_bignum, compare)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {0};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            EXPECT_EQ(num_t(a) == num_t(b), a == b) << text;
            EXPECT_EQ(num_t(a) != num_t(b), a != b) << text;
            EXPECT_EQ(num_t(a) <  num_t(b), a <  b) << text;
            EXPECT_EQ(num_t(a) <= num_t(b), a <= b) << text;
            EXPECT_EQ(num_t(a) >  num_t(b), a >  b) << text;
            EXPECT_EQ(num_t(a) >= num_t(b), a >= b) << text;
        }
    }
}

TEST(test_bignum, compare_signed)
{
    EXPECT_EQ(num_t(0x18000) > num_t(0x10000), true);
}

TEST(test_bignum, shift)
{
    for (long long num: numbers()) {
        for (size_t n = 0; n != 31; n++) {
            char text[0x100] = {0};
            snprintf(text, sizeof(text), "num=%lld; n=%zd;", num, n);

            EXPECT_EQ(num_t(num << n).hex(), (num_t(num) << n).hex()) << text;
            EXPECT_EQ(num_t(num >> n).hex(), (num_t(num) >> n).hex()) << text;
        }
    }
}

TEST(test_bignum, shift_assign)
{
    num_t a = 0xffff;
    EXPECT_EQ(num_t(0x7fff).hex(), (a >>= 1).hex());
    EXPECT_EQ(num_t(0x7fff).hex(), a.hex());

    num_t b = 0xffff;
    EXPECT_EQ(num_t(0x1fffe).hex(), (b <<= 1).hex());
    EXPECT_EQ(num_t(0x1fffe).hex(), b.hex());
}

TEST(test_bignum, stringify)
{
    for (long long num: numbers()) {
        char name[0x100] = {0};
        snprintf(name, sizeof(name), "%lld", num);

        EXPECT_EQ(name, num_t(num).str());
    }
}

TEST(test_bignum, stringify_binary)
{
    EXPECT_EQ("0", num_t().str(2));
    EXPECT_EQ("1", num_t(1).str(2));
    EXPECT_EQ("10", num_t(2).str(2));
    EXPECT_EQ("11", num_t(3).str(2));
    EXPECT_EQ("100", num_t(4).str(2));
    EXPECT_EQ("1111", num_t(15).str(2));
    EXPECT_EQ("10000", num_t(16).str(2));
    EXPECT_EQ("10001", num_t(17).str(2));
    EXPECT_EQ("111111111111111", num_t(0x7fff).str(2));
    EXPECT_EQ("1000000000000000", num_t(0x8000).str(2));
    EXPECT_EQ("1000000000000001", num_t(0x8001).str(2));
    EXPECT_EQ("10010001101000101011001111000", num_t(0x12345678).str(2));
    EXPECT_EQ("-1", num_t(-1).str(2));
    EXPECT_EQ("-10", num_t(-2).str(2));
    EXPECT_EQ("-11", num_t(-3).str(2));
}

TEST(test_bignum, stringify_hex)
{
    EXPECT_EQ("0", num_t().str(16));
    EXPECT_EQ("1", num_t(1).str(16));
    EXPECT_EQ("2", num_t(2).str(16));
    EXPECT_EQ("3", num_t(3).str(16));
    EXPECT_EQ("4", num_t(4).str(16));
    EXPECT_EQ("f", num_t(0x0f).str(16));
    EXPECT_EQ("10", num_t(0x10).str(16));
    EXPECT_EQ("ff", num_t(0xff).str(16));
    EXPECT_EQ("12345678", num_t(0x12345678).str(16));
    EXPECT_EQ("-1", num_t(-1).str(16));
    EXPECT_EQ("-3", num_t(-3).str(16));
    EXPECT_EQ("-ff", num_t(-0xff).str(16));
}

TEST(test_bignum, stringify_36)
{
    EXPECT_EQ("0", num_t().str(36));
    EXPECT_EQ("1", num_t(1).str(36));
    EXPECT_EQ("f", num_t(15).str(36));
    EXPECT_EQ("g", num_t(16).str(36));
    EXPECT_EQ("z", num_t(35).str(36));
    EXPECT_EQ("10", num_t(36).str(36));
    EXPECT_EQ("11", num_t(37).str(36));
    EXPECT_EQ("17", num_t(36 + 7).str(36));
    EXPECT_EQ("-1", num_t(-1).str(36));
    EXPECT_EQ("-z", num_t(-35).str(36));
    EXPECT_EQ("-10", num_t(-36).str(36));
}

TEST(test_bignum, simple_add)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {0};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            if (int16_t(b) != b)
                continue;

            EXPECT_EQ(num_t(a + b).hex(), (num_t(a) += int16_t(b)).hex()) << text;
            EXPECT_EQ(num_t(a - b).hex(), (num_t(a) -= int16_t(b)).hex()) << text;
        }
    }
}

TEST(test_bignum, increment)
{
    {
        num_t a = 11;
        num_t& b = ++a;
        EXPECT_EQ(num_t(12).hex(), a.hex());
        EXPECT_EQ(num_t(12).hex(), b.hex());
    }

    {
        num_t a = 11;
        num_t b = a++;
        EXPECT_EQ(num_t(12).hex(), a.hex());
        EXPECT_EQ(num_t(11).hex(), b.hex());
    }

    {
        num_t a = 11;
        num_t& b = --a;
        EXPECT_EQ(num_t(10).hex(), a.hex());
        EXPECT_EQ(num_t(10).hex(), b.hex());
    }

    {
        num_t a = 11;
        num_t b = a--;
        EXPECT_EQ(num_t(10).hex(), a.hex());
        EXPECT_EQ(num_t(11).hex(), b.hex());
    }
}

TEST(test_bignum, operators_assign_arithm)
{
    num_t x = 11;

    EXPECT_EQ(num_t(13).hex(), (x += num_t(2)).hex());
    EXPECT_EQ(num_t(13).hex(), x.hex());

    x = 11;
    EXPECT_EQ(num_t(7).hex(), (x -= num_t(4)).hex());
    EXPECT_EQ(num_t(7).hex(), x.hex());

    x = 11;
    EXPECT_EQ(num_t(77).hex(), (x *= num_t(7)).hex());
    EXPECT_EQ(num_t(77).hex(), x.hex());

    x = 11;
    EXPECT_EQ(num_t(2).hex(), (x /= num_t(4)).hex());
    EXPECT_EQ(num_t(2).hex(), x.hex());

    x = 11;
    EXPECT_EQ(num_t(3).hex(), (x %= num_t(4)).hex());
    EXPECT_EQ(num_t(3).hex(), x.hex());
}

TEST(test_bignum, divide_function)
{
    num_t r = 11;
    num_t m;

    num_t& self = r.divide(num_t(17), num_t(11), &m);

    EXPECT_EQ(num_t(1).hex(), r.hex());
    EXPECT_EQ(num_t(6).hex(), m.hex());
    EXPECT_EQ(num_t(1).hex(), self.hex());

    r.divide(num_t(7), num_t(2));
    EXPECT_EQ(num_t(3).hex(), r.hex());
}

TEST(test_bignum, from_hex)
{
    EXPECT_EQ("", num_t().from_hex("").hex());
    EXPECT_EQ("00:01", num_t().from_hex("00:01").hex());
    EXPECT_EQ("ff:ff", num_t().from_hex("ff:ff").hex());
    EXPECT_EQ("00:01:ff:ff", num_t().from_hex("00:01:ff:ff").hex());

    EXPECT_THROW(num_t().from_hex("0"), std::runtime_error);
    EXPECT_THROW(num_t().from_hex("0x"), std::runtime_error);
    EXPECT_THROW(num_t().from_hex("00:"), std::runtime_error);
    EXPECT_THROW(num_t().from_hex("00"), std::runtime_error);
    EXPECT_THROW(num_t().from_hex("00:00:00"), std::runtime_error);
}

TEST(test_bignum, from_str)
{
    EXPECT_EQ(num_t().hex(),           num_t().from_str("0")->hex());
    EXPECT_EQ(num_t(1).hex(),          num_t().from_str("1")->hex());
    EXPECT_EQ(num_t(9).hex(),          num_t().from_str("9")->hex());
    EXPECT_EQ(num_t(10).hex(),         num_t().from_str("10")->hex());
    EXPECT_EQ(num_t(255).hex(),        num_t().from_str("255")->hex());
    EXPECT_EQ(num_t(32767).hex(),      num_t().from_str("32767")->hex());
    EXPECT_EQ(num_t(32768).hex(),      num_t().from_str("32768")->hex());
    EXPECT_EQ(num_t(2147483647).hex(), num_t().from_str("2147483647")->hex());

    EXPECT_EQ(num_t(-1).hex(), num_t().from_str("-1")->hex());
    EXPECT_EQ(num_t(-2).hex(), num_t().from_str("-2")->hex());

    EXPECT_EQ("00:0c:9f:2c:9c:d0:46:74:ed:ea:40:00:00:00",
              num_t().from_str("1000000000000000000000000000000")->hex());

    EXPECT_FALSE(num_t().from_str(""));
    EXPECT_FALSE(num_t().from_str("af"));
    EXPECT_FALSE(num_t().from_str("--1"));
}

TEST(test_bignum, from_str16)
{
    EXPECT_EQ(num_t().hex(),         num_t().from_str("0", 16)->hex());
    EXPECT_EQ(num_t(1).hex(),        num_t().from_str("1", 16)->hex());
    EXPECT_EQ(num_t(0x0f).hex(),     num_t().from_str("f", 16)->hex());
    EXPECT_EQ(num_t(0x10).hex(),     num_t().from_str("10", 16)->hex());
    EXPECT_EQ(num_t(0x7fff).hex(),   num_t().from_str("7fff", 16)->hex());
    EXPECT_EQ(num_t(0x8000).hex(),   num_t().from_str("8000", 16)->hex());
    EXPECT_EQ(num_t(0xf1234).hex(),  num_t().from_str("f1234", 16)->hex());
    EXPECT_EQ(num_t(-1).hex(),       num_t().from_str("-1", 16)->hex());
    EXPECT_EQ(num_t(-2).hex(),       num_t().from_str("-2", 16)->hex());
    EXPECT_EQ(num_t(-0xf1234).hex(), num_t().from_str("-f1234", 16)->hex());

    EXPECT_FALSE(num_t().from_str("", 16));
    EXPECT_FALSE(num_t().from_str("g", 16));
    EXPECT_FALSE(num_t().from_str("--1", 16));
}

TEST(test_bignum, from_str36)
{
    EXPECT_EQ(num_t(1).hex(), num_t().from_str("1", 36)->hex());
    EXPECT_EQ(num_t(35).hex(), num_t().from_str("z", 36)->hex());
    EXPECT_EQ(num_t(36).hex(), num_t().from_str("10", 36)->hex());
}

TEST(test_bignum, multiply_function)
{
    EXPECT_EQ(num_t(77).hex(), num_t().multiply(num_t(7), num_t(11)).hex());
}

TEST(test_bignum, multiply_karatsuba)
{
    num_t num_basic = 1;
    num_t num_new = 1;
    num_t item;

    for (long long num = 1; num < 100; num++) {
        item = num;
        num_basic *= item;

        num_t r;
        r.multiply_karatsuba(num_new, item);
        num_new = std::move(r);

        EXPECT_TRUE(num_basic == num_new) << "num=" << num_basic.hex();

        ASSERT_EQ((num_basic * num_basic).hex(),
                  num_t().multiply_karatsuba(num_basic, num_basic).hex()) <<
            "num = " << num_basic.hex();
    }

    EXPECT_EQ(num_t(100).hex(), num_t().multiply_karatsuba(10, 10).hex());
    EXPECT_EQ(num_t(-100).hex(), num_t().multiply_karatsuba(-10, 10).hex());
    EXPECT_EQ(num_t(-100).hex(), num_t().multiply_karatsuba(10, -10).hex());
    EXPECT_EQ(num_t(100).hex(), num_t().multiply_karatsuba(-10, -10).hex());
}

TEST(test_bignum, compat_with_ver1)
{
    const char* million = "123123123123123123123123123123123";

    EXPECT_EQ(million, num_t(million).str());

    EXPECT_EQ(million, (num_t() = million).str());
    EXPECT_THROW(num_t() = "0f", std::runtime_error);

    num_t x;
    EXPECT_EQ(true, x.assign(million));
    EXPECT_EQ(million, x.str());
    EXPECT_EQ(false, x.assign("0f"));

    EXPECT_EQ(true, x.assign("ffff", 16));
    EXPECT_EQ("ffff", x.str(16));
}
