//
// Created by nikita on 16.04.19.
//

#include <string>
#include <gtest/gtest.h>
#include <climits>

#include "../big_integer.h"



// TODO: проверка, что мы не меняем объект при операциях
void checkNumber(const big_integer& actual, std::string expected) {
    if (expected != to_string(actual)) {
        std::cout << ">> Test for " << expected << " failed\n" <<
                  ">> expected\n" <<
                  ">> \"" << expected << "\"\n" <<
                  ">> actual\n" <<
                  ">> \"" << to_string(actual) << "\"" << std::endl;
        FAIL();
    }
}

void checkNumber(big_integer actual, big_integer expected) {
    if (expected != actual) {
        std::cout << ">> Test for " << to_string(expected) << " failed\n" <<
                  ">> expected\n" <<
                  ">> \"" << to_string(expected) << "\"\n" <<
                  ">> actual\n" <<
                  ">> \"" << to_string(actual) << "\"" << std::endl;
        FAIL();
    }
}


void checkNumber(std::string actual, std::string expected) {
    big_integer a(actual);
    std::string res = to_string(a);
    if (expected != res) {
        std::cout << ">> Test for " << actual << " failed\n" <<
                  ">> expected\n" <<
                  ">> \"" << expected << "\"\n" <<
                  ">> actual\n" <<
                  ">> \"" << res << "\"" << std::endl;
        FAIL();
    }
}


void checkAdd(std::string s1, std::string s2, std::string expected) {
    big_integer a(s1);
    big_integer b(s2);
    std::string res1 = to_string(a + b);
    std::string res2 = to_string(b + a);
    if (expected != res1  || expected != res2) {
        std::cout << ">> Test for " << s1 << " + " << s2 << " failed\n" <<
                  ">> expected\n" <<
                  ">> \"" << expected << "\"\n" <<
                  ">> actual\n" <<
                  ">> \"" << res1 << "\"\n" <<
                  ">> (\"" << to_string(a + b, 2) << "\")\n" <<
                  ">> \"" << res2 << "\"\n" <<
                  ">> (\"" << to_string(b + a, 2) << "\")\n" << std::endl;

        FAIL();
    }
}

void checkWithCopy(const big_integer& actual, std::string expected) {
    checkNumber(actual, expected);
    big_integer a(actual);
    big_integer b = actual;

    checkNumber(a, expected);
    checkNumber(b, expected);
}

void checkWithSpaces(std::string actual, std::string expected) {
    checkNumber(actual, expected);
//    checkNumber(actual, "         " + expected);
//    checkNumber(actual, expected + "          ");
//    checkNumber(actual, "         " + expected + "          ");
}

TEST(withInts, add) {
    big_integer a;

    a = a + 12345;
    checkNumber(a, "12345");
}

// 462

TEST(correctness, resize) {
    big_integer a("99999999999999999999999999999999999999");
    for (size_t i = 0; i < 10; ++i) {
        a.number.resize(i);
        std::cout << "-->" << a.number.size() << " " << a.number.capacity << " '" << to_string(a) << "'" << std::endl;

    }

}


TEST(correctness, mulshort) {
    big_integer a("99999999999999999999999999999999999999");
    for (size_t i = 0; i < 10; ++i) {
        a.mulShort(10);
        std::cout << "-->" << a.number.size() << " " << a.number.capacity << " '" << to_string(a) << "'" << std::endl;
    }
}


TEST(correctness, string_constructors) {
    checkWithSpaces("12345", "12345");
    checkWithSpaces("9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",
                    "9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
    checkWithSpaces("1", "1");
    checkWithSpaces("0", "0");
    checkWithSpaces("-1", "-1");
    checkWithSpaces("-123", "-123");
    checkWithSpaces("-0", "0");
//    checkWithSpaces("-9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",
//                    "-9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
//    checkWithSpaces("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
//                    "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
    checkWithSpaces("9223372036854775808", "9223372036854775808");
    checkWithSpaces("18446744073709551615", "18446744073709551615");
    checkWithSpaces("18446744073709599999", "18446744073709599999");
}

TEST(correctness, other_constructors) {
    checkWithCopy(big_integer(), "0");
    checkWithCopy(big_integer(0), "0");
    checkWithCopy(big_integer(12345678), "12345678");
    checkWithCopy(big_integer(-12345678), "-12345678");
    checkWithCopy(big_integer(1), "1");
    checkWithCopy(big_integer(-1), "-1");
    big_integer a = 1;
    checkWithCopy(a, "1");
    a = -1;
    checkWithCopy(a, "-1");
    a = 0;
    checkWithCopy(a, "0");
    a = INT_MAX;
    checkWithCopy(a, std::to_string(INT_MAX));
    a = INT_MIN;
    checkWithCopy(a, std::to_string(INT_MIN));
}

void testCompare_less(const big_integer &a, const big_integer &b) {
    if (a > b) FAIL();
    if (!(a < b)) FAIL();
    if (!(a != b)) FAIL();
    if (a == b) FAIL();
    if (a >= b) FAIL();
    if (!(a <= b)) FAIL();
}

void testCompare_greather(const big_integer &a, const big_integer &b) {
    std::cout << ">> Test for " << to_string(a) << " ? " << to_string(b) << "\n";
    std::cout << ">> res " << big_integer::cmp(a, b) << "\n";
    if (a < b) FAIL();
    if (!(a > b)) FAIL();
    if (!(a != b)) FAIL();
    if (a == b) FAIL();
    if (a <= b) FAIL();
    if (!(a >= b)) FAIL();
    testCompare_less(b, a);
}

void testCompare_equal(const big_integer &a, const big_integer &b) {
    if (a < b) FAIL();
    if (a > b) FAIL();
    if (a != b) FAIL();
    if (!(a == b)) FAIL();
    if (!(a <= b)) FAIL();
    if (!(a >= b)) FAIL();
}

TEST(correctness, compare) {
    testCompare_equal(big_integer(100), big_integer(100));
    testCompare_equal(big_integer(0), big_integer(0));
    testCompare_equal(big_integer(-100), big_integer(-100));
    testCompare_greather(big_integer(100), big_integer(0));
    testCompare_greather(big_integer(0), big_integer(-100));
    testCompare_greather(big_integer(100), big_integer(50));
    testCompare_greather(big_integer(-50), big_integer(-100));
}

TEST(coorectness, addition) {
    checkAdd("1", "1", "2");
    checkAdd("100", "100", "200");
    checkAdd("-1", "1", "0");
    checkAdd("0", "-10", "-10");
    checkAdd("0", "10", "10");

    checkAdd("18446744073709551615", "1", "18446744073709551616");
    checkAdd("18446744073709551615", "0", "18446744073709551615");
    checkAdd("9223372036854775807", "9223372036854775807", "18446744073709551614");
    checkAdd("18446744073709551616", "1", "18446744073709551617");
    checkAdd("18446744073709551610", "30", "18446744073709551640");

    checkAdd("18446744073709551616", "-1", "18446744073709551615");
    checkAdd("18446744073709551617", "-1", "18446744073709551616");
    checkAdd("18446744073709551640", "-30", "18446744073709551610");

    checkAdd("9223372036854775807", "1", "9223372036854775808");
    checkAdd("9223372036854775807", "0", "9223372036854775807");
    checkAdd("9223372036854775808", "1", "9223372036854775809");
    checkAdd("9223372036854775800", "30", "9223372036854775830");

    checkAdd("9223372036854775807", "-1", "9223372036854775806");
    checkAdd("9223372036854775808", "-1", "9223372036854775807");
    checkAdd("9223372036854775830", "-30", "9223372036854775800");



    checkAdd("100000000000000000000000000000000000", "-100000000000000000000000000000000000", "0");
    checkAdd("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", "1",
             "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
    checkAdd("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001", "1",
             "-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    checkAdd("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", "-1",
             "-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
    checkAdd("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001", "-1",
             "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
}

TEST (correctness, increment) {
    big_integer a("18446744073709551614");
    ++a;
    checkNumber(a, big_integer("18446744073709551615"));
    ++a;
    checkNumber(a, big_integer("18446744073709551616"));
    ++a;
    checkNumber(a, big_integer("18446744073709551617"));
    ++a;
    checkNumber(a, big_integer("18446744073709551618"));
    ++a;
    checkNumber(a, big_integer("18446744073709551619"));
    ++a;
    checkNumber(a, big_integer("18446744073709551620"));
    ++a;
    checkNumber(a, big_integer("18446744073709551621"));
    ++a;
    checkNumber(a, big_integer("18446744073709551622"));
    ++a;
    checkNumber(a, big_integer("18446744073709551623"));


    a = big_integer("9223372036854775807");
    ++a;
    checkNumber(a, big_integer("9223372036854775808"));
    ++a;
    checkNumber(a, big_integer("9223372036854775809"));
    ++a;
    checkNumber(a, big_integer("9223372036854775810"));
    ++a;
    checkNumber(a, big_integer("9223372036854775811"));
    ++a;
    checkNumber(a, big_integer("9223372036854775812"));
    ++a;
    checkNumber(a, big_integer("9223372036854775813"));
    ++a;
    checkNumber(a, big_integer("9223372036854775814"));
    ++a;
    checkNumber(a, big_integer("9223372036854775815"));
    ++a;
    checkNumber(a, big_integer("9223372036854775816"));

    a = big_integer("-2");
    ++a;
    checkNumber(a, big_integer("-1"));
    ++a;
    checkNumber(a, big_integer("0"));
    ++a;
    checkNumber(a, big_integer("1"));
    ++a;
    checkNumber(a, big_integer("2"));
    ++a;
    checkNumber(a, big_integer("3"));

}

TEST (correctness, shiftLeft) {
    big_integer a("1000000000000000000000");
    a <<= 1;
    checkNumber(a, "2000000000000000000000");
    a <<= 1;
    checkNumber(a, "4000000000000000000000");
    a <<= 1;
    checkNumber(a, "8000000000000000000000");
    a <<= 1;
    checkNumber(a, "16000000000000000000000");
    a <<= 256;
    checkNumber(a, "1852673427797059126777135760139006525652319754650249024631321344126610074238976000000000000000000000");
    a <<= 333;
    checkNumber(a, "32418090381882757488378186435087196492284736189394038281216072888208225089163344893747711319899248392876545989150787415487462117776654494592866209641515341305165482839074293153792000000000000000000000");

}

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

TEST(correctness, two_plus_two)
{
    EXPECT_EQ(big_integer(2) + big_integer(2), big_integer(4));
    EXPECT_EQ(big_integer(2) + 2             , 4); // implicit converion from int must work
    EXPECT_EQ(2              + big_integer(2), 4);
}

TEST(correctness, default_ctor)
{
    big_integer a;
    big_integer b = 0;
    EXPECT_EQ(a, 0);
    EXPECT_EQ(a, b);
}

TEST(correctness, ctor_limits)
{
    big_integer a = std::numeric_limits<int>::min();
    big_integer b = std::numeric_limits<int>::max();
    EXPECT_EQ(a + b, -1);
}

TEST(correctness, copy_ctor)
{
    big_integer a = 3;
    big_integer b = a;

    EXPECT_EQ(a, b);
    EXPECT_EQ(b, 3);
}

TEST(correctness, copy_ctor_real_copy)
{
    big_integer a = 3;
    big_integer b = a;
    a = 5;

    EXPECT_EQ(b, 3);
}

TEST(correctness, copy_ctor_real_copy2)
{
    big_integer a = 3;
    big_integer b = a;
    b = 5;

    EXPECT_EQ(a, 3);
}

TEST(correctness, assignment_operator)
{
    big_integer a = 4;
    big_integer b = 7;
    b = a;

    EXPECT_TRUE(a == b);
}

TEST(correctness, self_assignment)
{
    big_integer a = 5;
    a = a;

    EXPECT_TRUE(a == 5);
}

TEST(correctness, assignment_return_value)
{
    big_integer a = 4;
    big_integer b = 7;
    (a = b) = a;

    EXPECT_TRUE(a == 7);
    EXPECT_TRUE(b == 7);
}

TEST(correctness, comparisons)
{
    big_integer a = 100;
    big_integer b = 100;
    big_integer c = 200;

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(c > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(c >= a);
}

TEST(correctness, compare_zero_and_minus_zero)
{
    big_integer a;
    big_integer b = -a;

    EXPECT_TRUE(a == b);
}

TEST(correctness, add)
{
    big_integer a = 5;
    big_integer b = 20;

    EXPECT_TRUE(a + b == 25);

    a += b;
    EXPECT_TRUE(a == 25);
}

TEST(correctness, add_signed)
{
    big_integer a = 5;
    big_integer b = -20;

    EXPECT_TRUE(a + b == -15);

    a += b;
    EXPECT_TRUE(a == -15);
}

TEST(correctness, add_return_value)
{
    big_integer a = 5;
    big_integer b = 1;

    (a += b) += b;
    std::cout << to_string(a) << std::endl;
    EXPECT_EQ(a, 7);
}

TEST(correctness, sub)
{
    big_integer a = 20;
    big_integer b = 5;

    std::cout << to_string(a) << "-" << to_string(b) << std::endl;
    EXPECT_TRUE(a - b == 15);

    a -= b;
    EXPECT_TRUE(a == 15);
}

TEST(correctness, sub_signed)
{
    big_integer a = 5;
    big_integer b = 20;

    EXPECT_TRUE(a - b == -15);

    a -= b;
    EXPECT_TRUE(a == -15);

    a -= -100;
    EXPECT_TRUE(a == 85);
}

TEST(correctness, sub_return_value)
{
    big_integer a = 5;
    big_integer b = 1;

    (a -= b) -= b;
    EXPECT_EQ(a, 3);
}

TEST(correctness, mul)
{
    big_integer a = 5;
    big_integer b = 20;

    EXPECT_TRUE(a * b == 100);

    a *= b;
    EXPECT_TRUE(a == 100);
}

TEST(correctness, mul_signed)
{
    big_integer a = -5;
    big_integer b = 20;

    EXPECT_TRUE(a * b == -100);

    a *= b;
    EXPECT_TRUE(a == -100);
}

TEST(correctness, mul_return_value)
{
    big_integer a = 5;
    big_integer b = 2;

    (a *= b) *= b;
    EXPECT_EQ(a, 20);
}

TEST(correctness, div_)
{
    big_integer a = 20;
    big_integer b = 5;
    big_integer c = 20;

    checkNumber(a / b, "4");
    checkNumber(a % b, "0");

    a /= b;
    checkNumber(a, "4");

    c %= b;
    checkNumber(c, "0");
}

TEST(correctness, div_int_min)
{
    big_integer a = std::numeric_limits<int>::min();
    EXPECT_TRUE((a / a) == (a / std::numeric_limits<int>::min()));
}

TEST(correctness, div_int_min_2)
{
    big_integer a = std::numeric_limits<int>::min();
    big_integer b = -1;
    std::cout << "a = " << to_string(a) << ":" << std::numeric_limits<int>::min() << std::endl;
    std::cout << "b = " << to_string(b) << ":" << -1 << std::endl;

    big_integer c = a / b;
    big_integer d = a / -1;
    std::cout << "C " << to_string(c) << std::endl;
    std::cout << "A " << to_string(d) << std::endl;
    EXPECT_TRUE(c == (a / -1));

    std::cout << "LAST " << to_string((c - std::numeric_limits<int>::max())) << std::endl;
    EXPECT_TRUE((c - std::numeric_limits<int>::max()) == 1);
}

TEST(correctness, div_signed)
{
    big_integer a = -20;
    big_integer b = 5;

    EXPECT_TRUE(a / b == -4);
    EXPECT_TRUE(a % b == 0);
}

TEST(correctness, div_rounding)
{
    big_integer a = 23;
    big_integer b = 5;

    EXPECT_TRUE(a / b == 4);
    EXPECT_TRUE(a % b == 3);
}

TEST(correctness, div_rounding_negative)
{
    big_integer a = 23;
    big_integer b = -5;
    big_integer c = -23;
    big_integer d = 5;

    EXPECT_TRUE(a / b == -4);
    EXPECT_TRUE(c / d == -4);
    EXPECT_TRUE(a % b == 3);
    EXPECT_TRUE(c % d == -3);
}

TEST(correctness, div_return_value)
{
    big_integer a = 100;
    big_integer b = 2;

    (a /= b) /= b;
    EXPECT_EQ(a, 25);
}

TEST(correctness, unary_plus)
{
    big_integer a = 123;
    big_integer b = +a;

    EXPECT_TRUE(a == b);

    // this code should not compile:
    // &+a;
}

TEST(correctness, negation)
{
    big_integer a = 666;
    big_integer b = -a;

    EXPECT_TRUE(b == -666);
}

TEST(correctness, negation_int_min)
{
    big_integer a = std::numeric_limits<int>::min();
    big_integer b = -a;

    EXPECT_EQ(std::numeric_limits<int>::max(), b - 1);
}

TEST(correctness, and_)
{
    big_integer a = 0x55;
    big_integer b = 0xaa;

    EXPECT_TRUE((a & b) == 0);
    EXPECT_TRUE((a & 0xcc) == 0x44);
    a &= b;
    EXPECT_TRUE(a == 0);
}

TEST(correctness, and_signed)
{
    big_integer a = 0x55;
    big_integer b = 0xaa;

    EXPECT_TRUE((b & -1) == 0xaa);
    EXPECT_TRUE((a & (0xaa - 256)) == 0);
    EXPECT_TRUE((a & (0xcc - 256)) == 0x44);

    big_integer c = 0x55;
    big_integer d = 0xcc;
    EXPECT_EQ(big_integer(0x44), c & d);
}

TEST(correctness, and_return_value)
{
    big_integer a = 7;

    (a &= 3) &= 6;
    EXPECT_EQ(a, 2);
}

TEST(correctness, or_)
{
    big_integer a = 0x55;
    big_integer b = 0xaa;

    EXPECT_TRUE((a | b) == 0xff);
    a |= b;
    EXPECT_TRUE(a == 0xff);

    big_integer c = 0x55;
    big_integer d = 0xcc;
    EXPECT_EQ(big_integer(0xdd), c | d);
}

TEST(correctness, or_signed)
{
    big_integer a = 0x55;
    big_integer b = 0xaa;

    EXPECT_TRUE((a | (b - 256)) == -1);
}

TEST(correctness, or_return_value)
{
    big_integer a = 1;

    (a |= 2) |= 4;
    EXPECT_EQ(a, 7);
}

TEST(correctness, xor_)
{
    big_integer a = 0xaa;
    big_integer b = 0xcc;

    EXPECT_TRUE((a ^ b) == 0x66);

    big_integer c = 0x55;
    big_integer d = 0xcc;
    EXPECT_EQ(big_integer(0x99), c ^ d);
}

TEST(correctness, xor_signed)
{
    big_integer a = 0xaa;
    big_integer b = 0xcc;

    EXPECT_TRUE((a ^ (b - 256)) == (0x66 - 256));
}

TEST(correctness, xor_return_value)
{
    big_integer a = 1;

    (a ^= 2) ^= 1;
    EXPECT_EQ(a, 2);
}

TEST(correctness, not_)
{
    big_integer a = 0xaa;

    EXPECT_TRUE(~a == (-a - 1));
}

TEST(correctness, shl_)
{
    big_integer a = 23;

    EXPECT_TRUE((a << 5) == 23 * 32);

    a <<= 5;
    EXPECT_TRUE(a == 23 * 32);
}

TEST(correctness, shl_return_value)
{
    big_integer a = 1;

    (a <<= 2) <<= 1;
    EXPECT_EQ(a, 8);
}

TEST(correctness, shr_)
{
    big_integer a = 23;

    EXPECT_EQ(a >> 2, 5);

    a >>= 2;
    EXPECT_EQ(a, 5);
}

TEST(correctness, shr_31)
{
    big_integer a = 65536;

    EXPECT_EQ((a*a) >> 31, 2);
}

TEST(correctness, shr_signed)
{
    big_integer a = -1234;

    EXPECT_EQ(a >> 3, -155);

    a >>= 3;
    EXPECT_EQ(a, -155);
}

TEST(correctness, shr_return_value)
{
    big_integer a = 64;

    (a >>= 2) >>= 1;
    EXPECT_EQ(a, 8);
}

TEST(correctness, add_long)
{
    big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                     "100000000000000000000000000000000000000");
    big_integer c("10000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000");

    EXPECT_EQ(a + b, c);
}

TEST(correctness, add_long_signed)
{
    big_integer a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b( "1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(a + b, 0);
}

TEST(correctness, add_long_signed2)
{
    big_integer a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                     "100000000000000000000000000000000000000");
    big_integer c( "-999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");

    EXPECT_EQ(a + b, c);
}

TEST(correctness, add_long_pow2)
{
    big_integer a( "18446744073709551616");
    big_integer b("-18446744073709551616");
    big_integer c( "36893488147419103232");

    EXPECT_EQ(a + a, c);
    EXPECT_EQ(b + c, a);
    EXPECT_EQ(c + b, a);
}

TEST(correctness, sub_long)
{
    big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                     "100000000000000000000000000000000000000");
    big_integer c( "9999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");

    EXPECT_EQ(a - b, c);
}

TEST(correctness, mul_long)
{
    big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                     "100000000000000000000000000000000000000");
    big_integer c("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                  "00000000000000000000000000000000000000");

    EXPECT_EQ(a * b, c);
}

TEST(correctness, mul_long_signed)
{
    big_integer a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                     "100000000000000000000000000000000000000");
    big_integer c("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                  "00000000000000000000000000000000000000");

    EXPECT_EQ(a * b, c);
}

TEST(correctness, mul_long_signed2)
{
    big_integer a("-100000000000000000000000000");
    big_integer c("100000000000000000000000000"
                  "00000000000000000000000000");
    big_integer d = a * a;
//    std::cout << "res @@@" << to_string(d)  << "@@@" << std::endl;
    EXPECT_EQ(a * a, c);
}

TEST(correctness, mul_long_pow2)
{
    big_integer a("18446744073709551616");
    big_integer b("340282366920938463463374607431768211456");
    big_integer c("115792089237316195423570985008687907853269984665640564039457584007913129639936");

    checkNumber(a * a, b);
//    checkNumber(b * b, c);
}

TEST(correctness, div_long)
{
    big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                     "100000000000000000000000000000000000000");
    big_integer c("100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(a / b, c);
}

TEST(correctness, div_long_signed)
{
    big_integer a("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                      "100000000000000000000000000000000000000");
    big_integer c("-100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(a / b, c);
}

TEST(correctness, div_long_signed2)
{
    big_integer a("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b(                                                     "-100000000000000000000000000000000000000");
    big_integer c( "100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(a / b, c);
}

TEST(correctness, negation_long)
{
    big_integer a( "10000000000000000000000000000000000000000000000000000");
    big_integer c("-10000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(-a, c);
    EXPECT_EQ(a, -c);
}

TEST(correctness, shl_long)
{
    EXPECT_EQ(big_integer("1091951238831590836520041079875950759639875963123939936"),
              big_integer("34123476213487213641251283746123461238746123847623123") << 5);

    EXPECT_EQ(big_integer("-104637598388784443044449444577438556334703518260785595038524928"),
              big_integer("-817481237412378461284761285761238721364871236412387461238476") << 7);

    EXPECT_EQ(big_integer("26502603392713913241969902328696116541550413468869982914247384891392"),
              big_integer("12341236412857618761234871264871264128736412836643859238479") << 31);
}

TEST(correctness, shr_long)
{
    EXPECT_EQ(big_integer("4730073393008085198307104580698364137020387111323398632330851"),
              big_integer("151362348576258726345827346582347652384652387562348756234587245") >> 5);

    EXPECT_EQ(big_integer("1118311528397465815295799577134738919815767762822175104787"),
              big_integer("143143875634875624357862345873246581736418273641238413412741") >> 7);

    EXPECT_EQ(big_integer("-1591563309890326054125627839548891585559049824963"),
              big_integer("-3417856182746231874623148723164812376512852437523846123876") >> 31);

}

TEST(correctness, string_conv)
{
    EXPECT_EQ(to_string(big_integer("100")), "100");
    EXPECT_EQ(to_string(big_integer("0100")), "100");
    EXPECT_EQ(to_string(big_integer("0")), "0");
    EXPECT_EQ(to_string(big_integer("-0")), "0");
    EXPECT_EQ(to_string(big_integer("-1000000000000000")), "-1000000000000000");
}


namespace
{
    unsigned const number_of_iterations = 1;
    size_t const number_of_multipliers = 8;

    int myrand()
    {
        int val = rand() - RAND_MAX / 2;
        if (val != 0)
            return val;
        else
            return 1;
    }
}

TEST(correctness, mul_div_randomized)
{
    for (unsigned itn = 0; itn != number_of_iterations; ++itn)
    {
        std::vector<int> multipliers;

        for (size_t i = 0; i != number_of_multipliers; ++i)
            multipliers.push_back(myrand());

        big_integer accumulator = 1;

        for (size_t i = 0; i != number_of_multipliers; ++i)
            accumulator *= multipliers[i];

        std::random_shuffle(multipliers.begin(), multipliers.end());

        for (size_t i = 1; i != number_of_multipliers; ++i)
            accumulator /= multipliers[i];

        EXPECT_TRUE(accumulator == multipliers[0]);
    }
}


// 6277101735386680763835789423207281965606254139937917492456
namespace
{
    template <typename T>
    void erase_unordered(std::vector<T>& v, typename std::vector<T>::iterator pos)
    {
        std::cout << "bs" << to_string(v.back()) << " " << to_string(*pos) << std::endl;
        std::swap(v.back(), *pos);
        std::cout << "as" << to_string(v.back()) << " " << to_string(*pos) << std::endl;
        v.pop_back();
    }

    template <typename T>
    T extract_random_element(std::vector<T>& v)
    {
        std::cout << "extracting b" << std::endl;
        size_t index = rand() % v.size();
        std::cout << to_string(v[index]) << std::endl;
        T copy = v[index];
        erase_unordered(v, v.begin() + index);
        std::cout << "extracting e" << std::endl;
        return copy;
    }

    template <typename T>
    void merge_two(std::vector<T>& v)
    {
        assert(v.size() >= 2);

        T a = extract_random_element(v);
        T b = extract_random_element(v);

//        std::cout << to_string(a) << " * " << to_string(b) << std::endl;
        T ab = a * b;
//        if (ab / a != b) {
//            std::string sa = to_string(a);
//            std::string sb = to_string(b);
//            std::string sab = to_string(ab);
//            std::string actual = to_string(ab / a);
//            std::cout << "failed " << sab << " / " << sa << " != " << sb << "\n" <<
//                        "actual " << actual;
//            ASSERT_TRUE(ab / a == b);
//        }
//
//        if (ab / b != a) {
//            std::string sa = to_string(a);
//            std::string sb = to_string(b);
//            std::string sab = to_string(ab);
//            std::string actual = to_string(ab / b);
//            std::cout << "failed " << sab << " / " << sb << " != " << sa << "\n" <<
//                      "actual " << actual;
//            ASSERT_TRUE(ab / b == a);
//        }
        std::cout << "poped " << to_string(a) << " " << to_string(b) << std::endl;
        ASSERT_TRUE(ab / a == b);
        ASSERT_TRUE(ab / b == a);
//        std::cout << "returned " << ab << std::endl;
        std::cout << "pushed " << to_string(ab) << std::endl;
        v.push_back(ab);

        std::cout << "start" << std::endl;
        for (const auto &item : v) {
            item.number.print();
            std::cout << to_string(item) << std::endl;
        }
        std::cout << "end" << std::endl;
        std::cout << std::endl;
    }

    template <typename T>
    T merge_all(std::vector<T> v)
    {
//        for (const auto &item : v) {
//            std::cout << to_string(item) << std::endl;
//        }
//        std::cout << "^^^^^^^^^^^^^" << std::endl;
        assert(!v.empty());

        while (v.size() >= 2)
            merge_two(v);

        return v[0];
    }
}

TEST(correctness, mul_merge_randomized)
{
    for (unsigned itn = 0; itn != number_of_iterations; ++itn)
    {
        std::vector<big_integer> x;
        for (size_t i = 0; i != number_of_multipliers; ++i)
            x.push_back(myrand());

        big_integer a = merge_all(x);
        big_integer b = merge_all(x);

        std::cout << to_string(a) << " " << to_string(b) << std::endl;
        EXPECT_TRUE(a == b);
    }
}

namespace
{
    big_integer rand_big(size_t size)
    {
        big_integer result = rand();

        for (size_t i = 0; i != size; ++i)
        {
            result *= RAND_MAX;
            result += rand();
        }

        return result;
    }
}

TEST(correctness, div_randomized)
{
    for (size_t itn = 0; itn != number_of_iterations * number_of_multipliers; ++itn)
    {
        big_integer divident = rand_big(10);
        big_integer divisor = rand_big(6);
        big_integer quotient = divident / divisor;
        big_integer residue = divident % divisor;
        ASSERT_EQ(divident - quotient * divisor, residue);
        EXPECT_GE(residue, 0);
        EXPECT_LT(residue, divisor);
    }
}
