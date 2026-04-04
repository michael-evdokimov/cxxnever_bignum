#include <gtest/gtest.h>
#include <cxxnever_bigint/integer.hpp>

namespace
{
typedef cxxnever::integer<3, signed, uint16_t, uint32_t> int48_t;
typedef cxxnever::integer<3, unsigned, uint16_t, uint32_t> uint48_t;
typedef cxxnever::integer<6, signed, uint16_t, uint32_t> int96_t;
typedef cxxnever::integer<6, unsigned, uint16_t, uint32_t> uint96_t;
}

TEST(test_bigint, smoke)
{
    cxxnever::int128_t i = 1;

    EXPECT_EQ("1", i.str());
}

TEST(test_bigint, ctor)
{
    EXPECT_EQ("00:00:00:00:00:00", int48_t().hex());
    EXPECT_EQ("00:00:00:00:00:01", int48_t(1).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", int48_t(-1).hex());
    EXPECT_EQ("12:34:56:78:9a:bc", int48_t(0x123456789abcull).hex());
    EXPECT_EQ("00:00:00:00:00:01", uint48_t(1).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", uint48_t(-1).hex());

    EXPECT_EQ("00:00:00:00:00:00:00:00:00:00:00:00", int96_t().hex());
    EXPECT_EQ("00:00:00:00:00:00:00:00:00:00:00:01", int96_t(1).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff", int96_t(-1).hex());

    EXPECT_EQ("00:00:00:00:00:00:00:00:00:00:00:00", uint96_t().hex());
    EXPECT_EQ("00:00:00:00:00:00:00:00:00:00:00:01", uint96_t(1).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff", uint96_t(-1).hex());

    EXPECT_EQ("ff:ff:ff:ff:ff:ff", int48_t(int8_t(-1)).hex());
    EXPECT_EQ("00:00:00:00:00:ff", int48_t(uint8_t(-1)).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", int48_t(int16_t(-1)).hex());
    EXPECT_EQ("00:00:00:00:ff:ff", int48_t(uint16_t(-1)).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", int48_t(int32_t(-1)).hex());
    EXPECT_EQ("00:00:ff:ff:ff:ff", int48_t(uint32_t(-1)).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", int48_t(int64_t(-1)).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", int48_t(uint64_t(-1)).hex());

    EXPECT_EQ("ff:ff:ff:ff:ff:ff", uint48_t(int8_t(-1)).hex());
    EXPECT_EQ("00:00:00:00:00:ff", uint48_t(uint8_t(-1)).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", uint48_t(int16_t(-1)).hex());
    EXPECT_EQ("00:00:00:00:ff:ff", uint48_t(uint16_t(-1)).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", uint48_t(int32_t(-1)).hex());
    EXPECT_EQ("00:00:ff:ff:ff:ff", uint48_t(uint32_t(-1)).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", uint48_t(int64_t(-1)).hex());
    EXPECT_EQ("ff:ff:ff:ff:ff:ff", uint48_t(uint64_t(-1)).hex());
}

TEST(test_bigint, assign)
{
    int48_t i = -1;

    EXPECT_EQ("00:00:12:34:56:78", (i = 0x12345678).hex());

    EXPECT_EQ("00:00:00:00:00:01", (i = int8_t(1)).hex());
    EXPECT_EQ("00:00:00:00:00:02", (i = uint8_t(2)).hex());
    EXPECT_EQ("00:00:00:00:00:03", (i = int16_t(3)).hex());
    EXPECT_EQ("00:00:00:00:00:04", (i = uint16_t(4)).hex());
    EXPECT_EQ("00:00:00:00:00:05", (i = int32_t(5)).hex());
    EXPECT_EQ("00:00:00:00:00:06", (i = uint32_t(6)).hex());
    EXPECT_EQ("00:00:00:00:00:07", (i = int64_t(7)).hex());
    EXPECT_EQ("00:00:00:00:00:08", (i = uint64_t(8)).hex());
}

TEST(test_bigint, assign_convert)
{
    EXPECT_EQ(uint48_t(-1).hex(), (uint48_t() = int48_t(-1)).hex());
    EXPECT_EQ(int96_t(-1).hex(), (int96_t() = int48_t(-1)).hex());
}

inline std::vector<long long> numbers()
{
    std::vector<long long> r = {
        0,
        1, 2, 3, 4, 5, 6, 7, 10,
        15, 16, 17,
        31, 32, 33,
        63, 64, 65,
        100,
        127, 128, 129,
        255, 256, 257,
        511, 512, 513,
        1000,
        1023, 1024, 1025,
        10'000,
        0x3fff, 0x4000, 0x4001,
        0x7fff, 0x8000, 0x8001,
        0xffff, 0x1'0000, 0x1'0001,
    };

    auto copy = r;
    for (long long n: copy)
        r.push_back(-n);

    return r;
}

TEST(test_bigint, add)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            EXPECT_EQ(int48_t(a + b).hex(),
                      (int48_t(a) + int48_t(b)).hex()) << text;

            EXPECT_EQ(int48_t(a - b).hex(),
                      (int48_t(a) - int48_t(b)).hex()) << text;

            EXPECT_EQ(int48_t(a + b).hex(),
                      (int48_t(a) += int48_t(b)).hex()) << text;

            EXPECT_EQ(int48_t(a - b).hex(),
                      (int48_t(a) -= int48_t(b)).hex()) << text;
        }
    }
}

TEST(test_bigint, increment)
{
    int48_t i = 0;

    EXPECT_EQ(int48_t(0x10000).hex(), (++(i = 0xffff)).hex());
    EXPECT_EQ(int48_t(0x10000).hex(), i.hex());
    EXPECT_EQ(int48_t(0x0ffff).hex(), ((i = 0xffff)++).hex());
    EXPECT_EQ(int48_t(0x10000).hex(), i.hex());

    EXPECT_EQ(int48_t(0x0ffff).hex(), (--(i = 0x10000)).hex());
    EXPECT_EQ(int48_t(0x0ffff).hex(), i.hex());
    EXPECT_EQ(int48_t(0x10000).hex(), ((i = 0x10000)--).hex());
    EXPECT_EQ(int48_t(0x0ffff).hex(), i.hex());
}

TEST(test_bigint, add_small)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            int16_t c = b;
            if (c != b)
                continue;

            EXPECT_EQ(int48_t(a + b).hex(), (int48_t(a) += c).hex()) << text;
            EXPECT_EQ(int48_t(a - b).hex(), (int48_t(a) -= c).hex()) << text;
        }
    }
}

TEST(test_bigint, multiply)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            EXPECT_EQ(int48_t(a * b).hex(),
                      (int48_t(a) * int48_t(b)).hex()) << text;

            EXPECT_EQ(int48_t(a * b).hex(),
                      (int48_t(a) *= int48_t(b)).hex()) << text;
        }
    }
}

TEST(test_bigint, multiply_function)
{
    EXPECT_EQ(int48_t(4).hex(), int48_t(-1).multiply(2, 2).hex());
}

TEST(test_bigint, multiply_small)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            int16_t c = b;
            if (c != b)
                continue;

            if (c < 0)
                continue;

            EXPECT_EQ(int48_t(a * b).hex(),
                      int48_t(1).multiply_small(int48_t(a), c).hex()) << text;
        }
    }
}

TEST(test_bigint, compare)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            EXPECT_EQ(a == b, int48_t(a) == int48_t(b)) << text;
            EXPECT_EQ(a != b, int48_t(a) != int48_t(b)) << text;
            EXPECT_EQ(a <  b, int48_t(a) <  int48_t(b)) << text;
            EXPECT_EQ(a <= b, int48_t(a) <= int48_t(b)) << text;
            EXPECT_EQ(a >  b, int48_t(a) >  int48_t(b)) << text;
            EXPECT_EQ(a >= b, int48_t(a) >= int48_t(b)) << text;

            unsigned long long c = a, d = b;
            EXPECT_EQ(c == d, uint48_t(c) == uint48_t(d)) << text;
            EXPECT_EQ(c != d, uint48_t(c) != uint48_t(d)) << text;
            EXPECT_EQ(c <  d, uint48_t(c) <  uint48_t(d)) << text;
            EXPECT_EQ(c <= d, uint48_t(c) <= uint48_t(d)) << text;
            EXPECT_EQ(c >  d, uint48_t(c) >  uint48_t(d)) << text;
            EXPECT_EQ(c >= d, uint48_t(c) >= uint48_t(d)) << text;

            EXPECT_EQ(c < d, uint48_t(int48_t(a)) < uint48_t(int48_t(b))) << text;
            EXPECT_EQ(a < b, int48_t(uint48_t(c)) < int48_t(uint48_t(d))) << text;
        }
    }
}

TEST(test_bigint, convert_unsigned)
{
    EXPECT_EQ(int48_t(2).hex(), (int48_t(1) + uint48_t(1)).hex());
    EXPECT_EQ(true, int48_t(1) < uint48_t(-1));
}

TEST(test_bigint, convert_bool)
{
    EXPECT_EQ(false, bool(int48_t(0)));
    EXPECT_EQ(true, bool(int48_t(1)));
    EXPECT_EQ(true, bool(int48_t(2)));
}

TEST(test_bigint, convert_sized)
{
    EXPECT_EQ(int96_t(-2).hex(), (int48_t(-1) * int96_t(2)).hex());
    EXPECT_EQ(int48_t(-2).hex(), (int48_t(-1) * int48_t(int96_t(2))).hex());
}

TEST(test_bigint, negate)
{
    EXPECT_EQ(int48_t(-1).hex(), (-int48_t(1)).hex());
    EXPECT_EQ(int48_t(1).hex(), (-int48_t(-1)).hex());
    EXPECT_EQ(uint48_t(-1).hex(), int48_t(1).negate().hex());
}

TEST(test_bigint, shift)
{
    for (long long n: numbers()) {
        for (size_t i = 0; i != 48; i++) {
            char text[0x100] = {};
            snprintf(text, sizeof(text), "n=%lld; i=%zd;", n, i);

            EXPECT_EQ(int48_t(n << i).hex(), (int48_t(n) << i).hex()) << text;

            EXPECT_EQ(int48_t((n >> i) & 0xff'ff'ff'ff'ff'ffull).hex(),
                      (int48_t(n) >> i).hex()) << text;

            EXPECT_EQ(uint48_t((n & 0xff'ff'ff'ff'ff'ffull) >> i).hex(),
                      (uint48_t(n) >> i).hex()) << text;

            EXPECT_EQ((int48_t(n) << i).hex(), (int48_t(n) <<= i).hex()) << text;
            EXPECT_EQ((int48_t(n) >> i).hex(), (int48_t(n) >>= i).hex()) << text;
        }
    }
}

TEST(test_bigint, divide)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            if (b == 0)
                continue;

            EXPECT_EQ(int48_t(a / b).hex(), (int48_t(a) / int48_t(b)).hex()) << text;
            EXPECT_EQ(int48_t(a % b).hex(), (int48_t(a) % int48_t(b)).hex()) << text;

            EXPECT_EQ(int48_t(a / b).hex(), (int48_t(a) /= int48_t(b)).hex()) << text;
            EXPECT_EQ(int48_t(a % b).hex(), (int48_t(a) %= int48_t(b)).hex()) << text;
        }
    }
}

TEST(test_bigint, divide_func)
{
    const int48_t a = 7;
    const int48_t b = 3;
    int48_t r;
    int48_t m;
    r.divide(&r, a, b, &m);

    EXPECT_EQ(int48_t(7 / 3).hex(), r.hex());
    EXPECT_EQ(int48_t(7 % 3).hex(), m.hex());
}

TEST(test_bigint, divide_small)
{
    for (long long a: numbers()) {
        for (long long b: numbers()) {
            char text[0x100] = {};
            snprintf(text, sizeof(text), "a=%lld; b=%lld;", a, b);

            if (b == 0)
                continue;

            int16_t b2 = b;
            if (b2 != b)
                continue;

            int16_t remainder = 0;
            int48_t r = 0;
            r.divide_small(&r, a, b, &remainder);

            EXPECT_EQ(int48_t(a / b).hex(), r.hex()) << text;
            EXPECT_EQ(a % b, remainder) << text;
        }
    }
}

TEST(test_bigint, stringify)
{
    EXPECT_EQ("0", int48_t().str());
    EXPECT_EQ("1", int48_t(1).str());
    EXPECT_EQ("123456789", int48_t(123456789).str());
    EXPECT_EQ("-1", int48_t(-1).str());
    EXPECT_EQ("281474976710655", uint48_t(-1).str());
    EXPECT_EQ("11110001001000000", int48_t(123456).str(2));
    EXPECT_EQ("1e240", int48_t(123456).str(16));
    EXPECT_EQ("z", int48_t(35).str(36));
    EXPECT_EQ("10", int48_t(36).str(36));
}

TEST(test_bigint, parsing)
{
    EXPECT_EQ(int48_t(0).hex(), int48_t(1).from_str("0")->hex());
    EXPECT_EQ(int48_t(1).hex(), int48_t(2).from_str("1")->hex());
    EXPECT_EQ(int48_t(123456789).hex(), int48_t().from_str("123456789")->hex());
    EXPECT_EQ(int48_t(-1).hex(), int48_t().from_str("-1")->hex());
    EXPECT_EQ(uint48_t(-1).hex(), uint48_t().from_str("-1")->hex());
    EXPECT_EQ(int48_t(0xff).hex(), int48_t().from_str("ff", 16)->hex());
    EXPECT_EQ(int48_t(7).hex(), int48_t().from_str("111", 2)->hex());
    EXPECT_EQ(int48_t(35).hex(), int48_t().from_str("z", 36)->hex());
    EXPECT_EQ(int48_t(36).hex(), int48_t().from_str("10", 36)->hex());
    EXPECT_EQ(nullptr, int48_t().from_str("z"));
}

TEST(test_bigint, bitops)
{
    EXPECT_EQ(int48_t(0x55aa).hex(), (int48_t(0xaa55aa) & int48_t(0xffff)).hex());
    EXPECT_EQ(int48_t(0x55aa).hex(), (int48_t(0x5500) | int48_t(0xaa)).hex());
    EXPECT_EQ(int48_t(0x55aa).hex(), (int48_t(0xaa55) ^ int48_t(0xffff)).hex());

    EXPECT_EQ(int48_t(0x55aa).hex(), (int48_t(0xaa55aa) &= int48_t(0xffff)).hex());
    EXPECT_EQ(int48_t(0x55aa).hex(), (int48_t(0x5500) |= int48_t(0xaa)).hex());
    EXPECT_EQ(int48_t(0x55aa).hex(), (int48_t(0xaa55) ^= int48_t(0xffff)).hex());
}
