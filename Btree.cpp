#include "Btree.hpp"


template<typename Key, 
        typename Value,        
        uint16_t M, 
        typename Allocator>
Btree<T, M, Allocator>::~Btree()
{
}   



template<typename Key,
        typename Value,
        uint16_t M ,
        typename Allocator>
const Value *Btree<Key,Value,M,Allocator>::search(const Key &k)const{

    const TNode* current = root;

    while(current != nullptr){
        int i = current->searchKey(k);
        if(i < current->current_key_number && current->keys[i] == k)
            return &current->values[i];
        if(current->isleaf)
            return nullptr;
        current = current->children[i];
    }
    return nullptr;

}

template<typename Key,
        typename Value,
        uint16_t M ,
        typename Allocator>
void Btree<Key,Value,M,Allocator>::insert(const Key &k, const Value &v){

}
