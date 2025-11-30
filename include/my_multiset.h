#ifndef MY_MULTI_SET
#define MY_MULTI_SET

#include "my_rbtree.h"
#include "my_utility.h"
#include "my_allocator.h"
#include <cstddef>

namespace mystl {

template<typename Key,
         typename Compare = std::less<Key>,
         typename Alloc = mystl::MyAllocator<RBTreeNode<Key>>>
class MyMultiset {
public:
    using key_type = Key;
    using value_type = Key;
    using size_type = size_t;
    using compare = Compare;
    using allocator_type = Alloc;

private:
    using tree_type = 
        MyRBTree<Key, value_type, mystl::identity<value_type>, Compare, Alloc>;
    
    tree_type tree_;

public:
    using iterator = typename tree_type::iterator;
    using const_iterator = typename  tree_type::const_iterator;


    // constructor
    MyMultiset() : tree_(Compare()) {}
    explicit MyMultiset(const Compare& comp) : tree_(comp) {}

    // iterator
    iterator begin() noexcept {return tree_.begin();}
    iterator end() noexcept {return tree_.end();}

    const_iterator begin() const noexcept {return tree_.begin();}
    const_iterator end() const noexcept {return tree_.end();}

    // capacity
    bool emptyy() const noexcept {return tree_.empty();}
    size_type size() const noexcept {return tree_.size();}

    // modifiers
    iterator insert(const value_type& value){
        return tree_.insert_equal(value);
    }

    void erase(iterator pos) {tree_.erase(pos);}

    void clear() { tree_.clear();}

    // lookup
    iterator find(const key_type& key) {return tree_.find(key);}
};

} // namespace mystl
#endif //MU_MULTI_SET