#ifndef MY_STACK_H
#define MY_STACK_H

#include "my_deque.h"
#include "my_utility.h"

namespace mystl{

template <typename T, class Container = mystl::MyDeque<T>>
class MyStack{
public:
    using value_type = T;
    using container_type = Container;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    container_type c;

public:
    MyStack() = default;
    explicit MyStack(const container_type& cont) : c(cont){}
    explicit MyStack(container_type&& cont) : c(mystl::move(cont)) {} 

    bool empty() const noexcept {return c.empty();}
    size_type size() {return c.size();}

    reference top() {return c.back();}
    const_reference top() const {return c.back();}

    void push(const value_type& value){c.push_back(value);}
    void push(value_type&& value){c.push_back(mystl::move(value));}

    void pop(){c.pop_back();}

    void swap(MyStack& other) noexcept{
        mystl::swap(c, other.c);
    }

    friend bool operator==(const MyStack& lhs, const MyStack& rhs){
        return lhs.c == rhs.c;
    }

    friend bool operator!=(const MyStack& lhs, const MyStack& rhs){
        return !(lhs == rhs);
    }

};
}// namesapce mystl
#endif // MY_STACK_H