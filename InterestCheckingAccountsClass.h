#ifndef INTEREST_CHECKING_ACCOUNTS_CLASS_H
#define INTERESTCHECKING_ACCOUNTS_CLASS_H

#include <iostream>
#include <iomanip>
#include <string>

#include "CheckingAccountsClass.h"

using namespace std;

class InterestCheckingAccountsClass : public CheckingAccountsClass {

protected:
  static float interestCheckingRate;              // annual interest rate earned when bal > minbal credited monthly
  static float minimmunBalanceRequired;           // minimum balance required to receive interest
  static float monthlyFeeChargeNoMinimumBalance;  // monthly fee (only charged if minimum balance not met)

public:
  InterestCheckingAccountsClass(
    string accountIdStr, string accountNameStr, float accountBalanceFl)
  : CheckingAccountsClass(accountIdStr,         accountNameStr,  accountBalanceFl) { }

  static void  setInterestCheckingRate(float interestCheckingRat)
  { interestCheckingRate = interestCheckingRat;/*$$*/ }
  static float getInterestCheckingRate()
  { return interestCheckingRate; /*$$*/ }

  static void  setMinimmunBlanceRequired(float minimmunBalance)
  { minimmunBalanceRequired = minimmunBalance;/*$$*/ }
  static float getMinimmunBlanceRequired()
  {
    /*$$*/ return minimmunBalanceRequired;
  }

  static void  setMonthlyFeeChargeNoMinimumBalance(float monthlyFeeChargeNoMinimumBal)
  { monthlyFeeChargeNoMinimumBalance = monthlyFeeChargeNoMinimumBal;/*$$*/ }
  static float getMonthlyFeeChargeNoMinimumBalance()
  { /*$$*/ return monthlyFeeChargeNoMinimumBalance; }

  void calculateInterest();
  void display();

};
#endif