#pragma once
#include"BtreeNode.hpp"

template<typename T, uint16_t M = 256,typename Allocator = std::allocator<T>>
class Btree
{

private:
    using TNode = BtreeNode<T,M>;

    /*
        Rebind allocatore:
        L'allocatore semplice per il tipo T, viene trasformato in  allocatore per un TNode.
    */
    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<TNode>;
    NodeAlloc node_allocator;

    TNode *root;

public:
    inline Btree():root(nullptr){};
    ~Btree();
};


