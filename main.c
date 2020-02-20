#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "account.h"
#include "transaction.h"
#include <unistd.h>

Transaction clientList[1000][1000];
Account accList[1000];
int clientCount = 0;
int accCount = 0;
int threadID = 0;
pthread_mutex_t lock;

//Thread function, assigns a thread an ID based on the order theyre created, then they take the list of transactions at the index
//of their ID. Will simulatanously parse transaction information and find the proper account from the account List, however
//mutex lock will be used before a transaction is performed.
void* runClient(void* ptr){
    int myID = threadID;
    threadID++;

    Transaction* myTransactions = clientList[myID];
    int transactionCount = 0;

    while(myTransactions[transactionCount] != NULL){
        Account acc1;
        Account acc2;

        for(int i = 0; i < accCount; i++){
            if(strcmp(myTransactions[transactionCount]->account1, accList[i]->name) == 0){
                acc1 = accList[i];
                if(myTransactions[transactionCount]->type == 't'){
                    for(int j = 0; j < accCount; j++){
                        if(strcmp(myTransactions[transactionCount]->account2, accList[j]->name) == 0){
                            acc2 = accList[j];
                            break;
                        }
                    }
                }
                break;
            }
        }

        pthread_mutex_lock(&lock);
        switch(myTransactions[transactionCount]->type){
            case 't':
                transfer(acc1, acc2, myTransactions[transactionCount]->amount);
                break;

            case  'w':
                withdraw(acc1, myTransactions[transactionCount]->amount);
                break;

            case 'd':
                deposit(acc1, myTransactions[transactionCount]->amount);
                break;
        }
        transactionCount++;
        pthread_mutex_unlock(&lock);
    }
}

//Home made string copy method for use in creating unique strings for objects
char* StringCopy(char* s){
    char* ptr = malloc(((strlen(s) +1) * sizeof(char)));
    char* traverse = s;
    int i = 0;
    while(*traverse != '\0'){
        ptr[i] = *traverse;
        i++;
        traverse++;
    }
    ptr[i] = '\0';

    return ptr;
}

//Reads an account line, creating an account structure based on what it reads and storing it in the account list
Account accountRead(FILE* fp){
    char s[100];
    fgets(s, 100, fp);
    char name[55];
    char type[55];
    int dfee;
    int wfee;
    int tfee;
    int trn;
    int trnfee;
    char overdraft;
    int overfee;

    sscanf(s, "%s %*s %s %*c %d %*c %d %*c %d %*s %d %d %*s %c ",name, type, &dfee, &wfee, &tfee, &trn, &trnfee, &overdraft);

    if(overdraft == 'Y'){
        sscanf(s, "%*s %*s %*s %*c %*d %*c %*d %*c %*d %*s %*d %*d %*s %*c %d", &overfee);

        return accountGen(StringCopy(name), StringCopy(type), dfee, wfee, tfee, trn, trnfee, 0, overfee);
    }

    return accountGen(StringCopy(name), StringCopy(type), dfee, wfee, tfee, trn, trnfee, -1, 0);

}

//For reading a deposit instruction
void depositRead(FILE* fp){
    fscanf(fp, "%*s");
    int c;

    while((c = getc(fp)) == 'd' || c == ' ') {
        ungetc(c,fp);

        fscanf(fp, " %*s");

        char accName[50];
        int amount = 0;

        fscanf(fp, " %s %d", accName, &amount);

        for(int i = 0;; i++){
            if(strcmp(accList[i]->name, accName) == 0){
                deposit(accList[i], amount);
                break;
            }
        }
    }
    return;
}

//Reading a client input, reads all the transactions in a line and creats structures for them
void clientRead(FILE* fp){
    fscanf(fp, "%*s ");
    int tcount = 0;

    int c = fgetc(fp);
    while(c == 'w' || c == 't' || c == 'd') {

        if(c == 't'){
            char acc1[50];
            char acc2[50];
            int amount;

            fscanf(fp, " %s %s %d", acc1, acc2, &amount);
            clientList[clientCount][tcount] = tGen(c, StringCopy(acc1), StringCopy(acc2), amount);
            tcount++;
        }

        else{
            char acc[50];
            int amount;
            fscanf(fp, " %s %d", acc, &amount);
            clientList[clientCount][tcount] = tGen(c, StringCopy(acc), NULL, amount);
            tcount++;
        }
        c = fgetc(fp);
        if(c == EOF){
            break;
        }
        if(c == ' '){
            c = fgetc(fp);
        }
    }
    return;
}

int main() {
    FILE* fp = fopen("assignment_3_input_file.txt", "r");

    //Nulling my arrays
    for(int i = 0; i<1000; i++){
        accList[i] = NULL;
        for(int j = 0; j<1000; j++){
            clientList[i][j] = NULL;
        }
    }
    char c;

    //Outer loop checking for end of file
    while((c = fgetc(fp)) != EOF){

        switch(c){
            //If account input
            case 'a':
                ungetc(c, fp);
                accList[accCount] = accountRead(fp);
                accCount++;
                break;

            //If deposit input
            case 'd':
                ungetc(c, fp);
                depositRead(fp);
                break;

            //If client input
            case 'c':
                ungetc(c, fp);
                clientRead(fp);
                clientCount++;
                break;
        }
    }


    //Creating, running and waiting for threads
    pthread_t store[1000];

    for(int i = 0; i < clientCount; i++){
        pthread_create(&store[i], NULL, runClient, NULL);
    }

    for(int i = 0; i < clientCount; i++){
        pthread_join(store[i], NULL);
    }

    //Outputting to file and console
    int accCount = 0;
    FILE * ofp = fopen("assignment_3_output_file.txt", "w");
    while(accList[accCount] != NULL){
        printf("%s type %s %d \n", accList[accCount]->name, accList[accCount]->type, accList[accCount]->balance);
        fprintf(ofp,"%s type %s %d \n", accList[accCount]->name, accList[accCount]->type, accList[accCount]->balance);
        accCount++;
    }
    fclose(fp);
    fclose(ofp);

    return 0;
}