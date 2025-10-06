#include "my_vector.h"
#include <iostream>

int main(){
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
    return 0;
}