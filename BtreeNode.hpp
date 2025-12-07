#pragma once

#include <memory>
#include <array>
#include <algorithm>

struct BtreeNode;

template<typename T,
        uint16_t M = 256, 
        typename Allocator = std::allocator<T>>
struct BtreeNode
{

    //Alias
    using TNode = BtreeNode<T,M,Allocator>;

    const static uint16_t MAX_KEY = M - 1;
    std::array<T,MAX_KEY> keys;
    std::array<TNode* ,M> children;

    uint16_t current_key_number;
    bool isleaf;

    TNode *parent;

    inline BtreeNode():isleaf(true),parent(nullptr),current_key_number(0){
        std::fill(children.begin(), children.end(), nullptr);
    }
    ~BtreeNode() = default;
    /* Is valid BtreeNode if current_key_number is <= MAX_KEY and >= [MAX/KEY]-1 
    *  Or if is the root and cuurrent_key_number may be 1
    */ 
    bool isvalid();
    
    static TNode* AllocateNode(Allocator& a) {
        
        //Allocator standard
        using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<TNode>;
        NodeAlloc nodeAlloc(a); 
        
        TNode* newNode = nodeAlloc.allocate(1);
        std::allocator_traits<NodeAlloc>::construct(nodeAlloc, newNode);
        
        return newNode;
    }

    static void DeallocateNode(Allocator& a, TNode* node) {
        // 1. REBIND 
        using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<TNode>;
        NodeAlloc nodeAlloc(a);
 
        std::allocator_traits<NodeAlloc>::destroy(nodeAlloc, node);
        nodeAlloc.deallocate(node, 1);
    }

};

