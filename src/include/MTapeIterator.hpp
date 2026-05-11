#ifndef TAPE_ITERATOR_HPP_
#define TAPE_ITERATOR_HPP_

#include <cstddef>
#include <iterator>

class MTape;

class MTapeIteratorProxy {
public:
    MTapeIteratorProxy(std::size_t index, MTape* tape);

    MTapeIteratorProxy(const MTapeIteratorProxy& other);
    
    MTapeIteratorProxy& operator=(const MTapeIteratorProxy& other);

    MTapeIteratorProxy& operator=(int value);

    operator int() const;

    friend void swap(MTapeIteratorProxy lhs, MTapeIteratorProxy rhs);

private:
    std::size_t index_;
    MTape* tape_;
};

class MTapeIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using iterator_concept = std::random_access_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = MTapeIteratorProxy;

    MTapeIterator(MTape* tape);

    MTapeIterator(std::size_t index, MTape* tape);

    MTapeIterator(const MTapeIterator& other);

    MTapeIterator& operator=(const MTapeIterator& other);

    reference operator*() const;

    MTapeIterator& operator++();

    MTapeIterator operator++(int);

    MTapeIterator& operator--();

    MTapeIterator operator--(int);
    
    MTapeIterator& operator+=(difference_type offset);

    MTapeIterator operator+(difference_type offset) const;

    MTapeIterator& operator-=(difference_type offset);

    MTapeIterator operator-(difference_type offset) const;

    difference_type operator-(const MTapeIterator& other) const;

    reference operator[](difference_type offset) const;

    bool operator==(const MTapeIterator& other) const;
    bool operator!=(const MTapeIterator& other) const;
    bool operator<(const MTapeIterator& other) const;
    bool operator>(const MTapeIterator& other) const;
    bool operator<=(const MTapeIterator& other) const;
    bool operator>=(const MTapeIterator& other) const;

private:
    std::size_t index_;
    MTape* tape_;
};

MTapeIterator operator+(
    MTapeIterator::difference_type offset,
    const MTapeIterator& iterator
);

#endif // TAPE_ITERATOR_HPP_
