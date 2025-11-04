#ifndef REF_BLOCK_BASE_H
#define REF_BLOCK_BASE_H

#include <atomic>
#include "my_utility.h"
#include <memory>
#include <functional>
#include <cstddef>      // for std::size_t, std::alignas

// base control class (with atomic operation and type erasure)

namespace mystl{
struct RefBlockBase {
    // atomic operation
    std::atomic<size_t> m_shared_count{1};   // when init, create the shared_ptr and own it
    std::atomic<size_t> m_weak_count{1};    // when init, m_shared_count  as one "weak reference"

    virtual ~RefBlockBase() = default;

    // type erasure
    // "dispose" = destroy resource
    virtual void dispose_resource() = 0;
    // "destroy" = destroy control block itself (like, delete this)
    virtual void destroy_self() = 0;
    // get T's pointer
    virtual void* get_resource_ptr() { return nullptr; }


    // ---- thread safe counter operation ----
    void increment_shared() noexcept{
        m_shared_count.fetch_add(1, std::memory_order_relaxed);
    }

    void increment_weak() noexcept{
        m_weak_count.fetch_add(1, std::memory_order_relaxed);
    }

    void decrement_shared() noexcept {
        // fetch_sub return the value before minus
        // use  acq_rel (Acquire-Release) ensure memory safe
        if(m_shared_count.fetch_sub(1, std::memory_order_acq_rel) == 1){
            // if shared counter goes 0, destroy resource
            dispose_resource();
            // when a shared ptr destroyed, the weak ref it has is also gond
            decrement_weak();
        }
    }

    void decrement_weak() noexcept{
        if (m_weak_count.fetch_sub(1, std::memory_order_acq_rel) == 1){
            // weak counter oges 0, destroy control block
            destroy_self();
        }
    }

    // try lock if shared_coutn > 0, so it atomically adds by 1, and return true
    bool try_increment_shared() noexcept {
        size_t count = m_shared_count.load(std::memory_order_relaxed);

        while(count != 0){
            // try to replace count with count + 1
            if(m_shared_count.compare_exchange_weak(count, count + 1, std::memory_order_acq_rel)){
                return true; // success
            }

            // if failes it will continue the loop
        }
        return false;
    }
};

// for 'new'
// Y is the actual type, D is the del type
template <typename Y, typename D>
struct RefBlockImpl : public RefBlockBase {
    Y* m_resource;
    D m_deleter;

    RefBlockImpl(Y* res, D del)
        : m_resource(res), m_deleter(mystl::move(del)){}

    void dispose_resource() override {
        // call the deleter
        m_deleter(m_resource);
    }

    void destroy_self() override {
        // destroy self
        delete this;
    }
};

template <typename T>
struct RefBlockMakeShared : public RefBlockBase{
    // T's data will followed directly after this struct
    // use an aligned char array for padding
    alignas(T) char m_storage[sizeof(T)];

    // get T's pointer
    void* get_resource_ptr() override{
        return reinterpret_cast<T*>(m_storage);
    }

    void dispose_resource() override {
        // call the deconstruct but not release the mmroy
        reinterpret_cast<T*>(m_storage) -> ~T();
    }

    void destroy_self() override{
        // delete self, this will release the memory in make_shared
        delete this;
    }
};

} // namespace mystl
#endif REF_BLOCK_BASE_H

