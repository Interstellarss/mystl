#ifndef POOL_RESOURCE_H
#define POOL_RESOURCE_H

#include "memory_resource.h"
#include <vector>
#include <iostream>

namespace my_pmr {

class SingleSizePoolResource : public MemoryResource{
private:
    struct Chunk {
        Chunk* next; //free linked list
    };

    MemoryResource* m_upstream;

    Chunk* m_head = nullptr;

};
} //namespace my_pmr
#endif // POOL_RESOURCE_H