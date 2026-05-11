#include "include/MTape.hpp"

MTapeIteratorProxy::MTapeIteratorProxy(std::size_t index, MTape* tape)
    : index_{index}
    , tape_{tape}
{}

MTapeIteratorProxy::MTapeIteratorProxy(const MTapeIteratorProxy& other)
    : index_{other.index_}
    , tape_{other.tape_}
{}

MTapeIteratorProxy& MTapeIteratorProxy::operator=(const MTapeIteratorProxy& other) {
    return *this = static_cast<int>(other);
}

MTapeIteratorProxy& MTapeIteratorProxy::operator=(int value) {
    tape_->WriteCell(index_, value);
    
    return *this;
}

MTapeIteratorProxy::operator int() const {
    return tape_->ReadCell(index_);
}

void swap(MTapeIteratorProxy lhs, MTapeIteratorProxy rhs) {
    const int temp = static_cast<int>(lhs);
    lhs = static_cast<int>(rhs);
    rhs = temp;
}

MTapeIterator::MTapeIterator(MTape* tape)
    : index_{0}
    , tape_{tape}
{}

MTapeIterator::MTapeIterator(std::size_t index, MTape* tape)
    : index_{index}
    , tape_{tape}
{}

MTapeIterator::MTapeIterator(const MTapeIterator& other)
    : index_{other.index_}
    , tape_{other.tape_}
{}

MTapeIterator& MTapeIterator::operator=(const MTapeIterator& other) {
    index_ = other.index_;
    tape_ = other.tape_;
    
    return *this;
}

MTapeIterator::reference MTapeIterator::operator*() const {
    return MTapeIteratorProxy{index_, tape_};
}

MTapeIterator& MTapeIterator::operator++() {
    ++index_;

    return *this;
}

MTapeIterator MTapeIterator::operator++(int) {
    MTapeIterator temp{*this};

    ++index_;
    
    return temp;
}

MTapeIterator& MTapeIterator::operator--() {
    --index_;
    return *this;
}

MTapeIterator MTapeIterator::operator--(int) {
    MTapeIterator temp{*this};

    --index_;
    
    return temp;
}

MTapeIterator& MTapeIterator::operator+=(MTapeIterator::difference_type offset) {
    index_ = static_cast<std::size_t>(
        static_cast<difference_type>(index_) + offset
    );

    return *this;
}

MTapeIterator MTapeIterator::operator+(MTapeIterator::difference_type offset) const {
    MTapeIterator temp{*this};
    temp += offset;
    return temp;
}

MTapeIterator& MTapeIterator::operator-=(MTapeIterator::difference_type offset) {
    return *this += -offset;
}

MTapeIterator MTapeIterator::operator-(MTapeIterator::difference_type offset) const {
    MTapeIterator temp{*this};
    temp -= offset;
    return temp;
}

MTapeIterator::difference_type MTapeIterator::operator-(const MTapeIterator& other) const {
    return static_cast<difference_type>(index_)
        - static_cast<difference_type>(other.index_);
}

MTapeIterator::reference MTapeIterator::operator[](
    MTapeIterator::difference_type offset
) const {
    return *(*this + offset);
}

bool MTapeIterator::operator==(const MTapeIterator& other) const {
    return index_ == other.index_;
}

bool MTapeIterator::operator!=(const MTapeIterator& other) const {
    return !(*this == other);
}

bool MTapeIterator::operator<(const MTapeIterator& other) const {
    return index_ < other.index_;
}

bool MTapeIterator::operator>(const MTapeIterator& other) const {
    return other < *this;
}

bool MTapeIterator::operator<=(const MTapeIterator& other) const {
    return !(other < *this);
}

bool MTapeIterator::operator>=(const MTapeIterator& other) const {
    return !(*this < other);
}

MTapeIterator operator+(
    MTapeIterator::difference_type offset,
    const MTapeIterator& iterator
) {
    return iterator + offset;
}
