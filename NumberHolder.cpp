//
// Created by nikita on 17.05.19.
//

#include <algorithm>
#include <cstdint>
#include "NumberHolder.h"
#include "big_integer.h"


const uint64_t NumberHolder::NEGATIVE_SIGN = ~UINT64_C(0);
const uint64_t NumberHolder::POSITIVE_SIGN = 0;
const size_t NumberHolder::INITIAL_SIZE = 1;

NumberHolder::NumberHolder() {
    this->_size = this->capacity = 0;
    this->digits = nullptr;
    this->resize(INITIAL_SIZE);
    std::fill_n(_size <= 2 ? small_data : digits, INITIAL_SIZE, UINT64_C(0));
}

NumberHolder::NumberHolder(NumberHolder const &nh) {
    this->capacity = 0;
    this->digits = nullptr;
    if (nh._size <= 2) {
        small_data[0] = nh.small_data[0];
        small_data[1] = nh.small_data[1];
    } else {
        assignBuffer(nh);
    }
    this->_size = nh._size;
}

NumberHolder::NumberHolder(int64_t number) : NumberHolder() {
    *this->begin() = number;
    expandSign(0);
}

NumberHolder::~NumberHolder() {
    if (_size > 2)
        deleteBufferLink();
}

void NumberHolder::deleteBufferLink() {
    if (_size > 2 && digits) {
        --digits[capacity];
        if (digits[capacity] == 0) {
            free(digits);
        }
        this->digits = nullptr;
    }
}

bool NumberHolder::isOnlyBufferUser() {
    if (_size > 2) {
        return digits[capacity] == 1;
    } else {
        return true;
    }
}

void NumberHolder::getNewBuffer() {
    if (_size <= 2) {
        return;
    }
    if (isOnlyBufferUser())
        return;
    uint64_t *tmp = allocate(_size);
    std::copy(this->digits, this->digits + this->_size, tmp);
    deleteBufferLink();
    this->digits = tmp;
    this->capacity = _size;
}

void NumberHolder::assignBuffer(NumberHolder const &nh) {
    if (nh.digits == this->digits) return;
    this->deleteBufferLink();
    this->digits = nh.digits;
    this->capacity = nh.capacity;
    ++(nh.digits[capacity]);
}

// change it
NumberHolder &NumberHolder::operator=(const NumberHolder &right) {
    if (this == &right)
        return *this;

    if (right._size <= 2 && this->_size <= 2) {

        this->small_data[0] = right.small_data[0];
        this->small_data[1] = right.small_data[1];
        this->_size = right._size;
        return (*this);
    }
    if (right._size <= 2 && this->_size > 2) {
        this->getNewBuffer();
        size_t i = 0;
        for (; i < right.size(); i++) {
            this->digits[i] = right.small_data[i];
        }
        for (; i < 3; i++) {
            this->digits[i] = right.sign();
        }
        this->_size = 3;
        return *this;
    }
    this->assignBuffer(right);
    this->_size = right._size;
    return *this;
}


void NumberHolder::resize(size_t new_size) {
    if (_size >= new_size) {
        return;
    }

    // size < new_size <= 2
    if (new_size <= 2) {
        if (_size == 1)
            small_data[1] = sign();
        _size = new_size;

        return;
    }
    // new_size > 2, big object
    if (_size <= 2) {
        uint64_t sign = this->sign();
        uint64_t tmp[2];
        tmp[0] = this->small_data[0];
        if (_size == 2)
            tmp[1] = this->small_data[1];
        else
            tmp[1] = this->sign();
        this->digits = allocate(new_size);
        digits[0] = tmp[0];
        digits[1] = tmp[1];
        std::fill(digits + 2, digits + new_size, sign);
        this->_size = this->capacity = new_size;

        return;
    }

    if (isOnlyBufferUser() && capacity > new_size) {
        std::fill(digits + this->_size, digits + new_size, sign());
        this->_size = new_size;
        return;
    }


    // not only user && expand
    uint64_t *tmp = allocate(new_size);
    std::copy(this->digits, this->digits + this->_size, tmp);
    std::fill(tmp + this->_size, tmp + new_size, sign());
    deleteBufferLink();
    this->capacity = new_size;
    this->digits = tmp;
    this->_size = new_size;

}

void NumberHolder::expandSign(size_t highestBit) {
    uint64_t sign = this->sign();
    std::fill(this->begin() + highestBit + 1, this->end(), sign);
}


void NumberHolder::clear() {
    if (_size <= 2) return;
    size_t begSize = _size;
    // big object
    size_t signBit = this->sign();
    for (int i = this->_size - 1; i >= 3 && this->digits[i] == signBit && _size > 2; i--) {
        _size--;
    }
    if (begSize != _size)
        _size++;
}

uint64_t *NumberHolder::allocate(size_t nElements) {
    size_t size = (nElements + 1) * sizeof(uint64_t);
    uint64_t *buffer = static_cast<uint64_t *>(malloc(size));
    *(buffer + nElements) = 1;
    return buffer;
}

size_t NumberHolder::size() const {
    return _size;
}

uint64_t NumberHolder::sign() const {
    if (_size == 0) return POSITIVE_SIGN;
    uint64_t highDigit = _size <= 2 ? small_data[_size - 1] : digits[_size - 1];
    return (highDigit & (UINT64_C(1) << (sizeof(highDigit) * 8 - 1))) == 0 ?
           NumberHolder::POSITIVE_SIGN :
           NumberHolder::NEGATIVE_SIGN;
}

void NumberHolder::setSign(bool positive) {
    if ((sign() == NumberHolder::POSITIVE_SIGN) == positive) {
        return;
    }
    resize(_size + 1);
    // size >= 2
    uint64_t &highPart = (_size == 2) ? small_data[1] : digits[_size - 1];
    highPart = positive ? NumberHolder::POSITIVE_SIGN : NumberHolder::NEGATIVE_SIGN;
}

NumberHolder::iterator NumberHolder::begin() {
    getNewBuffer();
    return _size <= 2 ? small_data : digits;
}


NumberHolder::iterator NumberHolder::end() {
    getNewBuffer();
    return (_size <= 2 ? small_data : digits) + this->_size;
}


NumberHolder::const_iterator NumberHolder::begin() const {
    return (_size <= 2 ? small_data : digits);
}


NumberHolder::const_iterator NumberHolder::end() const {
    return (_size <= 2 ? small_data : digits) + this->_size;
}


NumberHolder::reverse_iterator NumberHolder::rbegin() {
    return NumberHolder::reverse_iterator(this->end());
}

NumberHolder::reverse_iterator NumberHolder::rend() {
    return NumberHolder::reverse_iterator(this->begin());
}

ConstInfinityNumberHolderIterator NumberHolder::getConstInfinityIterator(size_t begin_pos) const {
    return ConstInfinityNumberHolderIterator(*this, begin_pos);
}

void NumberHolder::print(std::string s) const {
    printf("%s --> ", s.c_str());
    auto it = getConstInfinityIterator(_size - 1);
    for (size_t i = 0; i < _size; ++i) {
        printf("0x%016lx ", *it);
        --it;
    }
    std::cout << std::endl;
}

size_t NumberHolder::getRealSize() const {
    if (_size <= 2) {
        if (_size == 1)
            return 1;
        if (sign() == small_data[1])
            return 1;
        return 2;
    }
    // big object
    size_t realSizeRight = _size - 1;
    while (realSizeRight > 0 && digits[realSizeRight] == sign())
        realSizeRight--;
    realSizeRight++;
    return realSizeRight;
}

bool operator==(ConstInfinityNumberHolderIterator const &left,
                ConstInfinityNumberHolderIterator const &right) {
    return left.i == right.i;
}

bool operator!=(ConstInfinityNumberHolderIterator const &left,
                ConstInfinityNumberHolderIterator const &right) {
    return !(left == right);
}

ConstInfinityNumberHolderIterator const &operator++(ConstInfinityNumberHolderIterator &it) {
    ++it.i;
    return it;
}

ConstInfinityNumberHolderIterator const operator++(ConstInfinityNumberHolderIterator const &it, int a) {
    ConstInfinityNumberHolderIterator tmp(it);
    return ++tmp;
}


ConstInfinityNumberHolderIterator const &operator--(ConstInfinityNumberHolderIterator &it) {
    --it.i;
    return it;
}

ConstInfinityNumberHolderIterator const operator--(ConstInfinityNumberHolderIterator const &it, int a) {
    ConstInfinityNumberHolderIterator tmp(it);
    return --tmp;
}

ConstInfinityNumberHolderIterator::ConstInfinityNumberHolderIterator(NumberHolder const &holder, size_t pos) :
        holder(holder),
        i(pos),
        begin(holder._size <= 2 ? holder.small_data : holder.digits) {}


uint64_t ConstInfinityNumberHolderIterator::operator*() const {
    return i < holder._size ? begin[i] : holder.sign();
}