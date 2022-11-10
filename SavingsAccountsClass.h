#ifndef SAVINGS_ACCOUNTS_CLASS_H
#define SAVINGS_ACCOUNTS_CLASS_H

#include <iostream>
#include <iomanip>
#include <string>

#include "BankAccountsClass.h"

using namespace std;

class SavingsAccountsClass : public BankAccountsClass {
protected:
  static float savingsInterestRate;

public:
  SavingsAccountsClass(string accountIdStr, string accountOwnerStr, float accountBalanceFl)
  : BankAccountsClass(accountIdStr, accountOwnerStr, accountBalanceFl) 
  { 

  }

  static void  setSavingsInterestRate(float savingsInterest)
  { savingsInterestRate = savingsInterest;/*$$*/ }

  static float getSavingsInterestRate()
  {
    /*$$*/ return savingsInterestRate;
  }

  void        calculateInterest();
  bool        savingsWithdraw(float);
  void        display();
};

#endif
