#ifndef MY_UNIQUE_PTR
#define MY_UNIQUE_PTR


#include <iostream>
#include "my_utility.h"

namespace mystl{

template <typename T>
class MyUniquePtr{
private:
    T* m_ptr;

public:
    // construct func
    explicit MyUniquePtr(T* ptr = nullptr) noexcept
        : m_ptr(ptr){}

    // deconstruct func: release resource
    ~MyUniquePtr() noexcept{
        delete m_ptr;
    }

    //---- ban copy ----
    MyUniquePtr(const MyUniquePtr& other) = delete;
    //---- ban copy operator ----
    MyUniquePtr& operator=(const MyUniquePtr& other) = delete;

    //---- enable move ---
    MyUniquePtr(MyUniquePtr&& other) noexcept
        : m_ptr(other.m_ptr){
        // "steal" the old ptr
        other.m_ptr = nullptr;
    }

    // Move operator
    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept{
        if(this != &other){
            // 1. release the resource current holding
            delete m_ptr;

            //2. steal other resource
            m_ptr = other.m_ptr;

            // 3. nullptr of other;
            other.m_ptr = nullptr;
        }
        return *this;
    }

    // behavior as pointer
    T& operator*() const noexcept {return *m_ptr;}
    T* operator->() const noexcept {return m_ptr;}

    // check if holding pointer
    explicit operator bool() const noexcept {
        return m_ptr != nullptr;
    }

    // access to the pointer
    T* get() const noexcept{
        return m_ptr;
    }

    // release ownership and return pointer
    T* release() noexcept{
        T* temp = m_ptr;
        m_ptr = nullptr;
        return temp;
    }
    
    //reset
    void reset(T* ptr = nullptr) noexcept{
        delete m_ptr;
        m_ptr = ptr;
    }

};
} // namespace mystl
#endif // MY_UNIQUE_PTR
