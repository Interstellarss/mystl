#ifndef MY_UNORDERED_SET
#define MY_UNORDERED_SET

#include "my_hashtable.h"

namespace mystl{

template <
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = mystl::equal_to<Key>,
    typename Alloc = mystl::MyAllocator<hash_node<Key, Key>>>
class MyUnorderedSet{
public:
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Alloc;

    using table_type = MyHashTable<Key, Key, Hash, KeyEqual, Alloc>;

private:
    table_type table_;

public:
    explicit MyUnorderedSet(size_type bucket_count = 8)
        : table_(bucket_count){}

    bool empty() const noexcept {return table_.empty();}
    size_type size() const noexcept {return table_.size();}
    size_type bucket_count() const noexcept {return table_.bucket_count();}

    float load_factor() const noexcept {return table_.load_factor();}

    void clear() {table_.clear();}

    void insert(const key_type& key){
        table_.insert(key,key);
    }

    bool contains(const key_type& key) const {
        return const_cast<table_type&>(table_).find(key) != nullptr;
    }

    void rehash(size_type new_count){
        table_.rehash(new_count);
    }

    void print_debug() const {
        table_.print_debug();
    }
 

};
} // namespace mystl
#endif // MY_UNORDERED_SET