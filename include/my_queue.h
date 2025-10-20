#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include "my_deque.h"
#include "my_utility.h"
#include "my_algorithm.h"

namespace mystl{

template <typename T, class Container = mystl::MyDeque<T>>
class MyQueue{
public:
    using value_type = T;
    using container_type = Container;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    container_type c; //underlying container;

public:
    MyQueue() = default;
    explicit MyQueue(const container_type& cont) : c(cont){}
    explicit MyQueue(container_type&& count) : c(mystl::move(count)) {}

    bool empty() const noexcept {return c.empty();}
    size_type size() const noexcept {return c.size();}

    reference front() {return c.front();}
    const_reference front() const {return c.front();}

    reference back() {return c.back();}
    const_reference back() const {return c.back();}

    void push(const value_type& value){c.push_back(value);}
    void push(value_type&& value) {c.push_back(mystl::move(value));}

    void pop(){c.pop_front();}
    void swap(MyQueue& other) noexcept{
        mystl::swap(c, other.c);
    }

    friend bool operator==(const MyQueue& lhs, const MyQueue& rhs){
        return lhs.c == rhs.c;
    }

    friend bool operator!=(const MyQueue& lhs, const MyQueue& rhs){
        return !(lhs == rhs);
    }

};

} // namespace mystl

#endif // MY_QUEUE_H