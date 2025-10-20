#ifndef MY_UTILITY_H
#define MY_UTILITY_H

namespace mystl{

// remove_reference
template <class T> struct remove_reference {using type = T;};
template <class T> struct remove_reference<T&> {using type = T;};
template <class T> struct remove_reference<T&&> {using type = T;};


// move
template <class T>
constexpr typename remove_reference<T>::type&& move(T&& t) noexcept{
    return static_cast<typename remove_reference<T>::type&&>(t);
}

// forward (optional, for perfect forwarding)
template <class T>
constexpr T&& forward(typename remove_reference<T>::type& t) noexcept{
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept{
    return static_cast<T&&>(t);
}

// ============================
// function objects
// ============================

// identity: return the identity itself
template <class T>
struct identity{
    const T& operator()(const T& x)const noexcept {return x;}
};

// less: default comparator (a < b)
template <class T>
struct less{
    bool operator()(const T& lhs, const  T& rhs) const noexcept{
        return lhs < rhs;
    }
};

// equal_to: returns lhs == rhs
template <class T>
struct equal_to {
    constexpr bool operator()(const T& lhs, const T& rhs) const noexcept {
        return lhs == rhs;
    }
};


// ==============================
// my pair
// ==============================
template <typename T1, typename T2>
struct pair {
    using first_type = T1;
    using second_type = T2;

    T1 first;
    T2 second;

    // constructors
    constexpr pair() : first(), second() {}

    constexpr pair(const T1& a, const T2& b) : first(a), second(b) {}

    template <typename U1, typename U2>
    constexpr pair(U1&& a, U2&& b)
        : first(mystl::forward<U1>(a)), second(mystl::forward<U2>(b)) {}
    
    template <typename U1, typename U2>
    constexpr pair(const pair<U1, U2>& other)
        : first(other.first), second(other.second) {}

    // Copy and move
    pair(const pair&) = default;
    pair(pair&&) noexcept = default;
    pair& operator=(const pair&) = default;
    pair& operator=(pair&&) noexcept = default;

    // Comparison operators
    constexpr bool operator==(const pair& other) const{
        return first == other.first && second == other.second;
    }

    constexpr bool operator!=(const pair& other) const{
        return !(*this == other);
    }

    constexpr bool operator<(const pair& other) const {
        return first < other.first || (!(other.first < first) && second < other.second);
    }

    constexpr bool operator>(const pair& other) const {
        return other <*this;
    }

    constexpr bool operator<=(const pair& other) const {
        return !(other < *this);
    }

    constexpr bool operator>=(const pair& other) const {
        return !(*this<other);
    }
};

// ===============================
// mystl::make_pair helper
// ===============================
template <typename T1, typename T2>
constexpr pair<T1, T2> make_pair(T1&& a, T2&& b){
    return pair<T1, T2>(mystl::forward<T1>(a), mystl::forward<T2>(b));
}



// ===============================
// my hash func
// ===============================


} // namespace mystl

#endif // MY_UTILITY_H