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


        node->keys[idx] = succKey;
        node->values[idx] = succVal;


        removeInternal(node->children[idx + 1], succKey);
    }


    else {

        merge(node, idx);


        removeInternal(node->children[idx], k);
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
