#include "transaction.h"
#include <stdlib.h>

//A structure which represents a transaction, clients will be represented by a list of these
Transaction tGen(char gtype, char* a1, char* a2, int gamount){
    Transaction t = malloc(sizeof(transaction_struct));
    t->type = gtype;
    t->account1 = a1;
    t->account2 = a2;
    t->amount = gamount;

    return t;
}