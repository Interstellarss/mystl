#ifndef MY_MyMultimap_H
#define MY_MyMultimap_H

#include "my_rbtree.h"
#include "my_utility.h"

namespace mystl {

template <typename Key, typename T,
          typename Compare = std::less<Key>,
          typename Alloc = mystl::MyAllocator<RBTreeNode< std::pair<const Key, T> >>>
class MyMultimap {
public:
    using key_type       = Key;
    using mapped_type    = T;
    using value_type     = std::pair<const Key, T>;
    using size_type      = size_t;
    using compare        = Compare;
    using allocator_type = Alloc;

private:
    using key_of_value = mystl::extract_key<value_type>;

    using tree_type =
        MyRBTree<Key, value_type, key_of_value, Compare, Alloc>;

    tree_type tree_;

public:
    using iterator = typename tree_type::iterator;
    using const_iterator = typename tree_type::const_iterator;

    // -------- constructors ----------
    MyMultimap() : tree_(Compare()) {}
    explicit MyMultimap(const Compare& comp) : tree_(comp) {}

    // -------- iterators ----------
    iterator begin() noexcept { return tree_.begin(); }
    iterator end() noexcept { return tree_.end(); }

    // -------- capacity ----------
    bool empty() const noexcept { return tree_.empty(); }
    size_type size() const noexcept { return tree_.size(); }

    // -------- modifiers ----------
    iterator insert(const value_type& value) {
        return tree_.insert_equal(value);
    }

    void erase(iterator pos) { tree_.erase(pos); }

    void clear() { tree_.clear(); }

    // -------- lookup ----------
    iterator find(const key_type& k) { return tree_.find(k); }
};

} // namespace mystl

#endif // MY_MyMultimap_H
