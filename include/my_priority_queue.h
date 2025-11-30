#ifndef MY_MYPRIORITYQUEUE_H
#define MY_MYPRIORITYQUEUE_H

#include "my_binary_heep.h"

namespace mystl {

template <typename T,
          typename Container = mystl::MyVector<T>,
          typename Compare = std::less<T>>
class MyPriorityQueue{
public:
    using valu_type = T;
    using size_type = typename Container::size_type;
    using reference = valu_type&;
    using const_reference = const valu_type&;

private:
    mystl::MyBinaryHeap<T, Container, Compare>heap_;

public:
    MyPriorityQueue() = default;
    explicit MyPriorityQueue(const Compare& comp) : heap_(comp) {}

    bool empty() const { return heap_.empty();}

    size_type size() const {return heap_.size();}

    const_reference top() const {return heap_.top();}

    void push(const_reference value) {heap_.push(value);}

    void pop() { heap_.pop();}

};
}
#endif //MY_MYPRIORITYQUEUE_H