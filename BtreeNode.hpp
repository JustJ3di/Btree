#pragma once

#include <memory>
#include <array>
#include <algorithm>

struct BtreeNode;

template<typename Key,
        typename Value,
        uint16_t M = 256>
struct BtreeNode
{

    //Alias
    using TNode = BtreeNode<Key,Value,M>;
    
    const static uint16_t MAX_KEY = M - 1;
    std::array<Key,MAX_KEY> keys;
    std::array<Value,MAX_KEY> values;
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
    bool isvalid() const;

    int searchKey(const T&);

};

