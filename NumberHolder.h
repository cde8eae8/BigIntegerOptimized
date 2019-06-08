//
// Created by nikita on 17.05.19.
//

#ifndef BIGINTEGEROPTIMIZED_NUMBERHOLDER_H
#define BIGINTEGEROPTIMIZED_NUMBERHOLDER_H


#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>

struct NumberHolderIterator;
struct ConstInfinityNumberHolderIterator;
struct ConstNumberHolderIterator;

struct NumberHolder {
    typedef uint64_t *iterator;
    typedef uint64_t const * const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef uint64_t value_type;
    typedef uint64_t * pointer;
    typedef uint64_t & reference;

    uint64_t sign() const;
    NumberHolder();
    NumberHolder(NumberHolder const& nh);
    NumberHolder(int64_t nh);
    ~NumberHolder();
    NumberHolder&operator=(const NumberHolder& right);

    void assignBuffer(NumberHolder const &nh);
    void deleteBufferLink();
    bool isOnlyBufferUser();
    void getNewBuffer();

    friend void swap(NumberHolder& lhs, NumberHolder& rhs);

    void print(std::string s="") const;

    void clear();

    size_t size() const;
    void resize(size_t size);
    void setSign(bool positive);
    size_t getRealSize() const;

    NumberHolder::iterator begin();
    NumberHolder::iterator end();

    NumberHolder::const_iterator begin() const;
    NumberHolder::const_iterator end() const;
    ConstInfinityNumberHolderIterator getConstInfinityIterator(size_t begin_pos) const;
    NumberHolder::reverse_iterator rbegin();

    NumberHolder::reverse_iterator rend();

    friend class NumberHolderIterator;
    friend class ConstInfinityNumberHolderIterator;
    friend class ConstNumberHolderIterator;

// private:
    void expandSign(size_t highestBit);
    uint64_t *allocate(size_t nElements);

    static const uint64_t NEGATIVE_SIGN;
    static const uint64_t POSITIVE_SIGN;
    static const size_t INITIAL_SIZE;
    union {
        struct {
            uint64_t *digits;
            size_t capacity;
        };
        uint64_t small_data[2];
    };
    size_t _size;
//    bool isSmall;
};

void swap(NumberHolder& lhs, NumberHolder& rhs);


//
//struct NumberHolderIterator {
//    typedef ptrdiff_t difference_type;
//    typedef size_t size_type;
//    typedef uint64_t value_type;
//    typedef uint64_t * pointer;
//    typedef uint64_t & reference;
//
//    NumberHolderIterator(NumberHolder &holder, size_t pos);
//    friend bool operator==(NumberHolderIterator const &left, NumberHolderIterator const &right);
//    friend bool operator!=(NumberHolderIterator const &left, NumberHolderIterator const &right);
//    friend NumberHolderIterator const & operator++(NumberHolderIterator & it);
//    friend NumberHolderIterator const operator++(NumberHolderIterator const& it, int a);
//    friend NumberHolderIterator const & operator--(NumberHolderIterator & it);
//    friend NumberHolderIterator const operator--(NumberHolderIterator const& it, int i);
//    friend NumberHolderIterator const operator+(NumberHolderIterator const& it, size_t sh);
//    friend ptrdiff_t const operator-(NumberHolderIterator const &lhs, NumberHolderIterator const& rhs);
//    friend NumberHolderIterator const operator-(NumberHolderIterator const &lhs, size_t const& rhs);
//    uint64_t & operator[](size_t sh);
//
//    uint64_t& operator*() const;
// private:
//    NumberHolder& holder;
//    size_t i;
//    uint64_t *begin;
//};
//
//
//struct ConstNumberHolderIterator {
//    typedef ptrdiff_t difference_type;
//    typedef size_t size_type;
//    typedef uint64_t value_type;
//    typedef uint64_t * pointer;
//    typedef uint64_t & reference;
//
//    ConstNumberHolderIterator(const NumberHolder &holder, size_t pos);
//    friend bool operator==(ConstNumberHolderIterator const &left, ConstNumberHolderIterator const &right);
//    friend bool operator!=(ConstNumberHolderIterator const &left, ConstNumberHolderIterator const &right);
//    friend ConstNumberHolderIterator const & operator++(ConstNumberHolderIterator & it);
//    friend ConstNumberHolderIterator const operator++(ConstNumberHolderIterator const& it, int a);
//    friend ConstNumberHolderIterator const & operator--(ConstNumberHolderIterator & it);
//    friend ConstNumberHolderIterator const operator--(ConstNumberHolderIterator const& it, int i);
//    const uint64_t & operator*() const;
// private:
//    NumberHolder const & holder;
//    size_t i;
//    uint64_t const *const begin;
//};
//
//
//template <>
//struct std::iterator_traits<NumberHolderIterator> {
//    typedef ptrdiff_t difference_type; //almost always ptrdiff_t
//    typedef uint64_t value_type; //almost always T
//    typedef uint64_t & reference; //almost always T& or const T&
//    typedef uint64_t * pointer; //almost always T* or const T*
//    typedef std::bidirectional_iterator_tag iterator_category;  //usually std::forward_iterator_tag or similar
//};
//
//template <>
//struct std::iterator_traits<ConstNumberHolderIterator> {
//    typedef ptrdiff_t difference_type; //almost always ptrdiff_t
//    typedef uint64_t value_type; //almost always T
//    typedef uint64_t & reference; //almost always T& or const T&
//    typedef uint64_t * pointer; //almost always T* or const T*
//    typedef std::bidirectional_iterator_tag iterator_category;  //usually std::forward_iterator_tag or similar
//};
//



struct ConstInfinityNumberHolderIterator {
    ConstInfinityNumberHolderIterator(NumberHolder const &holder, size_t pos);
    friend bool operator==(ConstInfinityNumberHolderIterator const &left, ConstInfinityNumberHolderIterator const &right);
    friend bool operator!=(ConstInfinityNumberHolderIterator const &left, ConstInfinityNumberHolderIterator const &right);
    friend ConstInfinityNumberHolderIterator const & operator++(ConstInfinityNumberHolderIterator & it);
//    friend ConstInfinityNumberHolderIterator const operator++(ConstInfinityNumberHolderIterator const& it, int a);
    friend ConstInfinityNumberHolderIterator const & operator--(ConstInfinityNumberHolderIterator & it);
//    friend ConstInfinityNumberHolderIterator const operator--(ConstInfinityNumberHolderIterator const& it, int i);
    uint64_t operator*() const;
 private:
    NumberHolder const& holder;
    size_t i;
    uint64_t const *const begin;
};

bool operator==(ConstInfinityNumberHolderIterator const &left,
                ConstInfinityNumberHolderIterator const &right);

bool operator!=(ConstInfinityNumberHolderIterator const &left,
                ConstInfinityNumberHolderIterator const &right);
ConstInfinityNumberHolderIterator const & operator++(ConstInfinityNumberHolderIterator & it);
ConstInfinityNumberHolderIterator const operator++(ConstInfinityNumberHolderIterator const& it, int a);
ConstInfinityNumberHolderIterator const & operator--(ConstInfinityNumberHolderIterator & it);
ConstInfinityNumberHolderIterator const operator--(ConstInfinityNumberHolderIterator const& it, int i);
#endif //BIGINTEGEROPTIMIZED_NUMBERHOLDER_H
