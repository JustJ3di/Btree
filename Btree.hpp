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
    TNode* createNode(bool isLeaf) {
        TNode* newNode = node_allocator.allocate(1);
        std::allocator_traits<Allocator>::construct(node_allocator, newNode, isLeaf);
        return newNode;
    }

    TNode *root;


    //utilities
    void splitChild(TNode *, int, TNode *); //split del nodo
    void insertNonFull(TNode *, const Key &, const Value &);//insert nel nodo


public:
    inline Btree():root(nullptr){};
    ~Btree();

    const Value *search(const Key&)const;
    Value *search(const Key&);

    void insert(const Key&, const Value&);

};


