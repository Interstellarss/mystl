#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

#include <cstddef>

namespace mystl {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// Base iterator template
template <typename Category, typename T, typename Distance = ptrdiff_t,
          typename Pointer = T*, typename Reference = T&>
struct iterator {
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

// Iterator traits  template
template <typename Iterator>
struct iterator_traits {
    using iterator_category = typename Iterator::iterator_category;
    using value_type = typename Iterator::value_type;
    using difference_type = typename Iterator::difference_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
};

// Specialization for pointer types
template <typename T>
struct iterator_traits<T*> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;
};

// Specialization for const pointer types
template <typename T>
struct iterator_traits<const T*> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
};


// calculate the distance between two iterators
template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    using category = typename iterator_traits<InputIterator>::iterator_category;
    return distance_impl(first, last, category());
}

// position of the iterator
template <typename InputIterator, typename Distance>
void advance(InputIterator& it, Distance n, input_iterator_tag) {
    while (n--) ++it;
}


template <typename BidirectionalIterator, typename Distance>
void advance(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag) {
    if (n >= 0) {
        while (n--) ++it;
    } else {
        while (n++) --it;
    }
}

template <typename RandomAccessIterator, typename Distance>
void advance(RandomAccessIterator& it, Distance n, random_access_iterator_tag) {
    it += n;
}

template <typename Iterator, typename Distance>
void advance(Iterator& it, Distance n) {
    using category = typename iterator_traits<Iterator>::iterator_category;
    advance(it, n, category());
}

// =================================================
// reverse iterator (iterator adapter)
// =================================================

template <typename Iterator>
class reverse_iterator {
protected:
    Iterator current; // the underlying iterator
public:
    using iterator_type = Iterator;
    using iterator_category = typename iterator_traits<Iterator>::iterator_category;
    using value_type = typename iterator_traits<Iterator>::value_type;
    using difference_type = typename iterator_traits<Iterator>::difference_type;
    using pointer = typename iterator_traits<Iterator>::pointer;
    using reference = typename iterator_traits<Iterator>::reference;

    reverse_iterator() : current() {}
    explicit reverse_iterator(Iterator it) : current(it) {}
    reverse_iterator(const reverse_iterator& other) : current(other.current) {}

    Iterator base() const {
        return current;
    }

    reference operator*() const {
        Iterator temp = current;
        return *--temp;
    }

    pointer operator->() const {
        return &(operator*());
    }

    reverse_iterator& operator++() {
        --current;
        return *this;
    }

    reverse_iterator operator++(int) {
        reverse_iterator temp = *this;
        --current;
        return temp;
    }

    reverse_iterator& operator--() {
        ++current;
        return *this;
    }

    reverse_iterator operator--(int) {
        reverse_iterator temp = *this;
        ++current;
        return temp;
    }

    reverse_iterator operator+(difference_type n) const {
        return reverse_iterator(current - n);
    }
    reverse_iterator& operator+=(difference_type n) {
        current -= n;
        return *this;
    }
    reverse_iterator operator-(difference_type n) const {
        return reverse_iterator(current + n);
    }
    reverse_iterator& operator-=(difference_type n) {
        current += n;
        return *this;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const reverse_iterator& other) const {
        return current == other.current;
    }
    bool operator!=(const reverse_iterator& other) const {
        return current != other.current;
    }
    bool operator<(const reverse_iterator& other) const {
        return current > other.current;
    }
};


} // namespace mystl

#endif // MY_ITERATOR_H