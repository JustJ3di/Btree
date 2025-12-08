#pragma once
#include"BtreeNode.hpp"

template<typename Key,
        typename Value,
        uint16_t M = 256,
        typename Allocator = std::allocator<Key>>
class Btree
{

private:
    using TNode = BtreeNode<Key,Value,M>;

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

    Value *search(const Key&)const;

    void insert(const Key&, const Value&);

};


