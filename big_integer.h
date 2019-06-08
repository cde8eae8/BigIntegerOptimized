//
// Created by nikita on 17.05.19.
//

#ifndef BIGINTEGEROPTIMIZED_BIG_INTEGER_H
#define BIGINTEGEROPTIMIZED_BIG_INTEGER_H

#include <string>
#include "NumberHolder.h"

class big_integer {
 public:
    big_integer();
    big_integer(const big_integer &bi);
    big_integer(int64_t number);
    explicit big_integer(std::string s, unsigned int radix = 10);
    big_integer &operator=(const big_integer& right);
    friend void swap(big_integer& lhs, big_integer& rhs);
    ~big_integer() = default;



    big_integer& operator+=(big_integer const &rhs);
    big_integer& operator-=(big_integer const &rhs);
    big_integer& operator*=(big_integer rhs);
    big_integer& operator/=(big_integer rhs);
    big_integer& operator%=(const big_integer &rhs);


    big_integer& operator&=(big_integer const &rhs);
    big_integer& operator|=(big_integer const &rhs);
    big_integer& operator^=(big_integer const &rhs);

    big_integer &operator<<=(uint64_t shift);
    big_integer &operator>>=(uint64_t shift);


    friend big_integer operator+(big_integer lhs, big_integer const &rhs);
    friend big_integer operator-(big_integer lhs, big_integer const &rhs);
    friend big_integer operator*(big_integer lhs, big_integer const &rhs);
    friend big_integer operator/(big_integer lhs, big_integer const &rhs);
    friend big_integer operator%(big_integer lhs, big_integer const &rhs);


    const big_integer& operator++();
    big_integer operator-() const;
    big_integer operator+() const;
    void invert();

    friend bool operator>(const big_integer &left, const big_integer &right);
    friend bool operator<(const big_integer &left, const big_integer &right);
    friend bool operator>=(const big_integer &left, const big_integer &right);
    friend bool operator<=(const big_integer &left, const big_integer &right);
    friend bool operator==(const big_integer &left, const big_integer &right);
    friend bool operator!=(const big_integer &left, const big_integer &right);

    friend big_integer operator&(big_integer left, const big_integer &right);
    friend big_integer operator|(big_integer left, const big_integer &right);
    friend big_integer operator^(big_integer left, const big_integer &right);
    friend big_integer operator~(big_integer left);
    friend big_integer operator<<(big_integer left, uint64_t shift);
    friend big_integer operator>>(big_integer left, uint64_t shift);


    void div_rem(uint64_t divider, uint64_t& remainder);

    static int cmp(big_integer const &lhs, big_integer const &rhs);

    bool isZero();
    bool isNegative() const;
    bool isPositive() const;
    void print(std::string s="") const;

// private:
    big_integer & addShort(uint64_t right);
    void mulShort(uint64_t right);

    static uint64_t addWithCarry(uint64_t &res, uint64_t a, uint64_t b);
    static uint64_t mulWithCarry(uint64_t &res, uint64_t a, uint64_t b);
    static void div128(uint64_t &res, uint64_t &remainder, uint64_t d1, uint64_t d2, uint64_t divider);

    NumberHolder number;
};

std::string to_string(big_integer bi, int radix=10);

void swap(big_integer& lhs, big_integer& rhs);


big_integer operator+(big_integer lhs, big_integer const &rhs);
big_integer operator-(big_integer lhs, big_integer const &rhs);
big_integer operator*(big_integer lhs, big_integer const &rhs);
big_integer operator/(big_integer lhs, big_integer const &rhs);
big_integer operator%(big_integer lhs, big_integer const &rhs);

bool operator>(const big_integer &left, const big_integer &right);
bool operator<(const big_integer &left, const big_integer &right);
bool operator>=(const big_integer &left, const big_integer &right);
bool operator<=(const big_integer &left, const big_integer &right);
bool operator==(const big_integer &left, const big_integer &right);
bool operator!=(const big_integer &left, const big_integer &right);

big_integer operator&(big_integer left, const big_integer &right);
big_integer operator|(big_integer left, const big_integer &right);
big_integer operator^(big_integer left, const big_integer &right);
big_integer operator~(big_integer left);

big_integer operator<<(big_integer left, uint64_t shift);
big_integer operator>>(big_integer left, uint64_t shift);

#endif //BIGINTEGEROPTIMIZED_BIG_INTEGER_H
