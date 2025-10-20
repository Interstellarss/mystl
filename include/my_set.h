#ifndef MY_SET_H
#define MY_SET_H

#include "my_rbtree.h"
#include "my_utility.h"

namespace mystl{

template <typename Key, typename Compare = std::less<Key>, typename Alloc = mystl::MyAllocator<mystl::RBTreeNode<Key>>>
class MySet{

private:
    using tree_type = mystl::MyRBTree<Key, Key, mystl::identity<Key>, Compare, Alloc>;
    tree_type tree_;

public:
    using key_type = Key;
    using value_type = Key;
    using iterator = typename tree_type::iterator;
    using const_iterator = typename tree_type::const_iterator;
    using size_type = typename tree_type::size_type;

    MySet() = default;

    bool empty() const noexcept {return tree_.empty();}
    size_type size() const noexcept {return tree_.size();}

    iterator begin() noexcept {return tree_.begin();}

    iterator end() noexcept {return tree_.end();}

    mystl::pair<iterator, bool> insert(const value_type& value){
        auto it = tree_.find(value);
        if(it != tree_.end()) {
            return {it, false};
        }
        return {tree_.insert_equal(value), true};
    }

    void erase(const value_type& value){
        auto it = tree_.find(value);
        if(it != tree_.end()){
            tree_.erase(it);
        }
    }

    iterator find(const key_type& key) {return tree_.find(key);}

    void clear() {tree_.clear();}

};
} // namespace mystl
#endif // MY_SET_H