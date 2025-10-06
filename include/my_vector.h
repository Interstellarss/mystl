#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <stdexcept>
#include <memory>
#include "my_allocator.h"
#include "my_iterator.h"

namespace mystl {

template <class T, class Alloc = MyAllocator<T>>
class MyVector {
public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename allocator_type::value_type*;
    using const_pointer = const typename allocator_type::value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = mystl::reverse_iterator<iterator>;
    using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

private:
    allocator_type alloc_;
    pointer start_;
    pointer finish_;
    pointer end_of_storage_;

public:
    // ==== Constructors ====
    MyVector() : start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {};

    explicit MyVector(size_type n, const_reference value = value_type())
        : start_(alloc_.allocate(n)),
          finish_(start_),
          end_of_storage_(start_ + n) {
        std::uninitialized_fill_n(start_, n, value);
        finish_ = end_of_storage_;
    }

    MyVector(std::initializer_list<value_type> ilist){
        start_ = alloc_.allocate(ilist.size());
        finish_ = start_;
        end_of_storage_ = start_ + ilist.size();
        for (const auto& item : ilist) {
            alloc_.construct(finish_, item);
            ++finish_;
        }
    }

    ~MyVector() {
        clear();
        if (start_) {
            alloc_.deallocate(start_, end_of_storage_ - start_);
        }
    }


    // ===== Baisc Member Functions =====
    size_type size() const noexcept {
        return static_cast<size_type>(finish_ - start_);
    }

    size_type capacity() const noexcept {
        return static_cast<size_type>(end_of_storage_ - start_);
    }

    bool empty() const noexcept {
        return start_ == finish_;
    }

    reference operator[](size_type i) noexcept {
        return start_[i];
    }

    const_reference operator[](size_type i) const noexcept {
        return start_[i];
    }

    reference at(size_type i){
        if (i >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return start_[i];
    }


    void push_back(const_reference value) {
        if (finish_ == end_of_storage_) {
            reallocate();
        }
        alloc_.construct(finish_, value);
        ++finish_;
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        --finish_;
        alloc_.destroy(finish_);
    }

    void clear() noexcept {
        while (finish_ != start_) {
            --finish_;
            alloc_.destroy(finish_);
        }
    }

    void reserve(size_type new_cap) {
        if(new_cap <= capacity()){
            return;
        }
        reallocate_to(new_cap);
    }

    //resize the vector: change to logical size
    void resize(size_type new_size, const_reference value = value_type()){
        size_type old_size = size();

        if(new_size < old_size){
            //destroy extra elements
            while(finish_ != start_ + new_size){
                --finish_;
                alloc_.destroy(finish_);
            }
        }else if(new_size > old_size){
            if(new_size > capacity()){
                reallocate_to(std::max(new_size, capacity() * 2));
            }
            //construct new elements
            while(finish_ != start_ + new_size){
                alloc_.construct(finish_, value);
                ++finish_;
            }
        }
    }

    void shrink_to_fit(){
        if(capacity() == size()) return;
        reallocate_to(size());
    }


    // ===== Iterator Support =====
    iterator begin() noexcept {
        return start_;
    }

    iterator end() noexcept {
        return finish_;
    }

    const_iterator begin() const noexcept {
        return start_;
    }

    const_iterator end() const noexcept {
        return finish_;
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(finish_);
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(start_);
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(finish_);
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(start_);
    }

private:
    void reallocate(){

        size_type old_size = size();
        size_type old_capacity = capacity();
        size_type new_capacity = old_capacity == 0 ? 1 : old_capacity * 2;

        pointer new_start = alloc_.allocate(new_capacity);
        pointer new_finish = new_start;

        //move old data to new memory
        for(pointer p = start_; p != finish_; ++p, ++new_finish){
            alloc_.construct(new_finish, std::move(*p));
            alloc_.destroy(p);
        }

        if(start_){
            alloc_.deallocate(start_, old_capacity);
        }


        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + new_capacity;
    }

    void reallocate_to(size_type new_cap){
        size_type old_size = size();
        pointer new_start = alloc_.allocate(new_cap);
        pointer new_finish = new_start;

        //move old data to new memory
        for(pointer p = start_; p != finish_; ++p, ++new_finish){
            alloc_.construct(new_finish, std::move(*p));
            alloc_.destroy(p);
        }

        //free old memory
        if(start_){
            alloc_.deallocate(start_, old_size);
        }

        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + new_cap;
    }

};
}

#endif // MY_VECTOR_H
