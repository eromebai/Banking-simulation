
#include "account.h"

//Structure to represent and Account, created through this method
Account accountGen(char * gname, char *gtype, int gdepFee, int gwithFee, int gtransferFee, int gtransactions, int gtransactionFee, int goverdraft, int goverdraftFee){
    Account acc = malloc(sizeof(account_struct));

    acc->name = gname;
    acc->type = gtype;
    acc->depFee = gdepFee;
    acc->withFee = gwithFee;
    acc->transferFee = gtransferFee;
    acc->transactions = gtransactions;
    acc->transactionFee = gtransactionFee;
    acc->overdraft = goverdraft;
    acc->overFee = goverdraftFee;
    acc->balance = 0;

    return acc;
}

//Function to withdraw money from a given account if there is enough, checks for overdraft and applies the fee
int withdraw(Account acc, int amount){
    if(acc->balance - amount - acc->withFee > 0){
        acc->balance -= (amount + acc->withFee);
        if(acc->transactions <= 0){
            acc->balance -= acc->transactionFee;
        }
        acc->transactions--;
        return 0;
    }
    
    else{
        if(acc->overdraft == 0){
            if((acc->balance - amount) -acc->withFee < -5000){
                return -1;
            }

            else{
                int startlevel = 0;
                int endlevel = 0;
                int counter = 0;
                int total = 0;

                while(total > -5001){
                    if(acc->balance > total){
                        startlevel = counter;
                        break;
                    }
                    total -= 500;
                    counter++;
                }

                counter = 0;
                total = 0;

                while(total > -5001) {
                    if ((acc->balance - amount) - acc->withFee > total) {
                        endlevel = counter;
                        break;
                    }
                    total -= 500;
                    counter++;
                }
                if(endlevel - startlevel == 0){
                    acc->balance -= (amount + acc->withFee);
                    if(acc->transactions <= 0){
                        acc->balance -= acc->transactionFee;
                    }
                    acc->transactions--;
                    return 0;
                }

                else if((acc->balance - amount) - (((endlevel - startlevel) * acc->overFee) - acc->withFee) > -5001){
                    acc->balance -= amount;
                    int temp = acc->withFee;
                    int temp2 = acc->transactionFee;
                    acc->withFee = 0;
                    acc->transactionFee = 0;
                    int result;
                    result = withdraw(acc, ((endlevel - startlevel) * acc->overFee) + temp);
                    acc->withFee = temp;
                    acc->transactionFee = temp2;
                    acc->transactions++;
                    if(acc->transactions <= 0){
                        acc->balance -= acc->transactionFee;
                    }
                    acc->transactions--;
                    return result;
                }

                else{
                    return -1;
                }
            }
        }
        else{
            return -1;
        }
    }
}

//Deposites given amount into a given account, if the deposit fee is higher than the amount to do deposited, it rejects
void deposit(Account acc, int amount){
    if(amount < acc->depFee){
        return;
    }

    acc->balance += (amount - acc->depFee);

    if(acc->transactions <= 0){
        acc->balance -= acc->transactionFee;
    }
    acc->transactions--;
    return;
}

//Transfers money from account from to account to, using the deposite and withdraw functions after switching the fees temporarily
int transfer(Account from, Account to, int amount){
    int acc1 = from->withFee;
    int acc2 = to->depFee;
    from->withFee = from->transferFee;
    to->depFee = to->transferFee;
    if(withdraw(from, amount) == 0){
        deposit(to, amount);
        from->withFee = acc1;
        to->depFee = acc2;
        return 0;
    }

    else{
        from->withFee = acc1;
        to->depFee = acc2;
        return -1;
    }

}