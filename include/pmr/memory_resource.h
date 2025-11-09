#ifndef MY_MEMORY_RESOURCE_H
#define MY_MEMORY_RESOURCE_H

#include <cstddef>  // for std::size_t, std::byte, std::max_align_t
#include <stdexcept> // for std::bad_alloc

namespace my_pmr{

/**
 * Base clss for memory resource (follow the std::pmr::memory_resource)
 * This is the interface for all other memory pool here
 */
class MemoryResource{
public:
    using size = std::size_t;
    using max_align = std::max_align_t;

    virtual ~MemoryResource() = default;

    /**
     * @ brief the interface to requesting memory
     */
    void* allocate(size bytes, size alignment = alignof(max_align)){
        if (bytes == 0){
            return nullptr;
        }

        //cal the child impl
        return do_allocate(bytes, alignment);
    }

    /** 
     * 
    */
    void deallocate(void* p, size bytes, size alignment = alignof(max_align)){
        if(p == nullptr || bytes == 0){
            return;
        }

        do_deallocate(p, bytes, alignment);
    }

    bool is_equal(const MemoryResource& other) const noexcept{
        return do_is_equal(other);
    }

protected:
    //the virtual functions that child class must impl
    virtual void* do_allocate(size bytes, size alignment) = 0;
    virtual void do_deallocate(void* p, size bytes, size alignment) noexcept = 0;
    virtual bool do_is_equal(const MemoryResource& other) const noexcept = 0;
};


class NewDeleteResource : public MemoryResource{
protected:
    void* do_allocate(size byytes, size alignment) override{
        // in c++ 17 there is new with alignment, here just ignore
        (void)alignment;
        return ::operator new(byytes);
    }

    void do_deallocate(void* p, size bytes, size alignment) noexcept override{
        (void)bytes; (void)alignment;
        ::operator delete(p);
    }

    bool do_is_equal(const MemoryResource& other) const noexcept override{
        return dynamic_cast<const NewDeleteResource*>(&other) != nullptr;
    } 
};

// something like the std::pmr::get_default_resource()
inline MemoryResource* new_delete_resource() noexcept{
    static NewDeleteResource instance;
    return &instance;
}

// get current default resource, like the std::pmr::get_default_resource()
inline MemoryResource* get_default_resource() noexcept {
    static MemoryResource* default_res = new_delete_resource();
    return default_res;
}



} // namespace my_pmr
#endif MY_MEMORY_RESOURCE_H