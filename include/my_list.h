#ifndef MY_LIST_H
#define MY_LIST_H

#include "my_allocator.h"
#include "my_iterator.h"

#include <initializer_list>
#include <cstddef>
#include <cassert>


namespace mystl{

// ===================
// list node
// ===================
template <typename T>
struct list_node{
    list_node* prev;
    list_node* next;
    T data;
    list_node(const T& value = T()) : prev(nullptr), next(nullptr), data(value){}
};

// ===================
// list iterator
// ===================

template <typename T>
class list_iterator{
public:
    using iterator_catagory = mystl::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using node_ptr = list_node<T>*;

private:
    node_ptr node_; // current node

public:
    list_iterator() : node_(nullptr){}

    explicit list_iterator(node_ptr p) : node_(p){}

    reference operator*() const {return node_->data;};

    pointer operator->() const {return &(node_->data);}

    list_iterator& operator++(){
        node_ = node_->next;
        return *this;
    }

    list_iterator operator++(int){
        list_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    list_iterator& operator--(){
        node_ = node_-> prev;
        return *this;
    }

    list_iterator operator--(int){
        list_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const list_iterator& rhs) const {return node_ == rhs.node_;}
    bool operator!=(const list_iterator& rhs) const {return node_ != rhs.node_;}

    node_ptr get_node() const {return node_;}

};


// ===========================
// MyList
 template <typename T, typename Alloc = MyAllocator<list_node<T>>>
class MyList{
public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = list_iterator<T>;
    using const_iterator = list_iterator<T>;

private:
    using node_type = list_node<T>;
    using node_ptr = node_type*;

    allocator_type alloc_;
    node_ptr head_;
    size_type size_;

public:
    // ===== Constructors =======
    MyList() : size_(0){
        create_head();
    }

    MyList(std::initializer_list<T> ilist) : MyList(){
        for(const auto& val : ilist){
            push_back(val);
        }
    }

    ~MyList(){
        clear();
        destroy_head();
    }

    //====== Basic Operations =======
    bool empty() const noexcept {return size_ == 0;}

    size_type size() const noexcept {return size_;}

    iterator begin() noexcept {return iterator(head_->next);}

    iterator end() noexcept { return iterator{head_};}

    reference front() {return head_ -> next -> data;}

    reference back() {return head_ -> prev -> data;}

    void push_back(const T& value){
        insert_node_before(head_, value); //insert before sentinel
    }

    void push_front(const T& value){
        insert_node_before(head_->next, value);
    }

    void pop_back(){
        erase(head_->prev);
    }

    void pop_front(){
        erase(head_->next);
    }

    void clear() noexcept{
        node_ptr cur = head_-> next;
        while(cur != head_){
            node_ptr next = cur->next;
            destroy_node(cur);
            cur = next;
        }
        head_->next = head_->prev = head_;
        size_ = 0;
    }

    iterator insert(iterator pos, const T& value){
        node_ptr new_node = create_node(value);
        node_ptr cur = pos.get_node();

        new_node->next = cur;
        new_node->prev = cur->prev;
        cur->prev->next = new_node;
        cur->prev = new_node;

        ++size_;
        return iterator(new_node);
    }

    iterator erase(iterator pos){
        node_ptr cur = pos.get_node();
        node_ptr next_ = cur->next;

        cur->next->prev = cur->prev;
        cur->prev->next = cur->next;

        destroy_node(cur);
        --size_;
        return iterator(next_);
    }

private:
    void create_head(){
        head_ = alloc_.allocate(1);
        alloc_.construct(head_, node_type()); // properly construct sentinel
        head_->next = head_;
        head_->prev = head_;
    }

    void destroy_head(){
        alloc_.destroy(head_);
        alloc_.deallocate(head_, 1);
        head_ = nullptr;
    }

    node_ptr create_node(const T& value){
        node_ptr p = alloc_.allocate(1);
        alloc_.construct(p, value); // call list_node<T>(const T&) directly
        return p;
    }

    void destroy_node(node_ptr p){
        alloc_.destroy(p);
        alloc_.deallocate(p, 1);
    }

    void insert_node_before(node_ptr pos, const T& value){
        node_ptr new_node = create_node(value);
        new_node->next = pos;
        new_node->prev = pos->prev;
        pos->prev->next = new_node;
        pos->prev = new_node;
        ++size_;
    }
};

}   //namesapce mystl


#endif // MY_LIST_H