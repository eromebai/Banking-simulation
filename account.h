
#ifndef ASSIGNMENT3_ACCOUNT_H
#define ASSIGNMENT3_ACCOUNT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *name;
    char *type;
    int balance;
    int depFee;
    int withFee;
    int transferFee;
    int transactions;
    int transactionFee;
    int overdraft;
    int overFee;

}account_struct;

typedef account_struct* Account;

Account accountGen(char*, char*, int, int, int, int, int, int, int);

int withdraw(Account, int);

void deposit(Account, int);

int transfer(Account, Account, int);



#endif //ASSIGNMENT3_ACCOUNT_H
