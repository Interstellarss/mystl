#ifndef MY_DEQUE_H
#define MY_DEQUE_H

#include "my_allocator.h"
#include "my_iterator.h"

#include <cstddef>

namespace mystl{

// Helper function for buffer size calculation
constexpr size_t __deque_buffer_size(size_t n, size_t sz){
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

// ====================
// deque iterator (block-aware)
// ====================

template <typename T, typename Ref, typename Ptr>
class deque_iterator : public mystl::iterator<mystl::random_access_iterator_tag, T>{
public:
    using self = deque_iterator<T,Ref, Ptr>;
    using base_type = mystl::iterator<mystl::random_access_iterator_tag, T>;
    using value_type = typename base_type::value_type;
    using difference_type = typename base_type::difference_type;

    using pointer = Ptr;
    using reference = Ref;
    using map_pointer = T**; // pointer to current node in map

    pointer cur{nullptr};   // current element
    pointer first{nullptr}; // start of block
    pointer last{nullptr};  // end of block (one past)
    map_pointer node{nullptr}; // pointer into map

    static size_t buffer_size(){ return __deque_buffer_size(0, sizeof(T)); }

    deque_iterator() = default;
    deque_iterator(pointer c, map_pointer n) : cur(c), first(*n), last(*n + buffer_size()), node(n) {}

    void set_node(map_pointer new_node){ node = new_node; first = *new_node; last = first + buffer_size(); }

    reference operator*() const { return *cur; }
    pointer operator->() const { return cur; }

    self& operator++(){ ++cur; if (cur == last){ set_node(node + 1); cur = first; } return *this; }
    self operator++(int){ self tmp = *this; ++(*this); return tmp; }
    self& operator--(){ if (cur == first){ set_node(node - 1); cur = last; } --cur; return *this; }
    self operator--(int){ self tmp = *this; --(*this); return tmp; }

    self& operator+=(difference_type n){
        difference_type offset = n + (cur - first);
        if (offset >= 0 && static_cast<size_t>(offset) < buffer_size()) cur += n;
        else {
            difference_type bs = static_cast<difference_type>(buffer_size());
            difference_type node_offset = offset > 0 ? offset / bs : -(( -offset - 1) / bs) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * bs);
        }
        return *this;
    }
    self operator+(difference_type n) const { self tmp = *this; return tmp += n; }
    self& operator-=(difference_type n){ return *this += -n; }
    self operator-(difference_type n) const { self tmp = *this; return tmp -= n; }

    difference_type operator-(const self& rhs) const {
        return difference_type(buffer_size()) * (node - rhs.node - 1) + (cur - first) + (rhs.last - rhs.cur);
    }
    reference operator[](difference_type n) const { return *(*this + n); }

    bool operator==(const self& rhs) const { return cur == rhs.cur && node == rhs.node; }
bool operator!=(const self& rhs) const { return !(*this == rhs); }

    bool operator<(const self& rhs) const { return (node == rhs.node) ? (cur < rhs.cur) : (node < rhs.node); }
};

template <typename T, class Alloc = MyAllocator<T>>
class MyDeque{
public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = deque_iterator<T, T&, T*>;
    using const_iterator = deque_iterator<T, const T&, const T*>;

private:
    using map_pointer = T**;

    allocator_type alloc_;
    map_pointer map_{nullptr};
    size_type map_size_{0};
    iterator start_{};
    iterator finish_{};
    size_type size_{0};

    static size_t buffer_size(){ return __deque_buffer_size(0, sizeof(T)); }
    T* allocate_node(){ return alloc_.allocate(buffer_size()); }
    void deallocate_node(T* p){ alloc_.deallocate(p, buffer_size()); }

    void create_map_and_nodes(size_type n){
        size_type num_nodes = n / buffer_size() + 1;
        map_size_ = (num_nodes + 2 > 8) ? num_nodes + 2 : 8;
        map_ = new T*[map_size_];
        
        // Initialize map to nullptrs
        for (size_type i = 0; i < map_size_; ++i) {
            map_[i] = nullptr;
        }
        
        map_pointer nstart = map_ + (map_size_ - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;
        
        // Allocate nodes
        for (map_pointer cur = nstart; cur <= nfinish; ++cur) {
            *cur = allocate_node();
        }
        
        start_.set_node(nstart);
        start_.cur = start_.first;
        finish_.set_node(nfinish);
        finish_.cur = finish_.first + (n % buffer_size());
    }

    void reallocate_map(size_type nodes_to_add, bool add_at_front){
        size_type old_num_nodes = finish_.node - start_.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;
        
        if (map_size_ >= new_num_nodes + 2){
            map_pointer new_start = map_ + (map_size_ - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        
            if (new_start != start_.node){
                if (new_start < start_.node){
                    for (size_type i = 0; i < old_num_nodes; ++i) {
                        new_start[i] = start_.node[i];
                    }
                } else {
                    for (size_type i = old_num_nodes; i > 0; --i) {
                        new_start[i - 1] = start_.node[i - 1];
                    }
                }
                start_.set_node(new_start);
                finish_.set_node(new_start + old_num_nodes - 1);
            }
            return;
        }
        
        // Allocate new larger map
        size_type new_map_size = map_size_ ? map_size_ * 2 : 8;
        while (new_map_size < new_num_nodes + 2) new_map_size *= 2;
        
        map_pointer new_map = new T*[new_map_size];
        // Initialize new map to nullptrs
        for (size_type i = 0; i < new_map_size; ++i) {
            new_map[i] = nullptr;
        }
        
        map_pointer new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        
        // Copy existing node pointers
        for (size_type i = 0; i < old_num_nodes; ++i) {
            new_start[i] = start_.node[i];
        }
        
        delete [] map_;
        map_ = new_map;
        map_size_ = new_map_size;
        start_.set_node(new_start);
        finish_.set_node(new_start + old_num_nodes - 1);
    }
    
    void reserve_map_at_back(size_type nodes_to_add = 1){
        if (nodes_to_add > size_type(map_ + map_size_ - 1 - finish_.node)){
            reallocate_map(nodes_to_add, false);
        }
    }
    
    void reserve_map_at_front(size_type nodes_to_add = 1){
        if (nodes_to_add > size_type(start_.node - map_)){
            reallocate_map(nodes_to_add, true);
        }
    }

public:
    MyDeque() = default;
    
    explicit MyDeque(size_type n){
        create_map_and_nodes(n);
        size_ = n;
        iterator it = start_;
        for (size_type i = 0; i < n; ++i, ++it){
            alloc_.construct(&*it);
        }
    }
    
    
    MyDeque(std::initializer_list<T> init){
        create_map_and_nodes(init.size());
        size_ = 0;
        for (const auto& v : init){
            push_back(v);
        }
    }
    
    ~MyDeque(){
        // Destroy all elements
        if (size_) {
            for (iterator it = start_; it != finish_; ++it) {
                alloc_.destroy(&*it);
            }
        }
        
        // Deallocate all blocks
        if (map_) {
            for (size_type i = 0; i < map_size_; ++i) {
                if (map_[i]) {
                    deallocate_node(map_[i]);
                }
            }
            delete [] map_;
        }
    }

    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }

    reference front() {
        if (empty()) throw std::out_of_range("deque is empty");
        return *start_;
    }

    reference back(){
        if (empty()) throw std::out_of_range("deque is empty");
        iterator tmp = finish_;
        --tmp;
        return *tmp;
    }

    const_reference front() const{
        if (empty()) throw std::out_of_range("deque is empty");
        return *start_;
    }
    const_reference back() const{
        if (empty()) throw std::out_of_range("deque is empty");
        const_iterator tmp = finish_;
        --tmp;
        return *tmp;
    }

    iterator begin() noexcept { return start_; }
    iterator end() noexcept { return finish_; }
    const_iterator begin() const noexcept { return const_iterator(start_.cur, start_.node); }
    const_iterator end() const noexcept { return const_iterator(finish_.cur, finish_.node); }

    void clear(){
        for (iterator it = start_; it != finish_; ++it) {
            alloc_.destroy(&*it);
        }
        start_.cur  = start_.first;
        finish_.set_node(start_.node);
        finish_.cur = finish_.first;
        size_ = 0;
    }
    

    void push_back(const_reference value){
        if (!map_) create_map_and_nodes(0);
        
        if (finish_.cur != finish_.last - 1){
            // Construct in-place
            alloc_.construct(finish_.cur, value);
            ++finish_.cur;
        } else {
            // Allocate new block at back
            reserve_map_at_back(1);
            *(finish_.node + 1) = allocate_node();
            alloc_.construct(finish_.cur, value);
            finish_.set_node(finish_.node + 1);
            finish_.cur = finish_.first;
        }
        ++size_;
    }
    
    void push_front(const_reference value){
        if (!map_) create_map_and_nodes(0);
        
        if (start_.cur != start_.first){
            --start_.cur;
            alloc_.construct(start_.cur, value);
        } else {
            // Allocate new block at front
            reserve_map_at_front(1);
            *(start_.node - 1) = allocate_node();
            start_.set_node(start_.node - 1);
            start_.cur = start_.last - 1;
            alloc_.construct(start_.cur, value);
        }
        ++size_;
    }
    
    void pop_back(){
        if (empty()) throw std::out_of_range("deque is empty");
        
        if (finish_.cur != finish_.first){
            --finish_.cur;
            alloc_.destroy(finish_.cur);
        } else {
            // Move to previous block
            finish_.set_node(finish_.node - 1);
            finish_.cur = finish_.last - 1;
            alloc_.destroy(finish_.cur);
        }
        --size_;
    }
    
    void pop_front(){
        if (empty()) throw std::out_of_range("deque is empty");
        
        if (start_.cur != start_.last - 1){
            alloc_.destroy(start_.cur);
            ++start_.cur;
        } else {
            alloc_.destroy(start_.cur);
            start_.set_node(start_.node + 1);
            start_.cur = start_.first;
        }
        --size_;
    }
};

} // namespace mystl

#endif // MY_DEQUE_H