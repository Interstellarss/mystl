#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE

#include "my_allocator.h"
#include "my_iterator.h"
#include "my_utility.h"
#include "my_vector.h"

#include <vector>
#include <functional>
#include <cstddef>
#include <initializer_list>

namespace mystl{

// ======================
// Hash Node
// ======================
template <typename Key, typename Value>
struct hash_node{
    mystl::pair<Key, Value> data;
    hash_node* next{nullptr};

    hash_node(const Key& k, const Value& v, hash_node* n = nullptr)
        : data(k, v), next(n) {}
};

// =====================
// Hash Table
// =====================
template <
    typename Key,
    typename Value,
    //TODO: replace this hash
    typename Hash = std::hash<Key>,
    typename KeyEqual = mystl::equal_to<Key>,
    typename Alloc = mystl::MyAllocator<hash_node<Key, Value>>>
class MyHashTable{
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = mystl::pair<Key,Value>;
    using size_type = std::size_t;
    using node_type = hash_node<Key, Value>;
    using allocator_type = Alloc;

private:
    mystl::MyVector<node_type*> buckets_;
    size_type size_{0};
    float max_load_factor_{0.75f};
    Hash hasher_;
    KeyEqual key_equal_;
    allocator_type alloc_;

public:
    MyHashTable(size_type bucket_count = 8)
        : buckets_(bucket_count, nullptr), size_(0) {}
    
    ~MyHashTable(){
        clear();
    }

    bool empty() const noexcept {return size_ ==0;}

    size_type size() const noexcept { return size_;}
    size_type bucket_count() const noexcept {return buckets_.size();}

    void clear(){
        for(auto& head : buckets_){
            while(head){

            }
        }
    }

    void rehash(size_type new_count){
        mystl::MyVector<node_type*> new_buckets(new_count, nullptr);

        for(auto& head : buckets_){
            while(head){
                node_type* next = head->next;
                size_t index = hasher_(head->data.first) % new_count;
                head->next = new_buckets[index];
                new_buckets[index] = head;
                head = next;
            }
        }
        buckets_.swap(new_buckets);
    }

    float load_factor() const noexcept{
        return static_cast<float>(size_) / static_cast<float>(buckets_.size());
    }

    void insert(const Key& key, const Value& value){
        if(load_factor() > max_load_factor_){
            rehash(buckets_.size() * 2);
        }

        size_t idx = hasher_(key) % buckets_.size();
        node_type* head = buckets_[idx];

        for(node_type* node = head; node; node = node->next){
            if(key_equal_(node->data.first, key)){
                node->data.second = value;
                return;
            }
        }

        node_type* new_node = alloc_.allocate(1);
        alloc_.construct(new_node, key, value, head);
        buckets_[idx] = new_node;
        ++size_;
    }

    Value* find(const Key& key){
        size_t idx = hasher_(key) % buckets_.size();
        for(node_type* node = buckets_[idx]; node; node = node->next){
            if(key_equal_(node->data.first, key))
                return &node->data.second;;
        }
        return nullptr;
    }

    bool erase(const Key& key){
        size_t idx = hasher_(key) % buckets_.size();
        node_type* prev = nullptr;
        node_type* curr = buckets_[idx];
        while(curr){
            if(key_equal_(curr->data.first, key)){
                if(prev) prev->next = curr->next;
                else buckets_[idx] = curr->next;

                alloc_.destroy(curr);
                alloc_.deallocate(curr, 1);
                --size_;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    Value& operator[](const Key& key){
        Value* val = find(key);
        if(val) return *val;
        insert(key, Value());
        return *find(key);
    }

    void print_debug() const {
        for (size_t i = 0; i < buckets_.size(); ++i) {
            node_type* node = buckets_[i];
            if (!node) continue;
            std::cout << "[" << i << "]: ";
            while (node) {
                std::cout << "(" << node->data.first << "," << node->data.second << ") -> ";
                node = node->next;
            }
            std::cout << "nullptr\n";
        }
    }
};

}// namespace mystl
#endif // MY_HASH_TABLE