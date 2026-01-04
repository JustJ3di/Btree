#include "Btree.hpp"


template <typename Key,
          typename Value,
          uint16_t M,
          typename Allocator>
const Value *Btree<Key, Value, M, Allocator>::search(const Key &k) const
{

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
        uint16_t M,
        typename Allocator>
Value *Btree<Key, Value, M, Allocator>::search(const Key &k)
{
    TNode* current = root;

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

    //caso già presente
    Value *tmp = search(k); 
    if(tmp != nullptr){
        *tmp = v;
        return;
    }
    if(root == nullptr){
        root = createNode(true);
        root->keys[0] = k;
        root->values[0] = v;
        root->current_key_number = 1;
        return;
    }
    //La radice è piena -> split
    if (root->current_key_number == root->MAX_KEY)
    {
        //new root
        TNode* newRoot = createNode(false);
        //primo figlio sinistro è il root
        newRoot->children[0] = root;
        
        //figlio destro
        splitChild(newRoot, 0, root);
        
        int i = 0;
        if (newRoot->keys[0] < k) {
            i++;
        }
        insertNonFull(newRoot->children[i], k, v);
        
        root = newRoot;
    }else{
        insertNonFull(root,k,v);
    }
    
}


//split preventivo
//x è il nuovo nodo genitore mentre y è il vecchio
template<typename Key,
        typename Value, 
        uint16_t M, 
        typename Allocator>
void Btree<Key, Value, M, Allocator>::splitChild(TNode *x, int i , TNode *y)
{
    TNode* z = createNode(y->isleaf);
    
    // 't' (minimum degree) grado minimo del nodo 
    int t = (y->MAX_KEY + 1) / 2;

    // assegno a z il giusto numero di key
    z->current_key_number = t - 1;
    for (int j = 0; j < t - 1; j++) {
        z->keys[j] = y->keys[j + t];
        z->values[j] = y->values[j + t];
    }
    if (!y->isleaf) {
        for (int j = 0; j < t; j++) {
            z->children[j] = y->children[j + t];
        }
    }

    // Riduco il vecchio nodo.
    y->current_key_number = t - 1;
    //copio i figli nella posizone nuova
    for (int j = x->current_key_number; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }
    x->children[i + 1] = z;

    // copio chiavi e valori. nella  nuova posizione
    for (int j = x->current_key_number - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
        x->values[j + 1] = x->values[j];
    }

    // set the new value
    x->keys[i] = y->keys[t - 1];
    x->values[i] = y->values[t - 1];
    
    x->current_key_number += 1;
}


//inserisce nel nodo se questo non è pieno.
template <typename Key,
          typename Value,
          uint16_t M,
          typename Allocator>
void Btree<Key, Value, M, Allocator>::insertNonFull(TNode *x, const Key &k, const Value &v)
{

    int i = x->current_key_number - 1;

    if (x->isleaf) {
        while (i >= 0 && k < x->keys[i]) {
            x->keys[i + 1] = x->keys[i];
            x->values[i + 1] = x->values[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->values[i + 1] = v;
        x->current_key_number += 1;

    } else {
        while (i >= 0 && k < x->keys[i]) {
            i--;
        }
        i++;

        if (x->children[i]->current_key_number == x->children[i]->MAX_KEY) {
            
            splitChild(x, i, x->children[i]);

            if (k > x->keys[i]) {
                i++;
            }
        }
        insertNonFull(x->children[i], k, v);
    }
}

template<typename Key,
        typename Value,
        uint16_t M,
        typename Allocator>
void Btree<Key, Value, M, Allocator>::removeFromLeaf(TNode *node, int idx) {

    for (int i = idx + 1; i < node->current_key_number; ++i) {
        node->keys[i - 1] = node->keys[i];
        node->values[i - 1] = node->values[i];
    }
    node->current_key_number--;
}

template<typename Key, typename Value, uint16_t M, typename Allocator>
void Btree<Key, Value, M, Allocator>::fill(TNode* node, int idx) {
    int t = (node->MAX_KEY + 1) / 2;


    if (idx != 0 && node->children[idx - 1]->current_key_number >= t) {
        borrowFromPrev(node, idx);
    }

    else if (idx != node->current_key_number && node->children[idx + 1]->current_key_number >= t) {
        borrowFromNext(node, idx);
    }

    else {
        if (idx != node->current_key_number) {
            merge(node, idx);
        } else {
            merge(node, idx - 1);
        }
    }
}
template<typename Key, typename Value, uint16_t M, typename Allocator>
void Btree<Key, Value, M, Allocator>::merge(TNode* node, int idx) {
    TNode* child = node->children[idx];
    TNode* sibling = node->children[idx + 1];
    int t = (node->MAX_KEY + 1) / 2;


    child->keys[t - 1] = node->keys[idx];
    child->values[t - 1] = node->values[idx];

    for (int i = 0; i < sibling->current_key_number; ++i) {
        child->keys[i + t] = sibling->keys[i];
        child->values[i + t] = sibling->values[i];
    }


    if (!child->isleaf) {
        for (int i = 0; i <= sibling->current_key_number; ++i) {
            child->children[i + t] = sibling->children[i];
        }
    }


    child->current_key_number += sibling->current_key_number + 1;

    for (int i = idx; i < node->current_key_number - 1; ++i) {
        node->keys[i] = node->keys[i + 1];
        node->values[i] = node->values[i + 1];
        node->children[i + 1] = node->children[i + 2];
    }

    node->current_key_number--;
    freeNode(sibling);
}
template<typename Key, typename Value, uint16_t M, typename Allocator>
void Btree<Key, Value, M, Allocator>::borrowFromPrev(TNode* node, int idx) {
    TNode* child = node->children[idx];
    TNode* sibling = node->children[idx - 1];


    for (int i = child->current_key_number - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
        child->values[i + 1] = child->values[i];
    }
    if (!child->isleaf) {
        for (int i = child->current_key_number; i >= 0; --i) {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = node->keys[idx - 1];
    child->values[0] = node->values[idx - 1];

    if (!child->isleaf) {
        child->children[0] = sibling->children[sibling->current_key_number];
    }


    node->keys[idx - 1] = sibling->keys[sibling->current_key_number - 1];
    node->values[idx - 1] = sibling->values[sibling->current_key_number - 1];

    child->current_key_number += 1;
    sibling->current_key_number -= 1;
}

template<typename Key, typename Value, uint16_t M, typename Allocator>
void Btree<Key, Value, M, Allocator>::borrowFromNext(TNode* node, int idx) {
    TNode* child = node->children[idx];
    TNode* sibling = node->children[idx + 1];


    child->keys[child->current_key_number] = node->keys[idx];
    child->values[child->current_key_number] = node->values[idx];

    if (!child->isleaf) {
        child->children[child->current_key_number + 1] = sibling->children[0];
    }


    node->keys[idx] = sibling->keys[0];
    node->values[idx] = sibling->values[0];

    for (int i = 1; i < sibling->current_key_number; ++i) {
        sibling->keys[i - 1] = sibling->keys[i];
        sibling->values[i - 1] = sibling->values[i];
    }
    if (!sibling->isleaf) {
        for (int i = 1; i <= sibling->current_key_number; ++i) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->current_key_number += 1;
    sibling->current_key_number -= 1;
}

template <typename Key,
          typename Value,
          uint16_t M,
          typename Allocator>
void Btree<Key, Value, M, Allocator>::removeInternal(TNode* node, const Key& k) {


    int idx = node->searchKey(k);

    if (idx < node->current_key_number && node->keys[idx] == k) {


        if (node->isleaf) {

            removeFromLeaf(node, idx);
        } else {

            removeFromNonLeaf(node, idx);
        }

    } else {

        if (node->isleaf) {
            return;
        }

        int t = (node->MAX_KEY + 1) / 2;


        bool flag = (idx == node->current_key_number);


        if (node->children[idx]->current_key_number < t) {
            fill(node, idx);
        }


        if (flag && idx > node->current_key_number) {
            removeInternal(node->children[idx - 1], k);
        } else {
            removeInternal(node->children[idx], k);
        }
    }
}

template<typename Key,
        typename Value,
        uint16_t M,
        typename Allocator>
void Btree<Key, Value, M, Allocator>::removeFromNonLeaf(TNode *node, int idx) {
    Key k = node->keys[idx];

    int t = (node->MAX_KEY + 1) / 2;

    // --- CASE1
    if (node->children[idx]->current_key_number >= t) {


        TNode* cur = node->children[idx];
        while (!cur->isleaf) {
            cur = cur->children[cur->current_key_number];
        }

        Key predKey = cur->keys[cur->current_key_number - 1];
        Value predVal = cur->values[cur->current_key_number - 1];

        node->keys[idx] = predKey;
        node->values[idx] = predVal;


        removeInternal(node->children[idx], predKey);
    }

    // --- CASE2

    else if (node->children[idx + 1]->current_key_number >= t) {
        TNode* cur = node->children[idx + 1];
        while (!cur->isleaf) {
            cur = cur->children[0];
        }


        Key succKey = cur->keys[0];
        Value succVal = cur->values[0];
    }
}



template <typename Key,
          typename Value,
          uint16_t M,
          typename Allocator>
void Btree<Key, Value, M, Allocator>::del(const Key &k) {
    //1
    if (root == nullptr) {
        return;
    }
    //2
    if (search(k) == nullptr) {
        return;
    }
    //3
    removeInternal(root, k);

    //4
    if (root->current_key_number == 0) {
        TNode* old = root;
        if (root->isleaf) {
            root = nullptr;
        } else {
          root = root->children[0];
        }
        freeNode(old);
    }
}
