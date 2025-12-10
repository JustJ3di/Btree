#include "BtreeNode.hpp"
/*
    La ricerca nella chiave nel nodo può avere due risultati:
    La chiave è nel nodo quindi ritorno l'indice dell'elemento cercato.
    La chiave non è nel nodo quindi ritorno l'indice al figlio .
    La ricerca è effettuata tramite la funcione lower_bound che esegue una ricerca binaria per il primo elemento >= key.
    Il corretto ritorno è dato da distance che ritorna l'indice esatto dell'array di key.

*/
template <typename Key,typename Value, uint16_t M>
int BtreeNode<Key, Value, M>::searchKey(const Key& key)
{
    auto it = std::lower_bound(keys.begin(), keys.begin()+current_key_number, key); //return the iterator to the first element >= key
    //GET THE INDEX
    //in un array<> è una semplice sottrazione di puntatori.
    //Ritorna l'indice corrispondente all'iteratore ottenuto da lower_bound
    return std::distance(keys.begin(), it);
}
