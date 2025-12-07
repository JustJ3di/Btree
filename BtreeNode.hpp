#pragma once

#include <memory>
#include <array>
#include <algorithm>

template<typename T,uint16_t M = 256>
struct BtreeNode
{
    const static uint16_t MAX_KEY = M - 1;
    std::array<T,MAX_KEY> keys;
    std::array<BtreeNode<T,M>* ,M> children;

    uint16_t current_key_number;
    bool isleaf;

    BtreeNode <T,M> *parent;

    BtreeNode():isleaf(true),parent(nullptr),current_key_number(0){
        std::fill(children.begin(), children.end(), nullptr);
    }
    ~BtreeNode() = default;
    /* Is valid BtreeNode if current_key_number is <= MAX_KEY and >= [MAX/KEY]-1 
    *  Or if is the root and cuurrent_key_number may be 1
    */ 
    inline bool isvalid(){ return (current_key_number <= MAX_KEY && current_key_number >= MAX_KEY/2 - 1) || (parent == nullptr || current_key_number >=1 && current_key_number<=MAX_KEY);}
};

