#include "BtreeNode.hpp"

template <typename T, uint16_t M, typename Allocator> 
bool BtreeNode<T, M, Allocator>::isvalid()
{
    const uint16_t MIN_KEY_NO_ROOT = (M + 1)/2 - 1;
    if (current_key_number > MAX_KEY)
    {
        return false;
    }
    if (parent == nullptr) // Special case is root
    {
        return current_key_number >= 1; 
    }
    return current_key_number >= MIN_KEY_NO_ROOT;
}