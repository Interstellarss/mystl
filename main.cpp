#include <iostream>

#include "my_vector.h"
#include "my_list.h"
#include "my_deque.h"
#include "my_queue.h"
#include "my_stack.h"
#include "my_rbtree.h"
#include "my_set.h"
#include "my_map.h"
#include "my_binary_heep.h"
#include "my_hashtable.h"
#include "my_unordered_set.h"
#include "my_unordered_map.h"
#include "my_multiset.h"
#include "my_multimap.h"
#include "my_priority_queue.h"

#include "ptr/my_ptr.h"
#include "ptr/my_unique_ptr.h"


#include "my_utility.h"
#include "my_algorithm.h"

#include <string>


int main() {

    // =====================
    // Testing myvector
    // =====================
    std::cout << "======== Test MyVector =============\n";
    mystl::MyVector<int> vec = {1, 2, 3, 4};

    vec.push_back(5);

    std::cout << "Vector elements: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }

    std::cout << "\n";

    std::cout << "Reverse Vector elements: ";
    for (auto rit = vec.rbegin(); rit != vec.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n";

    //testing pop_back
    vec.pop_back();
    std::cout << "After pop_back, Vector elements: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    vec.reserve(10); 
    std::cout << "\nAfter reserve(10), capacity: " << vec.capacity() << "\n";
    vec.resize(8, 0);
    std::cout << "After resize(8, 0), Vector elements: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Testing my algorithm

    mystl::MyVector<int> v = {5, 3, 1, 4, 2};

    mystl::sort(v.begin(), v.end());
    mystl::reverse(v.begin(), v.end());

    auto it = mystl::find(v.begin(), v.end(), 3);
    if (it != v.end())
        std::cout << "Found 3 at position: " << (it - v.begin()) << "\n";

    std::cout << "Sorted & reversed vector: ";
    for (auto x : v) std::cout << x << " ";
    
    // ===================
    // Test for mylist
    // ===================
    std::cout << "\n ======== Test MyList =============\n";

    mystl::MyList<int> lst = {1, 2, 3};

    std::cout << "Forward: ";
    for (auto it = lst.begin(); it != lst.end(); ++it)
        std::cout << *it << " ";
    
    lst.push_front(0);
    lst.push_back(4);

    std::cout << "\nReverse: ";
    for (auto it = --lst.end(); ; --it) {
        std::cout << *it << " ";
        if (it == lst.begin()) break;
    }

    // Test my deque
    std::cout << "\n ======== Test MyDeque =============\n";
    mystl::MyDeque<int> dq = {5, 3, 1, 4, 2};
    std::cout << "\nDeque elements: ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Test deque modifications
    dq.push_front(6);
    dq.push_back(7);
    std::cout << "After push_front(6) and push_back(7): ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    dq.pop_front();
    dq.pop_back();
    std::cout << "After pop_front() and pop_back(): ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Test deque with more elements to trigger reallocation
    mystl::MyDeque<int> large_dq;
    for (int i = 0; i < 100; ++i) {
        large_dq.push_back(i);
    }
    std::cout << "Large deque size: " << large_dq.size() << "\n";
    
    // Verify all elements
    bool all_ok = true;
    for (int i = 0; i < 100; ++i) {
        if (large_dq.begin()[i] != i) {
            all_ok = false;
            break;
        }
    }
    std::cout << "Large deque elements " << (all_ok ? "correct" : "incorrect") << "\n";

    // Additional deque validation
    mystl::MyDeque<int> validation_dq;
    for (int i = 0; i < 1000; ++i) {
        validation_dq.push_back(i);
    }
    for (int i = 0; i < 500; ++i) {
        validation_dq.pop_front();
    }
    for (int i = 0; i < 500; ++i) {
        validation_dq.push_front(i);
    }
    std::cout << "Validation deque size: " << validation_dq.size() << "\n";
    
    bool validation_ok = true;
    for (int i = 0; i < 500; ++i) {
        if (validation_dq.begin()[i] != 499 - i) {
            validation_ok = false;
            break;
        }
    }
    for (int i = 500; i < 1000; ++i) {
        if (validation_dq.begin()[i] != i) {
            validation_ok = false;
            break;
        }
    }
    std::cout << "Validation deque " << (validation_ok ? "passed" : "failed") << "\n";

    std::cout << "======== Test MyQueue =============\n";


    mystl::MyQueue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);
    std::cout << "Queue front: " << q.front() << ", back: " << q.back() << "\n";
    q.pop();
    std::cout << "After pop front: " << q.front() << "\n";

    mystl::MyStack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    std::cout << "Stack top: " << s.top() << "\n";
    s.pop();
    std::cout << "After pop top: " << s.top() << "\n";

    std::cout << "Red Black Tree tree:" << std::endl; 

    mystl::MyRBTree<int, int, mystl::identity<int>, mystl::less<int>> tree;
    //tree.insert_equal(10);

    for (int i = 1; i <= 10; ++i)
        tree.insert_equal(i);
    
    std::cout << "rbtree insert done!" << std::endl;
    

    for (int i = 1; i <= 9; ++i)
        tree.erase(tree.find(i));  // 全删一遍
    
    std::cout << "rbtree erase done!"  << std::endl;


    std::cout << "==== TEST MySet ====\n";
    mystl::MySet<int> set;

    set.insert(10);
    set.insert(5);
    set.insert(20);
    set.insert(10);  // duplicate, should not be inserted

    std::cout << "Set contents (in order): ";
    for (auto it = set.begin(); it != set.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    set.erase(5);
    std::cout << "After erase(5): ";
    for (auto it = set.begin(); it != set.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    if (set.find(10) != set.end()) {
        std::cout << "Found 10 in set!\n";
    }

    std::cout << "Size: " << set.size() << "\n\n";


    std::cout << "==== TEST MyMap ====\n";
    mystl::MyMap<std::string, int> m;

    m["apple"] = 3;
    m["banana"] = 5;
    m["cherry"] = 7;

    m.insert(mystl::make_pair(std::string("date"), 9));

    std::cout << "Map contents (in order):\n";
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << " => " << it->second << "\n";
    }

    m.erase("banana");

    std::cout << "\nAfter erase('banana'):\n";
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << " => " << it->second << "\n";
    }

    std::cout << "m['apple'] = " << m["apple"] << "\n";
    std::cout << "m['new_fruit'] (default) = " << m["new_fruit"] << "\n";

    std::cout << "Size: " << m.size() << "\n";

    std::cout << "======== Test MyHashTable =============\n";

    // Create a hash table (int -> std::string)
    mystl::MyHashTable<int, std::string, std::hash<int>, mystl::equal_to<int>> table;

    // Insert some elements
    table.insert(1, "Apple");
    table.insert(2, "Banana");
    table.insert(3, "Cherry");

    std::cout << "After insertion:\n";
    table.print_debug();

        // Find existing and missing keys
    if (auto val = table.find(2)) {
        std::cout << "\nKey 2 found: " << *val << "\n";
    } else {
        std::cout << "\nKey 2 not found\n";
    }

    if (auto val = table.find(10)) {
        std::cout << "Key 10 found: " << *val << "\n";
    } else {
        std::cout << "Key 10 not found\n";
    }

     // Erase a key
    std::cout << "\nErasing key 1...\n";
    table.erase(1);
    table.print_debug();

    std::cout << "======== Test MyUnorderedMap =============\n";


    mystl::MyUnorderedMap<std::string, int> umap;
    umap["apple"] = 10;
    umap["banana"] = 20;
    umap.insert("orange", 30);

    umap.print_debug();

    std::cout << "banana: " << *umap.find("banana") << "\n";
    umap.erase("apple");
    umap.print_debug();

    std::cout << "======== Test MyUnorderedSet =============\n";
    
    mystl::MyUnorderedSet<int> uset;
    uset.insert(5);
    uset.insert(7);
    uset.insert(7);
    uset.insert(9);

    uset.print_debug();
    std::cout << "Contains 7? " << (uset.contains(7) ? "Yes" : "No") << "\n";

    std::cout <<std::endl;

    std::cout << "======== Test MyUniquePtr =============\n";
    mystl::MyUniquePtr<int> uptr1(new int(10));
    std::cout << "uptr1 value: " << *uptr1 << std::endl;
    mystl::MyUniquePtr<int> uptr2 = std::move(uptr1);
    std::cout << "uptr2 value: " << *uptr2 << std::endl;
    std::cout << "uptr1 is now null: " << (uptr1.get() == nullptr) << std::endl;

    std::cout << "======== Test MySharedPtr =============\n";
    mystl::MySharedPtr<int> sptr1 = mystl::MyMakeShared<int>(20);
    std::cout << "sptr1 value: " << *sptr1 << std::endl;
    std::cout << "sptr1 use count: " << sptr1.use_count() << std::endl;
    {
        mystl::MySharedPtr<int> sptr2 = sptr1;
        std::cout << "sptr1 use count: " << sptr1.use_count() << std::endl;
    }
    std::cout << "sptr1 use count: " << sptr1.use_count() << std::endl;

    std::cout << "======== Test MyWeakPtr =============\n";
    mystl::MyWeakPtr<int> wptr1(sptr1);
    std::cout << "wptr1 expired: " << wptr1.expired() << std::endl;
    if (auto sptr3 = wptr1.lock()) {
        std::cout << "sptr3 value: " << *sptr3 << std::endl;
        std::cout << "sptr1 use count: " << sptr1.use_count() << std::endl;
    }
    sptr1 = nullptr;
    std::cout << "wptr1 expired: " << wptr1.expired() << std::endl;

    std::cout << "======== Test  MyBinary Heap =============\n";
    mystl::MyBinaryHeap<int> heap;

    heap.push(10);
    heap.push(50);
    heap.push(20);
    heap.push(40);

    //std::cout << " Test! \n" << std::endl;

    while (!heap.empty())
    {
        //std::cout << " Test! \n" << std::endl;
        std::cout << heap.top() << " ";
        //std::cout << " Test! \n" << std::endl;
        heap.pop();
    }
    std::cout << std::endl;
    std::cout << " MyBinaryHeap Test Finished! \n" << std::endl;

    std::cout << "======== Test Priority Queue =============\n";

    mystl::MyPriorityQueue<int> pq;
    pq.push(10);
    pq.push(50);
    pq.push(20);    
    pq.push(40);

    while (!pq.empty())
    {
        //std::cout << " Test! \n" << std::endl;
        std::cout << pq.top() << " ";
        //std::cout << " Test! \n" << std::endl;
        pq.pop();
    }
    std::cout << std::endl;
    std::cout << " MyPriorityQueue Test Finished! \n" << std::endl;

    std::cout << "======== Test MyMultiMap =============\n";

    mystl::MyMultimap<int, std::string> mp;
    mp.insert({1, "a"});
    mp.insert({1, "b"});
    mp.insert({2, "c"});


    std::cout << " MyMultimap Test Finished! \n" << std::endl;


    return 0;
}