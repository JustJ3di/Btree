#pragma once

#include <memory>
#include <array>
#include <algorithm>



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
    //TNode *parent;

    inline BtreeNode():isleaf(true),current_key_number(0){
        std::fill(children.begin(), children.end(), nullptr);
    }
    ~BtreeNode() = default;

    int searchKey(const Key&);

};

