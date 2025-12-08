#include "Btree.hpp"


template<typename Key, 
        typename Value,        
        uint16_t M, 
        typename Allocator>
Btree<T, M, Allocator>::~Btree()
{
}



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

template <typename Key, typename Value, uint16_t M, typename Allocator>
Value *Btree<Key, Value, M, Allocator>::search(const Key &)
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
        //figlio sinistro.
        TNode* newRoot = createNode(false);
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

//x è il nuovo nodo genitore mentre y è il vecchio
template<typename Key,
        typename Value, 
        uint16_t M, 
        typename Allocator>
void Btree<Key, Value, M, Allocator>::splitChild(TNode *x, int i , TNode *y)
{
    TNode* z = createNode(y->isleaf);
    
    // 't' (minimum degree) grado minimo del nodo 
    int t = (y->MAX_KEYS + 1) / 2;

    // assegno a z il giusto numero di key
    z->current_key_number = t - 1;

    // 1. COPIA CHIAVI/VALORI 
    for (int j = 0; j < t - 1; j++) {
        z->keys[j] = y->keys[j + t];
        z->values[j] = y->values[j + t];
    }

    // 2. COPIA FIGLI (se non è foglia)
    if (!y->isleaf) {
        for (int j = 0; j < t; j++) {
            z->children[j] = y->children[j + t];
        }
    }

    // Riduciamo y. 
    y->current_key_number = t - 1;

    // 3. FAI SPAZIO IN X PER IL PUNTATORE AL NUOVO FIGLIO Z
    for (int j = x->current_key_number; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }
    x->children[i + 1] = z;

    // 4. FAI SPAZIO IN X PER LA CHIAVE MEDIANA CHE SALE
    for (int j = x->current_key_number - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
        x->values[j + 1] = x->values[j];
    }

    // 5. PORTA SU LA CHIAVE MEDIANA

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

        if (x->children[i]->current_key_number == x->children[i]->MAX_KEYS) {
            
            splitChild(x, i, x->children[i]);

            if (k > x->keys[i]) {
                i++;
            }
        }
        insertNonFull(x->children[i], k, v);
    }
}
