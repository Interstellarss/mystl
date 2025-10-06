#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

// include necessary headers
#include <cstddef>

namespace mystl{

template <class T>
class MyAllocator {
public:
    // Define value_type as T
    using size_type = std::size_t;
    // Define difference_type as a signed integral type
    using difference_type = std::ptrdiff_t;

    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = T&;

    MyAllocator() = default;
    template <class U>
    MyAllocator(const MyAllocator<U>&) noexcept {}

    static pointer allocate(size_type n) {
        return static_cast<pointer>(operator new(n * sizeof(value_type)));
    }

    // Deallocate
    static void deallocate(pointer p, size_type) noexcept{
        ::operator delete(p);
    }

    //construct
    template <class... Args>
    static void construct(pointer p, Args&&... args) {
        ::new (p) value_type(std::forward<Args>(args)...);
    }

    //deconstruct
    static void destroy(pointer p) noexcept {
        p->~T();
    }
};

// Allocators for different types are equal
template <class T, class U>
bool operator==(const MyAllocator<T>&, const MyAllocator<U>& ) noexcept {
    return true;
}

template <class T, class U>
bool operator!=(const MyAllocator<T>&, const MyAllocator<U>& ) noexcept {
    return false;
}

} // namespace mystl


#endif // MY_ALLOCATOR_H