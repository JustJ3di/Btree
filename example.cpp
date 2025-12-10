#include "Btree.hpp"

#include <stdio.h>
#include <string>

int main(int argc, char const *argv[])
{
    Btree<int, std::string>btree;

    btree.insert(10,"coap");

    return 0;
}
