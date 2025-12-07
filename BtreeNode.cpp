#include "BtreeNode.hpp"

template <typename T, uint16_t M>
bool BtreeNode<T, M>::isvalid() const
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

template <typename T, uint16_t M>
int BtreeNode<T, M>::searchKey(const T& key)
{
    auto start = keys.begin();
    auto end = keys.begin() + current_key_number;

    auto index = std::binary_search(start, end, key);

    //GET THE INDEX
    return std::distance(start, index);

    return 0;
}
