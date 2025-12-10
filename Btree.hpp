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
        std::allocator_traits<NodeAlloc>::construct(node_allocator, newNode, isLeaf);
        return newNode;
    }

    TNode *root;

    // Metodo privato
    void freeNode(TNode* node) {
        if (node == nullptr) return;
        std::allocator_traits<NodeAlloc>::destroy(node_allocator, node);
        node_allocator.deallocate(node, 1);
    }
    void deleteTreeRecursively(TNode* node) {
        if (node == nullptr) return;

        
        if (!node->isleaf) {
            for (int i = 0; i <= node->current_key_number; i++) {
                deleteTreeRecursively(node->children[i]);
            }
        }

        freeNode(node);
    }

    //utilities
    void splitChild(TNode *, int, TNode *); //split del nodo
    void insertNonFull(TNode *, const Key &, const Value &);//insert nel nodo


public:
    inline Btree():root(nullptr){};
    ~Btree(){clear();};

    const Value *search(const Key&)const;
    Value *search(const Key&);

    void insert(const Key&, const Value&);
    void clear(){
        if(root!=nullptr)
            deleteTreeRecursively(root);
        root=nullptr;
    }    

};


#include "Btree.cpp"

