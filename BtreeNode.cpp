#include "BtreeNode.hpp"

template <typename T, uint16_t M>
bool BtreeNode<T, M>::isvalid()
{
    const uint16_t MIN_KEY_NO_ROOT = (M + 1)/2 -1;
    if (current_key_number > MAX_KEY)//IS VALID FOR ALL NODE
    {
        return false;
    }

    if (parent == nullptr) //is the root node
    {
        return current_key_number >= 1;
        
    }
    
    return current_key_number >= MIN_KEY_NO_ROOT;
}


template <typename T, uint16_t M, typename Allocator>
typename BtreeNode<T, M, Allocator>::TNode* BtreeNode<T, M, Allocator>::AllocateNode(Allocator& a) // Accetta l'istanza dell'allocatore
{
    using TNode = BtreeNode<T, M, Allocator>::TNode;

    TNode *newNode = a.allocate(1);
    
    a.construct(newNode); 
    
    return newNode;
}

template <typename T, uint16_t M, typename Allocator>
void BtreeNode<T, M, Allocator>::DeallocateNode(Allocator &a, TNode *node)
{
    a.destroy(node);
    a.deallocate(node, 1);
}
