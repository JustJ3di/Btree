#pragma once
#include"BtreeNode.hpp"

template<typename T, uint16_t M = 256,typename Allocator = std::allocator<BtreeNode<T,M>>>
class Btree
{

private:
    using TNode = BtreeNode<T,M>;
    TNode *root;
    Allocator a;

public:

    Tnode *Tnodeallocate()
    Btree():root(nullptr);
    ~Btree();
};


