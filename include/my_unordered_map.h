#ifndef MY_UNORDERED_MAP
#define MY_UNORDERED_MAP


#include "my_hashtable.h"

namespace mystl {

template <
    typename Key,
    typename Value,
    typename Hash = std::hash<Key>,
    typename KeyEqual = mystl::equal_to<Key>,
    typename Alloc = mystl::MyAllocator<hash_node<Key, Value>>>
class MyUnorderedMap{
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = mystl::pair<Key, Value>;
    using size_type = std::size_t;
    using allocator_type = Alloc;
    using table_type = MyHashTable<Key, Value, Hash, KeyEqual, Alloc>;

private:
    table_type table_;

public:
    explicit MyUnorderedMap(size_type bucket_count = 8)
        : table_(bucket_count) {}
    
    bool empty() const noexcept {return table_.empty();}

    size_type size() const {return table_.size();}

    size_type bucket_count() const noexcept {return table_.bucket_count();}

    float load_factor() const noexcept {return table_.load_factor();}

    void clear() {table_.clear();}

    void insert(const key_type& key, const mapped_type& value){
        table_.insert(key, value);
    }

    bool erase(const key_type& key){
        return table_.erase(key);
    }

    mapped_type* find(const key_type& key){
        return table_.find(key);
    }

    mapped_type& operator[](const key_type& key){
        return table_[key];
    }

    void rehash(size_type new_count){
        table_.rehash(new_count);
    }

    void print_debug() const {
        table_.print_debug();
    }

};
} // namespace mystl
#endif // MY_UNORDERED_MAP