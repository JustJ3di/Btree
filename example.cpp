#include <iostream>
#include "Btree.hpp"

int main()
{
    // M = 4 significa che ogni nodo può avere al massimo 3 chiavi.
    // Alla 4^ chiave, deve avvenire uno split.
    Btree<int, std::string, 4> btree; 

    std::cout << "--- Fase 1: Riempimento Radice ---" << std::endl;
    btree.insert(10, "dieci");
    btree.insert(20, "venti");
    btree.insert(30, "trenta");

    // A questo punto la radice è piena: [10, 20, 30]
    auto* check = btree.search(20);
    if(check) std::cout << "Check pre-split ok: " << *check << std::endl;

    std::cout << "\n--- Fase 2: Lo Split ---" << std::endl;
    // Questo inserimento deve causare lo split della radice!
    // La mediana (20) dovrebbe salire nella nuova radice.
    // 10 va a sinistra, 30 e 40 vanno a destra (o viceversa in base alla logica esatta).
    btree.insert(40, "quaranta");

    std::cout << "Inserimento post-split completato." << std::endl;

    // Verifichiamo se troviamo tutto
    int keys_to_find[] = {10, 20, 30, 40};
    bool all_good = true;

    for(int k : keys_to_find) {
        auto* res = btree.search(k);
        if (res) {
            std::cout << "Trovato " << k << ": " << *res << std::endl;
        } else {
            std::cerr << "ERRORE: Persa la chiave " << k << " dopo lo split!" << std::endl;
            all_good = false;
        }
    }

    if(all_good) {
        std::cout << "\nSUCCESS: Lo split funziona! L'albero è cresciuto in altezza." << std::endl;
    }

    return 0;
}