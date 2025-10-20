#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H

#include "my_iterator.h"
#include "my_utility.h"

namespace mystl{

// =========== find ===============
template <typename InputIt, typename T>
InputIt find(InputIt first, InputIt last, const T& value){
    for(; first != last; ++first){
        if(*first == value){
            return first;
        }
    }
    return last;
}

// ============ count =============
template <typename InputIt, typename T>
typename mystl::iterator_traits<InputIt>::difference_type
count(InputIt first, InputIt last, const T& value){
    typename mystl::iterator_traits<InputIt>::difference_type n = 0;
    for(; first != last; ++first){
        if(*first == value){
            ++n;
        }
    }
    return n;
}


// ============= copy ==============
// template <typename InputIt, typename OutputIt>
// OutputIt copy(InputIt first, InputIt last, OutputIt result){
//     for(; first != last; ++first, ++last){
//         *result = *first;
//     }
//     return result;
// }
template <typename InputIt, typename OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt result){
    for(; first != last; ++first, ++result){
        *result = *first;
    }
    return result;
}



// ============== fill ==============
template <typename ForwardIt, typename T>
void fill(ForwardIt first, ForwardIt last, const T& value){
    for(; first != last; ++first){
        *first = value;
    }
}

// ============== swap ==============
template <typename T>
void swap(T& a, T& b){
    T tmp = mystl::move(a);
    a = mystl::move(b);
    b = mystl::move(tmp);
}


// =============== reverve ===========
template <typename BidirectionalIt>
void reverse(BidirectionalIt first, BidirectionalIt last){
    while ((first != last) && (first != --last)){
        mystl::swap(*first, *last);
        ++first;
    }
}

// =============== min/max ==============
template <typename T, typename Compare>
const T& min(const T& a, const T& b, Compare comp){
    return comp(b, a) ? b : a;
}

template <typename T, typename Compare>
const T& max(const T& a, const T& b, Compare comp){
    return comp(a, b) ? b : a;
}

// =============== equal =================
template <typename InputIt1, typename InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2){
    for(; first1 != last1; ++first1, ++first2){
        if(*first1 != *first2){
            return false;
        }
    }
    return true;
}

// =============== lexicographical_compare =================
template <typename InputIt1, typename InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                             InputIt2 first2, InputIt2 last2){
    for(; first1 != last1 && first2 != last2; ++first1, ++first2){
        if(*first1 < *first2) return true;
        if(*first2 < *first1) return false;
    }
    return (first1 == last1) && (first2 != last2);
}

// =============== for each =============================
template <typename InputIt, typename UnaryFunction>
UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f){
    for(; first != last; ++first) f(*first);
    return f;
}

// =============== copy backward ===========================
template <typename BidirectionalIt1, typename BidirectionalIt2>
BidirectionalIt2 copy_backward(BidirectionalIt1 first, BidirectionalIt1 last, BidirectionalIt2 d_last){
    while (first != last) {
        *--d_last = *--last;
    }
    return d_last;
}


// ============== sort (quick sort impl) ===============
template <typename RandomIt>
void sort(RandomIt first, RandomIt last){
    if( first == last) return;
    RandomIt i = first;
    RandomIt j = last;

    --j;

    auto pivot = *first;

    RandomIt left = first;
    RandomIt right = j;

    while(left != right){
        while(*right >= pivot && left != right) --right;
        while(*left <= pivot && left != right) ++left;
        if(left != right) mystl::swap(*left, *right);
    }

    mystl::swap(*first, *left);
    sort(first, left);
    sort(++left, last);
}

} //namespace mystl

#endif // MY_ALGORITHM_H