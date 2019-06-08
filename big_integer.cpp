//
// Created by nikita on 12.04.19.
//

#include "big_integer.h"

#include <cstring>
#include <iterator>
#include <iostream>
#include <algorithm>


std::string to_string(big_integer bi, int radix) {
    std::string res;

    bool negative = bi.isNegative(); //
    if (negative)
        bi.invert();            //
    do {
        uint64_t remainder;
        bi.div_rem(radix, remainder);
        res.push_back(static_cast<char>(remainder + '0'));
    } while (!bi.isZero());     //
    if (negative)
        res.push_back('-');
    std::reverse(res.begin(), res.end());
    return res;
}

bool big_integer::isZero() {
    for (const auto &digit : number) {
        if (digit != 0) return false;
    }
    return true;
}

bool big_integer::isNegative() const {
    return this->number.sign() != 0;
}

// return: this >= 0
bool big_integer::isPositive() const {
    return !isNegative();
}

big_integer::big_integer() : number() {}

big_integer::big_integer(int64_t _number) : number(_number) { }

big_integer::big_integer(const big_integer &bi) : number(bi.number) { }

big_integer &big_integer::operator=(const big_integer& right) {
//    std::cout << "was " << to_string(right) << std::endl;
    this->number = right.number;
//    std::cout << "now " << to_string(*this) << std::endl;
    return *this;
}

void swap(big_integer &lhs, big_integer &rhs) {
//    std::cout << "***bswap\n" << to_string(lhs) << " " << to_string(rhs) << "|" <<  std::endl;
//    swap(lhs.number, rhs.number);
    big_integer a = rhs;

//    std::cout << "***cswap\n" << to_string(lhs) << " " << to_string(rhs) << " " << to_string(a) << "|" <<  std::endl;
    rhs = lhs;
//    std::cout << "***dswap\n" << to_string(lhs) << " " << to_string(rhs) << " " << to_string(a) << "|" <<  std::endl;
    lhs = a;
//    std::cout << "***aswap\n" << to_string(lhs) << " " << to_string(rhs) << " " << to_string(a) << "|" <<  std::endl;
}

big_integer::big_integer(std::string s, unsigned int radix) : big_integer() {
    size_t i = 0;
    bool isNegative = false;
    if (s[0] == '-') {
        isNegative = (s != "-0");
        ++i;
    }
    for ( ; i < s.size(); ++i) {
        uint64_t digit = 0;
        if ('0' <= s[i] && s[i] <= '9')
            digit = static_cast<uint64_t>(s[i] - '0');
        else if ('a' <= s[i] && s[i] <= 'f')
            digit = static_cast<uint64_t>(s[i] - 'a' + 10);

        mulShort(radix);
//        std::cout << to_string(*this) << std::endl;
        addShort(digit);
//        std::cout << to_string(*this) << std::endl;
//        print();
    }
//    print("sres");
    if (isNegative)
        invert();
//    print("sneg");
    number.setSign(!isNegative);
//    print("res ");
}

//big_integer::~big_integer() {
//    free(this->digits);
//}

uint64_t big_integer::addWithCarry(uint64_t &res, uint64_t a, uint64_t b) {
    uint64_t carry = 0;
    asm volatile ("add %2, %1\n"
                  "adc $0, %0"
    : "+r" (carry), "+r" (a)
    : "r" (b));
    res = a;
    return carry;
}

uint64_t big_integer::mulWithCarry(uint64_t &res, uint64_t a, uint64_t b) {
    uint64_t carry = 0;
    asm volatile ("mov %2, %%rax \n"
                  "mul %1 \n"
                  "mov %%rax, %1 \n"
                  "mov %%rdx, %0"
    : "+r" (carry), "+r" (a)
    : "r" (b)
    : "rdx", "rax");
    res = a;
    return carry;
}

void big_integer::div128(uint64_t &res, uint64_t &remainder, uint64_t d1, uint64_t d2, uint64_t divider) {
    uint64_t r = 0;
    uint64_t rem = 0;
    asm volatile ("mov %2, %%rdx \n"
                  "mov %3, %%rax \n"
                  "mov %4, %%rcx \n"
                  "div %%rcx \n"
                  "mov %%rax, %0 \n"
                  "mov %%rdx, %1"
    : "=r" (r), "=r" (rem)
    : "r" (d1), "r" (d2), "r" (divider)
    : "rdx", "rax", "rcx");
    res = r;
    remainder = rem;
}

const big_integer& big_integer::operator++() {
    uint64_t carry = 1;
    bool sign = isPositive();

    for (auto &digit : number) {
        if (carry == 0) break;
        carry = big_integer::addWithCarry(digit, digit, carry);
    }
    if (!carry)
        number.setSign(sign);
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer res(*this);
    res.invert();
    return res;
}

big_integer big_integer::operator+() const {
    return (*this);
}

void big_integer::invert() {
    for (auto &digit : number) {
        digit = ~digit;
    }
    this->operator++();
}

// TODO: dont make copy
big_integer &big_integer::addShort(uint64_t right) {
//    print("badd");
    big_integer &res = *this;
    uint64_t carry = right;
    auto digit = res.number.begin();
    auto end = res.number.end();
    for ( ; digit != end; ++digit) {
        carry = big_integer::addWithCarry(*digit, *digit, carry);
    }
//    print("add ");
    if (carry) {
        printf("+:resize");
        number.resize(res.number.size() + 1);
        digit = res.number.begin();
        digit[this->number.size() - 1] = ~UINT64_C(0);
//        res.number.set(res.number.size() - 1, ~UINT64_C(0));
    }
//    print("add2");
    return res;
}

big_integer operator+(big_integer lhs, big_integer const &rhs) {
    return lhs += rhs;
}

big_integer operator-(big_integer lhs, big_integer const &rhs) {
    return lhs -= rhs;
}

big_integer operator*(big_integer lhs, big_integer const &rhs) {
    return lhs *= rhs;
}

big_integer operator/(big_integer lhs, big_integer const &rhs) {
//    std::cout << to_string(lhs) << "/" << to_string(rhs) << std::endl;
    return lhs /= rhs;
}

big_integer operator%(big_integer lhs, big_integer const &rhs) {
    return lhs %= rhs;
}


// TODO: dont make copy
void big_integer::mulShort(uint64_t right) {
//    std::cout << "mul " << to_string(*this) << std::endl;
    big_integer &res = *this;
//    print("mbeg");
//    bool positiveResult = right < 0 == res.isNegative();
    bool positiveResult = res.isPositive(); // because its version for uint64_t
    if (res.isNegative())
        res.invert();
//    print("inv ");
    uint64_t carry = 0;
    auto digit = res.number.begin();
    auto end = res.number.end();
    for ( ; digit != end; ++digit) {
        uint64_t c = big_integer::mulWithCarry(*digit, *digit, right);
        carry = big_integer::addWithCarry(*digit, *digit, carry);
        carry += c;
    }
//    print("carr");
    if (carry) {
//        std::cout << res.number.size() + 1 << " " << res.number.size() << std::endl;
        res.number.resize(res.number.size() + 1);
        digit = res.number.begin();
        digit[this->number.size() - 1] = carry; // TODO after resize can change
//        res.number.set(res.number.size() - 1, carry);
    }
//    print("res ");
    if (!positiveResult)
        res.invert();
//    print("2inv");
//    std::cout << "aft " << to_string(*this) << std::endl;
    res.number.setSign(positiveResult);
}


big_integer& big_integer::operator+=(big_integer const &rhs) {
//    uint64_t carry, newCarry;
//    carry = 0;
//    int resultSign;
//    if (rhs.isNegative() && isNegative())
//        resultSign = -1;
//    else if (!rhs.isNegative() && !isNegative())
//        resultSign = 1;
//    else
//        resultSign = 0;
////    print("lef");
////    rhs.print("rig");
//
//
//    number.resize(rhs.number.size());
//    auto rightDigit = rhs.number.getConstInfinityIterator(0);
//    for (auto &digit : number) {
//        newCarry = big_integer::addWithCarry(digit, digit, *rightDigit);
//        newCarry += big_integer::addWithCarry(digit, digit, carry);
//        carry = newCarry;
//        ++rightDigit;
//    }
////    print("add");
//    if (resultSign) {
//        number.setSign(resultSign == 1);
//    }
////    print("bcl");
//    number.clear();
////    print("res");
//
//    return (*this);
    uint64_t carry, newCarry;
    carry = newCarry = 0;
    int resultSign;
    if (rhs.isNegative() && isNegative())
        resultSign = -1;
    else if (!rhs.isNegative() && !isNegative())
        resultSign = 1;
    else
        resultSign = 0;
    number.resize(rhs.number.size());
    size_t len = std::min(number.size(), rhs.number.size());
    size_t i;
    auto digits = number.begin();
    auto rhsDigit = rhs.number.begin();
    for (i = 0; i < len; ++i) {
        newCarry = big_integer::addWithCarry(digits[i], digits[i], rhsDigit[i]);
        newCarry += big_integer::addWithCarry(digits[i], digits[i], carry);
        carry = newCarry;
//        this->print();
    }
//    print();
    uint64_t highBit = rhs.isNegative() ? NumberHolder::NEGATIVE_SIGN : NumberHolder::POSITIVE_SIGN;
    while (i < number.size()) {
        newCarry = big_integer::addWithCarry(digits[i], digits[i], carry);
        newCarry += big_integer::addWithCarry(digits[i], digits[i], highBit);
        carry = newCarry;
        i++;
//        this->print();

    }
//    print("add");
    if (resultSign) {
        number.setSign(resultSign == 1);
    }
    number.clear();
//    print("res");

    return (*this);

}

big_integer& big_integer::operator-=(big_integer const &rhs) {
    this->invert();                 // -lhs
    this->operator+=(rhs);           // rhs - lhs
    this->invert();                 // lhs - rhs
    return (*this);
}

big_integer& big_integer::operator*=(big_integer rhs) {
//    std::cout << to_string(*this) << "*" << to_string(rhs) << std::endl;
    big_integer res;
    big_integer &left = *this;
    bool resultSign = (rhs.isNegative() == isNegative());
    if (rhs.isNegative())
        rhs.invert();
    if (isNegative())
        left.invert();
    number.resize(rhs.number.size());
    size_t len = std::min(number.size(), rhs.number.size());
    size_t i;
    size_t shift = 0;
    auto rightDigit = rhs.number.getConstInfinityIterator(0);
    big_integer copy;
    for (i = 0; i < len; ++i) {
        copy = *this;
        copy.mulShort(*rightDigit);
        copy <<= shift;
        res += copy;

        shift += 64;
        ++rightDigit;
    }
    if(!resultSign)
        res.invert();
    number.setSign(resultSign);

    swap(res, *this);
    number.clear();
    return (*this);
}


// TODO: copy constructor
big_integer& big_integer::operator/=(big_integer rhs) {
//    std::cout << to_string(*this) << "/" << to_string(rhs) << std::endl;
    big_integer res;
    big_integer &left = *this;
    bool resultSign = (rhs.isNegative() == isNegative());
    if (rhs.isNegative())
        rhs.invert();
    if (isNegative())
        left.invert();
    if (rhs.number.getRealSize() == 1) {
        uint64_t rem;
        this->div_rem(*rhs.number.begin(), rem);
        if(!resultSign)
            this->invert();
        return (*this);
    }
    if (*this < rhs) {
        *this = big_integer();
        return (*this);
    } else if (*this == rhs) {
        *this = big_integer(1);
        if (!resultSign)
            this->invert();
        return (*this);
    }

    uint64_t m = left.number.size() - rhs.number.size();
    uint64_t shift;
    res.number.resize(m + 1);

    auto it = rhs.number.end(); // changed
    auto end = rhs.number.begin();
    for ( ; it != end; --it) {
        if (*(it - 1) != 0) {
            break;
        }
    }
    if (it == end) {
        throw std::runtime_error("Division by zero");
    }
    --it;
    uint64_t k = *it;
//    print("br1");
    uint64_t sh = 0;
    while (static_cast<int64_t>(k) >= 0) {
        sh++;
        k <<= 1;
    }
    shift = (it - rhs.number.begin()) * 64 + sh;
    rhs <<= shift;
    left <<= shift;
    rhs.number.setSign(true);
    left.number.setSign(true);
    int realSizeRight = rhs.number.getRealSize();
    int realSizeLeft = left.number.getRealSize();

    big_integer highPart = rhs << 64 * (realSizeLeft - realSizeRight);
    if (left >= highPart) {
        res = 1;
        left -= highPart;

    }
    big_integer partLeft, partRight;
    realSizeLeft = left.number.getRealSize();

    partRight.number.resize(2);
    for (auto &digit : partRight.number) {
        digit = 0;
    }
    auto rightIt = rhs.number.begin();
    auto partRightIt = partRight.number.begin();
    for (int i = std::min(2, realSizeRight) - 1; i >= 0; --i) {
//        partRightIt[i] = rightIt[rhs.number.size() - std::min(2, realSizeRight) + i - 1];
        partRightIt[i] = rightIt[realSizeRight - std::min(2, realSizeRight) + i];
    }
//    partLeft.print("pleft");
//    partRight.print("prigh");

    // checked before + debug

    for (int j = realSizeLeft - realSizeRight - 1; j >= 0; j--) {
        partLeft.number.resize(3);

        for (auto &digit : partLeft.number) {
            digit = 0;
        }

//        left.number.print("elft");
        realSizeLeft = left.number.getRealSize();

        auto leftIt = left.number.begin();
        auto partLeftIt = partLeft.number.begin();
        for (int i = std::min(3, realSizeLeft) - 1; i >= 0; --i) {
            partLeftIt[i] = leftIt[realSizeLeft - std::min(3, realSizeLeft) + i];
        }
        partLeft.number.setSign(true);
        partRight.number.setSign(true);

//        partLeft.print("pleft2");
//        partRight.print("prigh2");
        uint64_t l, r, mid;
        l = 0;
        r = UINT64_MAX;

//        partLeft.print("/left");
//        partRight.print("/right");


        big_integer n;
        while (r != l) {
            mid = r - (r - l) / 2;
            n = (partRight);
            n.mulShort(mid);

            if (n < partLeft) {
                l = mid;
            } else if (n > partLeft) {
                r = mid - 1;
            } else {
                l = r = mid;
            }
        }

        uint64_t shift = std::max(static_cast<size_t>(j) * 64, 0UL);
        big_integer copy;
        copy = (rhs);
        copy.mulShort(l);
        copy <<= shift;
//        copy.print("tmp");
//        left.print("tll");
        if (copy > left) {
            copy -= rhs << shift;
//            copy <<= shift;
            l--;
        }
//        copy.print("cpy");
        if (copy <= left) {
            left -= copy;
//            left.print("leftEND");
//            std::cout << to_string(l) << std::endl;
//            std::cout << shift << std::endl;
        } else {
            l = 0;
//            std::cout << "l = 0" << std::endl;
        }

        res <<= 64;
//        res.print("+b");
        res.addShort(l);
//        res.print("+a");

//        if (left < rhs)
//            break;
    }
//    res.print("res_end");
    res.number.setSign(true);
    if (!resultSign) {
        res.invert();
    }
    swap(*this, res);
    return (*this);
}

big_integer& big_integer::operator%=(const big_integer &rhs) {
    big_integer tmp(*this);
    *this -= (tmp / rhs * rhs);
    return *this;
}


big_integer& big_integer::operator&=(big_integer const &rhs) {
    if (number.size() < rhs.number.size()) {
        number.resize(rhs.number.size());
    }
    // size >= rhs.size
    auto rhsDigit = rhs.number.getConstInfinityIterator(0);
    for (auto &digit : this->number) {
        digit &= *rhsDigit;
    }
    this->number.clear();
    return (*this);
}


big_integer& big_integer::operator|=(big_integer const &rhs) {
    if (number.size() < rhs.number.size()) {
        number.resize(rhs.number.size());
    }
    // size >= rhs.size
    auto rhsDigit = rhs.number.getConstInfinityIterator(0);
    for (auto &digit : this->number) {
        digit |= *rhsDigit;
    }
    this->number.clear();
    return (*this);
}


big_integer& big_integer::operator^=(big_integer const &rhs) {
    if (number.size() < rhs.number.size()) {
        number.resize(rhs.number.size());
    }
    // size >= rhs.size
    auto rhsDigit = rhs.number.getConstInfinityIterator(0);
    for (auto &digit : this->number) {
        digit ^= *rhsDigit;
    }
    this->number.clear();
    return (*this);
}


void big_integer::div_rem(uint64_t divider, uint64_t& remainder) {
    uint64_t rem = 0;
    auto end = this->number.rend();
    for (auto i = this->number.rbegin(); i != end; ++i) {
        big_integer::div128(*i, rem, rem, *i, divider);
    }
    remainder = rem;
}

// return: sign(lhs - rhs)
int big_integer::cmp(big_integer const &lhs, big_integer const &rhs) {
    if (lhs.isNegative() && !rhs.isNegative()) {
        return -1;
    } else if (!lhs.isNegative() && rhs.isNegative()) {
        return 1;
    }
    size_t len = std::min(lhs.number.size(), rhs.number.size());

    auto leftBeg = lhs.number.begin();
    auto rightBeg = rhs.number.begin();

//    auto rightBeg = rhs.number.getConstInfinityIterator(max - 1);
    size_t end = std::max(rhs.number.size(), lhs.number.size());
    size_t leftSize = lhs.number.size();
    size_t rightSize = rhs.number.size();
    size_t leftSign = lhs.number.sign();
    size_t rightSign = lhs.number.sign();
    for (size_t i = end - 1; i >= len; i--) {
        uint64_t leftDigit, rightDigit;
        leftDigit = (i >= leftSize) ? leftSign : leftBeg[i];
        rightDigit = (i >= rightSize) ? rightSign : rightBeg[i];

        if (leftDigit != rightDigit) {
            int res = (leftDigit > rightDigit) ? 1 : -1;
            return res;
        }
    }

    for (size_t i = len; i--; ) {
        uint64_t leftDigit, rightDigit;
        leftDigit = leftBeg[i];
        rightDigit = rightBeg[i];

        if (leftDigit != rightDigit) {
            int res = (leftDigit > rightDigit) ? 1 : -1;
            return res;
        }
    }
    return 0;
}

bool operator!=(const big_integer &left, const big_integer &right) {
    return big_integer::cmp(left, right) != 0;
}

bool operator==(const big_integer &left, const big_integer &right) {
    return big_integer::cmp(left, right) == 0;
}

bool operator<(const big_integer &left, const big_integer &right) {
    return big_integer::cmp(left, right) < 0;
}

bool operator>(const big_integer &left, const big_integer &right) {
    return big_integer::cmp(left, right) > 0;
}

bool operator<=(const big_integer &left, const big_integer &right) {
    return big_integer::cmp(left, right) <= 0;
}

bool operator>=(const big_integer &left, const big_integer &right) {
    return big_integer::cmp(left, right) >= 0;
}

big_integer operator~(big_integer left) {
    for (auto &digit : left.number) {
        digit = ~digit;
    }
    return left;
}

big_integer operator&(big_integer left, big_integer const& right) {
    return left &= right;
}

big_integer operator|(big_integer left, big_integer const& right) {
    return left |= right;
}

big_integer operator^(big_integer left, big_integer const& right) {
    return left ^= right;
}

uint64_t shl(uint64_t number, uint64_t shift) {
    return shift == 64 ? 0 : number << shift;
}

uint64_t shr(uint64_t number, uint64_t shift) {
    return shift == 64 ? 0 : number >> shift;
}

big_integer &big_integer::operator<<=(uint64_t shift) {
    if (shift == 0)
        return (*this);
//    print("bef");
    bool sign = isPositive();
    uint64_t full = shift / 64;
    uint64_t red = shift % 64;
    size_t oldSize = number.size();
    number.resize(full + oldSize + 1);


    uint64_t highPart, lowPart;
    highPart = lowPart = 0;
    auto newBeg = number.end();
    --newBeg;
    auto oldBeg = number.begin() + oldSize - 1;// NumberHolder::iterator(number, oldSize - 1);
    highPart = shr(*oldBeg, (64 - red));
    if (isNegative()) {
        highPart |= ~UINT64_C(0) << red;
    }
    *newBeg = highPart;
//    --oldBeg;
    --newBeg;
    for (int i = oldSize - 1; i >= 1; i--) {
        lowPart = *oldBeg << red;
        --oldBeg;
        highPart = shr(*oldBeg, (64 - red));
        lowPart |= highPart;
        *newBeg = lowPart;
        --newBeg;
    }
    *newBeg = *oldBeg << red;
    std::fill(number.begin(), number.begin() + full, UINT64_C(0));  // NumberHolder::iterator(number, full), UINT64_C(0));
    this->number.setSign(sign);
    this->number.clear();
//    print("aft");
    return (*this);
}


big_integer &big_integer::operator>>=(uint64_t shift) {
    bool sign = isPositive();
    uint64_t full = shift / 64;
    uint64_t red = shift % 64;

    if (full > number.size()) {
        big_integer tmp;
        swap(*this, tmp);
        return (*this);
    }
    uint64_t highPart, lowPart;
    highPart = lowPart = 0;
    auto it = number.begin();
    auto end = number.begin() + number.size() - full - 1;   // NumberHolder::iterator(number, number.size() - full - 1);
    for ( ; it != end; ) {
        lowPart = shr(*it, red);
        ++it;
        highPart = shl(*it, 64 - red);
        lowPart |= highPart;
        --it;
        *it = lowPart;
        ++it;
    }
    lowPart = shr(*it, red);
    if (isNegative()) {
        lowPart |= shl(~UINT64_C(0), 64 - red);
    }
    *it = lowPart;
    std::fill(number.begin(), number.begin() + full, UINT64_C(0));//  NumberHolder::iterator(number, full), UINT64_C(0));
    this->number.setSign(sign);
    this->number.clear();
    return (*this);
}

big_integer operator<<(big_integer left, uint64_t shift) {
    return left <<= shift;
}

big_integer operator>>(big_integer left, uint64_t shift) {
    return left >>= shift;
}

void big_integer::print(std::string s) const {
    number.print(s);
}

//
//void big_integer::setSign(bool positive) {
//    if (isNegative() == !positive) {
//        return;
//    }
//    resize(_size + 1);
//    digits[_size - 1] = positive ? big_integer::POSITIVE_SIGN : big_integer::NEGATIVE_SIGN;
//}
