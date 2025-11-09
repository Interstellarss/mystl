#ifndef MY_PTR_H
#define MY_PTR_H

#include "ref_block_base.h"
#include "my_utility.h"
#include <type_traits>

namespace mystl {

// forwared declaration
template <typename T>
class MySharedPtr;

template <typename T>
class MyWeakPtr;

template <typename T, typename... Args>
MySharedPtr<T> MyMakeShared(Args&&... args);

template <typename T>
class MySharedPtr {
    // allow myweaktptr to access prive
    friend class MyWeakPtr<T>;
    template<typename U, typename... Args>
    friend MySharedPtr<U> MyMakeShared(Args&&... args);

private:
    T* m_ptr = nullptr;

    RefBlockBase* m_block = nullptr;

    //private construck for myweakptr lock  and MyMakeShared
    MySharedPtr(T* ptr, RefBlockBase* block) noexcept
        : m_ptr(ptr), m_block(block) {}

public:
    // --- construct func ---

    // default
    MySharedPtr() noexcept = default;

    // accept rao ptr
    explicit MySharedPtr(T* ptr)
        : MySharedPtr(ptr, std::default_delete<T>()){

    }

    // construct from nullptr
    MySharedPtr(std::nullptr_t) noexcept
        : MySharedPtr(){}

    // core construct with deleter
    template <typename Y, typename D,
              typename = typename std::enable_if<
                  !std::is_convertible<typename std::decay<D>::type, RefBlockBase*>::value>::type>
    MySharedPtr(Y* ptr, D&& deleter)
        : m_ptr(ptr){
        using DeleterType = typename std::decay<D>::type;
        DeleterType deleter_copy = mystl::forward<D>(deleter);
        try{
            // try to allocate for control block
            m_block = new RefBlockImpl<Y, DeleterType>(ptr, deleter_copy);
        }catch(...){
            // if new refblockimpl fails, delete ptr and through error
            deleter_copy(ptr);
            throw;
        }
    }

    // move constructor
    MySharedPtr(MySharedPtr&& other) noexcept
        : m_ptr(other.m_ptr), m_block(other.m_block) {
        other.m_ptr = nullptr;
        other.m_block = nullptr;
    }

    // --- deconstructor ---
    ~MySharedPtr() noexcept {
        if (m_block){
            m_block->decrement_shared();
        }
    }

    // --- copy control ---
    MySharedPtr(const MySharedPtr& other)
        : m_ptr(other.m_ptr), m_block(other.m_block){
        if(m_block){
            m_block -> increment_shared();
        }
    }

    MySharedPtr& operator=(const MySharedPtr& other){
        if(this != &other){
            // 1. release old resource
            if(m_block){
                m_block -> decrement_shared();
            }

            // 2. copy new resource
            m_ptr = other.m_ptr;
            m_block = other.m_block;

            if(m_block){
                m_block->increment_shared();
            }
        }
        return *this;
    }

    // move assignment operator
    MySharedPtr& operator=(MySharedPtr&& other) noexcept {
        if (this != &other) {
            // 1. Release current resources
            if (m_block) {
                m_block->decrement_shared();
            }

            // 2. Take ownership from other
            m_ptr = other.m_ptr;
            m_block = other.m_block;

            // 3. Nullify other's resources
            other.m_ptr = nullptr;
            other.m_block = nullptr;
        }
        return *this;
    }

    MySharedPtr& operator=(std::nullptr_t){
        if(m_block){
            m_block->decrement_shared();
            m_block = nullptr;
            m_ptr = nullptr;
        }
        return *this;
    }

    // --- accesser ---
    T* get() const noexcept {return m_ptr;}

    T& operator*() const noexcept {return *m_ptr;}
    T* operator->() const noexcept {return m_ptr;}

    size_t use_count() const noexcept{
        return m_block ? m_block->m_shared_count.load() : 0;
    }

    explicit operator bool() const noexcept {return m_ptr != nullptr;}

};


template <typename T>
class MyWeakPtr {
private:
    // weak ptr can not safely holding T*, cause T can be destroyed any time
    RefBlockBase* m_block = nullptr;

public:
    // allow shared ptr to access
    friend class MySharedPtr<T>;

    MyWeakPtr() noexcept = default;

    // construct from my shared ptr
    MyWeakPtr(const MySharedPtr<T>& shared) noexcept
        : m_block(shared.m_block){
        if (m_block){
            m_block->increment_weak();
        }
    }

    ~MyWeakPtr() noexcept {
        if(m_block){
            m_block -> decrement_weak();
        }
    }


    bool expired() const noexcept {
        return !m_block || m_block -> m_shared_count.load(std::memory_order_acquire) == 0;
    }

    // core utility lock()
    MySharedPtr<T> lock() const noexcept{
        if(expired()){
            return MySharedPtr<T>();
        }

        // try to atomatically increment shared count
        if (m_block -> try_increment_shared()){
            //sucess, now we are one of the legal owners
            return MySharedPtr<T>(reinterpret_cast<T*>(m_block->get_resource_ptr()), m_block);

        }else{
            // if counter turn to zero just at the time we try to check and access
            return MySharedPtr<T>();
        }
    }
};

template <typename T, typename... Args>
MySharedPtr<T> MyMakeShared(Args&&... args){
    // allocate one time for space for T and RefBlock
    // allocate refblockmakeshared, which includes already size for T
    RefBlockMakeShared<T>* block = new RefBlockMakeShared<T>();

    // get pointer to T
    void* void_ptr = block->get_resource_ptr();
    T* ptr = static_cast<T*>(void_ptr);

    // placement new to construct T on this address
    try {
        ::new (ptr) T(mystl::forward<Args>(args)...);

    } catch (...){
        // fail to construct
        delete block;
        throw;
    }

    // use private constructor to return mysharedptr
    // now the RefBlockBase's m
    return MySharedPtr<T>(ptr, block);
}


} // namespace mystl
#endif

