#include "BtreeNode.hpp"

/*
    Funzione d'utilità per verificare se il numero di key è valido in un nodo.
*/
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


/*
    La ricerca nella chiave nel nodo può avere due risultati:
    La chiave è nel nodo quindi ritorno l'indice dell'elemento cercato.
    La chiave non è nel nodo quindi ritorno l'indice al figlio .
    La ricerca è effettuata tramite la funcione lower_bound che esegue una ricerca binaria per il primo elemento >= key.
    Il corretto ritorno è dato da distance che ritorna l'indice esatto dell'array di key.

*/
template <typename T, uint16_t M>
int BtreeNode<T, M>::searchKey(const T& key)
{
    auto start = keys.begin();
    auto end = keys.begin() + current_key_number;

    auto it = std::lower_bound(start, end, key); //return the iterator to the first element >= key

    //GET THE INDEX
    //in un array<> è una semplice sottrazione di puntatori.
    //Ritorna l'indice corrispondente all'iteratore ottenuto da lower_bound
    return std::distance(start, it);
}
