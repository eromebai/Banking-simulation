#ifndef ASSIGNMENT3_TRANSACTION_H
#define ASSIGNMENT3_TRANSACTION_H

typedef struct {
    char type;
    char* account1;
    char* account2;
    int amount;

}transaction_struct;

typedef transaction_struct* Transaction;

Transaction tGen(char, char*, char*, int);

#endif //ASSIGNMENT3_TRANSACTION_H
