#ifndef MY_BINARY_HEAP
#define MY_BINARY_HEAP

#include "my_vector.h"
#include <cstddef>

namespace mystl{

template <typename T, 
          typename Container = mystl::MyVector<T>,
          typename Compare = std::less<typename Container::value_type>> 
class MyBinaryHeap{
public:
    using value_type = T;
    using container_type = Container;
    using size_type = typename Container::size_type;
    using reference = T&;
    using const_reference = const T&;

private:
    container_type data_;
    Compare comp_;
public:
    MyBinaryHeap() = default;

    // return heap empty
    bool empty() const noexcept{
        return data_.empty();
    }

    size_type size() const noexcept{
        return data_.size();
    }

    const_reference top() const {
        if (empty()) throw std::out_of_range("heap is empty!");
        return data_[0];
    }

    void push(const_reference value){
        data_.push_back(value);
        sift_up(data_.size() - 1);
    }

    void pop(){
        if(empty()) throw std::out_of_range("heap is empty!");
        data_[0] = data_.back();
        data_.pop_back();
        if (!empty()) sift_down(0);
    }

private:

    bool need_swap(const T& parent, const T& child){
        return comp_(parent, child);
    }

    // sift up: compare the 
    void sift_up(size_type index){
        while(index > 0){
            size_type parent = ( index - 1) / 2;
            if (!need_swap(data_[parent],data_[index])) break;
            mystl::swap(data_[parent], data_[index]);
            index = parent;            
        }
    }

    void sift_down(size_type index){
        size_type n = data_.size();
        while(true){
            size_type left = index * 2 + 1;
            size_type right = index * 2 + 2;
            size_type largest = index;

            if (left < n && need_swap(data_[largest], data_[left])){
                largest = left;
            }
            if (right < n && need_swap(data_[largest], data_[right])){
                largest = right;
            }

            if (largest == index) break;

            mystl::swap(data_[index], data_[largest]);
            index = largest;
        }
    }

}; // namespace mystl
}
#endif //MY_BINARY_HEAP