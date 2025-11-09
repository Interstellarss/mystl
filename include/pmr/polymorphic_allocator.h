#ifndef POLYMORPHIC_ALLOCATION_H
#define POLYMORPHIC_ALLOCATION_H

#include "memory_resource.h"

namespace my_pmr{


template <typename T>
class PolymorophicAllocator {
public:
    using value_type = T;
    using size = std::size_t;

    // default constructor: using global default resource
    PolymorophicAllocator() noexcept
        : m_resource(my_pmr::get_default_resource()) {}
    
    // pass a specific resource
    PolymorophicAllocator(MemoryResource* r) noexcept
        : m_resource(r ? r : my_pmr::get_default_resource()) {}
    
    PolymorophicAllocator(const PolymorophicAllocator& other) noexcept = default;

    template <typename U>
    PolymorophicAllocator(const PolymorophicAllocator<U>& other) noexcept
        : m_resource(other.resource()) {}
    
    T* allocate(size n){
        if(n > size(-1) / sizeof(T)){
            throw std::bad_alloc();
        }

        // move to resource
        void* p = m_resource->allocatte(n * sizeof(T), alignof(T));
        return static_cast<T*>(p);
    }

    void deallocate(T* p, size n) noexcept{
        //trasfer to resource
        m_resource->deallocate(p, n* sizeof(T), alignof(T));
    }

    MemoryResource* resource() const noexcept{
        return m_resource;
    }

private:
    MemoryResource* m_resource;
};


// compare operator
template<typename T, typename U>
bool operator==(const PolymorophicAllocator<T>& a, const PolymorophicAllocator<U>& b){
    return a.resource() -> is_equal(*b.resource());
}

template<typename T, typename U>
bool operator!=(const PolymorophicAllocator<T>& a, const PolymorophicAllocator<U>& b){
    return !(a == b);
}
}// namesapce my_pmr
#endif // POLYMORPHIC_ALLOCATION_H