# mystl

`mystl` is a header-only C++17 project that reimplements a subset of the C++ Standard Template Library as a learning exercise. It provides container, algorithm, and utility components that mirror familiar STL interfaces while staying small and easy to read.

## Features
- Containers: `MyVector`, `MyList`, `MyDeque`, `MyQueue`, `MyStack`, `MySet`, `MyMap`, `MyUnorderedSet`, `MyUnorderedMap`
- Infrastructure pieces such as `MyAllocator`, iterator adapters (e.g. `vector_iterator`, `reverse_iterator`), and a red-black tree backbone for ordered containers.
- Algorithm utilities (`sort`, `find`, `reverse`, `copy`, `fill`, ...) implemented in `include/my_algorithm.h`.
- Header-only usage: include what you need and link against the `mystl` interface target.
- Example program (`main.cpp`) that exercises the major containers and algorithms.

## Requirements
- C++17 compatible compiler (tested with GCC)
- CMake 3.16 or newer

## Build
```sh
cmake -S . -B build
cmake --build build
```
This builds the optional `mystl_demo` example executable. Disable it with `-DMYSTL_BUILD_EXAMPLES=OFF`.

### Run the example
```sh
./build/mystl_demo
```

## Repository layout
- `include/` – header implementations for containers, algorithms, utilities, allocator, iterators.
- `main.cpp` – demonstration program.
- `CMakeLists.txt` – CMake project definition; installs headers when you call `cmake --install`.
- `build/` – default out-of-source build directory (created by the commands above).
- `src/` – reserved for future source-based extensions.

## Example
```cpp
#include "my_vector.h"
#include "my_algorithm.h"

int main() {
    mystl::MyVector<int> values = {5, 2, 4, 1, 3};
    mystl::sort(values.begin(), values.end());

    for (int v : values) {
        std::cout << v << ' ';
    }
}
```

