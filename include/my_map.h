#ifndef MY_MAP_H
#define MY_MAP_H

#include "my_utility.h"
#include  "my_rbtree.h"

namespace mystl {

// etract key from pair (used for map)
template<typename Pair>
struct select1st{
    const typename Pair::first_type& operator()(const Pair& p) const noexcept{
        return p.first;
    }
};


template <typename Key, typename T, typename Compare = mystl::less<Key>,
          typename Alloc = mystl::MyAllocator<mystl::RBTreeNode<mystl::pair<const Key, T>>>>
class MyMap{

private:
    using value_type = mystl::pair<const Key, T>;
    using tree_type = mystl::MyRBTree<Key, value_type, select1st<value_type>, Compare, Alloc>;
    tree_type tree_;

public:
    using key_type = Key;
    using mapped_type = T;
    using iterator = typename tree_type::iterator;
    using const_iterator = typename tree_type::const_iterator;
    using size_type = typename tree_type::size_type;

    MyMap() = default;

    bool empty() const noexcept {return tree_.empty();}

    size_type size() const noexcept {return tree_.size();}

    iterator begin() noexcept {return tree_.begin();}
    iterator end() noexcept {return tree_.end();}


    // insert unique key
    mystl::pair<iterator, bool> insert(const value_type& value){
        auto it = tree_.find(value.first);
        if(it != tree_.end()){
            return {it, false};
        }
        it = tree_.insert_equal(value);
        return {it, true};
    }

    // Element access
    mapped_type& operator[](const key_type& key){
        auto it = tree_.find(key);
        if(it == tree_.end()){
            it = tree_.insert_equal(mystl::make_pair(key, T()));
        }

        return (*it).second;
    }

    iterator find(const key_type& key){return tree_.find(key);}

    void erase(const key_type& key){
        auto it = tree_.find(key);
        if(it != tree_.end()){
            tree_.erase(it);
        }
    }
};
} // namespace mystl
#endif //MY_MAP_H