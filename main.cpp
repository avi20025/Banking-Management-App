#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

#include "BankAccountsClass.h"
#include "CheckingAccountsClass.h"
#include "InterestCheckingAccountsClass.h"
#include "SavingsAccountsClass.h"
#include "TransactionsClass.h"

using namespace std;

const string BANK_CONFIGURATION_FILE_NAME_STR         = "InfoBankConfig.txt";
const string BANK_ACCOUNTS_FILE_NAME_STR              = "InfoBankAccounts-Initial.txt";
const string BANK_ACCOUNTS_TRANSACTIONS_FILE_NAME_STR = "InfoBankAccountsTransactions.txt";

void CheckFileStreamOpen(string fileNameStr, ifstream& inFile);
void CheckFileStreamOpen(string fileNameStr, ofstream& outFile);
void ReadConfigurationFile(string bankConfigurationFileNameStr);
void ReadInFromBankAccountsFile
(string bankAccountsFileNameStr,
 BankAccountsClass** &bankAccountsAry, unsigned& accountsCount);
void ReadInFromTransactionsFile
(string transactionsFileNameStr,
 TransactionsClass*  &transactionsAry, unsigned& transactionsCountUns);
void ProcessTransactions
(TransactionsClass*   transactionsAry, unsigned& transactionsCountUns,
 BankAccountsClass**& bankAccountsAry, unsigned& accountsCountUns);
bool getMatchingBankAccountIndexUns(
  BankAccountsClass**& bankAccountsAry, unsigned  accountsCountUns,
  string accountIdStr, unsigned& bankAccountIndex);
void UpdateBankAccountsFile
(string   bankAccountsFileNameStr,
 BankAccountsClass**& bankAccountsAry, unsigned totalNoBankAccountsUns);

int main() 
{
  BankAccountsClass** bankAccountsAry      = nullptr;
  TransactionsClass*  transactionsAry      = nullptr;
  unsigned            accountsCountUns     = 0;
  unsigned            transactionsCountUns = 0;

  cout << setprecision(2) << fixed << showpoint;           //  set up output for dollar amounts

  ReadConfigurationFile     (BANK_CONFIGURATION_FILE_NAME_STR);
  ReadInFromBankAccountsFile(BANK_ACCOUNTS_FILE_NAME_STR, 
                             bankAccountsAry, accountsCountUns);
  ReadInFromTransactionsFile(BANK_ACCOUNTS_TRANSACTIONS_FILE_NAME_STR,
                             transactionsAry, transactionsCountUns);
  ProcessTransactions       (transactionsAry, transactionsCountUns,
                             bankAccountsAry, accountsCountUns);
  UpdateBankAccountsFile    (BANK_ACCOUNTS_FILE_NAME_STR,
                             bankAccountsAry, accountsCountUns);

} // int main()

void CheckFileStreamOpen(string fileNameStr, ifstream& inFile) 
{
  if(inFile.fail())
  {
    cout << "File " << fileNameStr << " could not be opened" << endl;
    cout << "Please press enter key once or twice to continue..." ;cin.ignore(); cin.get();
    exit(EXIT_FAILURE);
  }
  /*$$*/
}

void CheckFileStreamOpen(string fileNameStr, ofstream& outFile) 
{
  if(outFile.fail())
  {
    cout << "File " << fileNameStr << " could not be opened" << endl;
    cout << "Please press enter key once or twice to continue..." ;cin.ignore(); cin.get();
    exit(EXIT_FAILURE);
  }
  /*$$*/
}

void ReadConfigurationFile(string bankConfigurationFileNameStr) 
{
  float val;
  int size = 10;
  ifstream fileInput(bankConfigurationFileNameStr);
  CheckFileStreamOpen(bankConfigurationFileNameStr, fileInput);

  cout << "Configuration Information: " << endl <<
          "========================== " << endl << endl;

  fileInput >> val; SavingsAccountsClass:: setSavingsInterestRate(val);
  cout << "Savings Interest Rate    : " << SavingsAccountsClass:: getSavingsInterestRate() << endl;

  fileInput >> val; CheckingAccountsClass:: setCheckingMinimumFl(val);
  cout << "Checking Minimum Balance : " << CheckingAccountsClass:: getCheckingMinimumFl() << endl;

  fileInput >> val; CheckingAccountsClass:: setChargePerCheckFl(val);
  cout << "Charge Per Check         : " << CheckingAccountsClass:: getChargePerCheckFl() << endl;

  fileInput >> val; InterestCheckingAccountsClass:: setInterestCheckingRate(val);
  cout << "Interest Checking Rate   : " << InterestCheckingAccountsClass:: getInterestCheckingRate() << endl;

  fileInput >> val; InterestCheckingAccountsClass:: setMinimmunBlanceRequired(val);
  cout << "Minimum Balance          : " << InterestCheckingAccountsClass:: getMinimmunBlanceRequired() << endl;

  fileInput >> val; InterestCheckingAccountsClass:: setMonthlyFeeChargeNoMinimumBalance(val);
  cout << "Monthly Fee              : " << InterestCheckingAccountsClass:: getMonthlyFeeChargeNoMinimumBalance() << endl;

  cout << endl << endl;

  fileInput.close();
  /*$$*/
}

void ReadInFromBankAccountsFile
(string bankAccountsFileNameStr, 
 BankAccountsClass** &bankAccountsAry, unsigned& accountsCountUns) {
    string lineFileBuffer;
    constexpr char COMMA_DELIMETER = ',';
    string   accountIdStr, accountOwnerStr, accountBalanceStr;
    float    accountBalanceFl;

    ifstream inFile(bankAccountsFileNameStr);
    CheckFileStreamOpen(bankAccountsFileNameStr, inFile);

    accountsCountUns = 0;
    while(getline(inFile, lineFileBuffer))
    ++accountsCountUns;

    bankAccountsAry = new BankAccountsClass*[accountsCountUns];

    inFile.clear(); inFile.seekg(0, ios::beg);

    cout << "Accounts: " << endl <<
            "========= " << endl << endl;

    
    for(unsigned x = 0; x < accountsCountUns; x++) 
    {
        getline(inFile, lineFileBuffer);
        istringstream isStringStream(lineFileBuffer);

        getline(isStringStream, accountIdStr, COMMA_DELIMETER);

        getline(isStringStream, accountOwnerStr, COMMA_DELIMETER);
        
        getline(isStringStream, accountBalanceStr, COMMA_DELIMETER);
        accountBalanceFl = stof(accountBalanceStr);

        switch (accountIdStr.at(0))
        {
        case 'S':
        case 's':
          bankAccountsAry[x] = new SavingsAccountsClass(accountIdStr, accountOwnerStr, accountBalanceFl);
          bankAccountsAry[x]->display();
          break;
        
        case 'C':
        case 'c':
          bankAccountsAry[x] = new CheckingAccountsClass(accountIdStr, accountOwnerStr, accountBalanceFl);
          bankAccountsAry[x]->display();
          break;

        case 'I':
        case 'i':
          bankAccountsAry[x] = new InterestCheckingAccountsClass(accountIdStr, accountOwnerStr, accountBalanceFl);
          bankAccountsAry[x]->display();
          break;
          
        default:
          cout << "*** Illegal account identification ***" << endl;
        }
    }
    inFile.close();
  /*$$*/
}//ReadInFromBankAccountsFile()

void ReadInFromTransactionsFile
(string transactionsFileNameStr,
 TransactionsClass* &transactionsAry,  unsigned& transactionsCountUns) {
    string lineFileBuffer;
    constexpr char COMMA_DELIMETER = ',';
    string   accountIdStr, accountTransactionIdStr, accountTransactionAmountStr;
    float    accountTransactionAmountFl;

    ifstream inFile(transactionsFileNameStr);
    CheckFileStreamOpen(transactionsFileNameStr, inFile);

    transactionsCountUns = 0;
    while(getline(inFile, lineFileBuffer))
    ++transactionsCountUns;

    transactionsAry = new TransactionsClass[transactionsCountUns]; 

    inFile.clear(); inFile.seekg(0, ios::beg);

    cout << "Transactions: " << endl <<
            "============= " << endl << endl;

    
    for(unsigned x = 0; x < transactionsCountUns; x++) 
    {
        getline(inFile, lineFileBuffer);
        istringstream isStringStream(lineFileBuffer);

        getline(isStringStream, accountIdStr, COMMA_DELIMETER);
        transactionsAry[x].setIdentifcationStr(accountIdStr);

        getline(isStringStream, accountTransactionIdStr, COMMA_DELIMETER);
        transactionsAry[x].setTransactionIdStr(accountTransactionIdStr);

        if(accountTransactionIdStr != "interest")
        {
          getline(isStringStream, accountTransactionAmountStr, COMMA_DELIMETER);
          accountTransactionAmountFl = stof(accountTransactionAmountStr);
          transactionsAry[x].setTransactionArgumentFl(accountTransactionAmountFl);
        }
        transactionsAry[x].displayTransaction();
    }
    cout << endl;
    inFile.close();
  /*$$*/
}//ReadInFromTransactionsFile

void ProcessTransactions
(TransactionsClass   *transactionsAry, unsigned& transactionsCountUns,
 BankAccountsClass** &bankAccountsAry, unsigned& accountsCountUns) 
{
   string   accountIdStr, accountTransactionIdStr, accountTransactionAmountStr;
   float    accountTransactionAmountFl;
   unsigned index;

   cout << "Process Transactions:" << endl <<
       "=====================" << endl << endl;;

   for(unsigned count = 0; count < transactionsCountUns; ++count)
   {
     accountIdStr = transactionsAry[count].getIdentifcationStr();
     if(!getMatchingBankAccountIndexUns(bankAccountsAry, accountsCountUns, accountIdStr, index))
     {
       cout <<
        "*** Account identification not found " << endl <<
        "Account Id: " << accountIdStr << endl << endl;
       return;
     }

     accountTransactionIdStr = transactionsAry[count].getTransactionIdStr();

     switch(accountIdStr.at(0))
     {
        case 'S':
        case 's':
        
        cout << "Account before transaction:" << endl <<
                "===========================" << endl;
        static_cast<SavingsAccountsClass*>(bankAccountsAry[index])->display();

        transactionsAry[count].displayTransaction();

        if(accountTransactionIdStr == "deposit")
        {
          accountTransactionAmountFl = transactionsAry[count].getTransactionArgumentFl();
          bankAccountsAry[index]->deposit(accountTransactionAmountFl);
        }

        else if(accountTransactionIdStr == "withdraw")
        {
          accountTransactionAmountFl = transactionsAry[count].getTransactionArgumentFl();
          static_cast<SavingsAccountsClass*>(bankAccountsAry[index])->savingsWithdraw(accountTransactionAmountFl);
        }

        else if(accountTransactionIdStr == "interest")
        {
          static_cast<SavingsAccountsClass*>(bankAccountsAry[index])->calculateInterest();
        }

        else
        {
          cout << "Invalid Transaction Id: " << accountTransactionIdStr << endl << endl;
        }

        cout << "Account after transaction" << endl <<
                "=========================" << endl;
        static_cast<SavingsAccountsClass*>(bankAccountsAry[index])->display();

        break;

        case 'C':
        case 'c':

          cout << "Account before transaction" << endl <<
                  "==========================" << endl;
          static_cast<CheckingAccountsClass*>(bankAccountsAry[index])->display();

          transactionsAry[count].displayTransaction();

          if(accountTransactionIdStr == "deposit")
          {
            accountTransactionAmountFl = transactionsAry[count].getTransactionArgumentFl();
            accountTransactionAmountFl = bankAccountsAry[index]->getAccountBalanceFl() + accountTransactionAmountFl;
            bankAccountsAry[index]->setAccountBalanceFl(accountTransactionAmountFl);
          } 

          else if(accountTransactionIdStr == "check")
          {
            accountTransactionAmountFl = transactionsAry[count].getTransactionArgumentFl();
            static_cast<CheckingAccountsClass*>(bankAccountsAry[index])->cashCheck(accountTransactionAmountFl);
          }

          cout << "Account after transaction" << endl <<
                  "=========================" << endl;
          static_cast<CheckingAccountsClass*>(bankAccountsAry[index])->display();

        break;

        case 'I':
        case 'i':

            cout << "Account before transaction" << endl <<
                     "==========================" << endl;
            static_cast<InterestCheckingAccountsClass*>(bankAccountsAry[index])->display();

            transactionsAry[count].displayTransaction();

            if(accountTransactionIdStr == "deposit")
            {
              accountTransactionAmountFl = transactionsAry[count].getTransactionArgumentFl();
              accountTransactionAmountFl = bankAccountsAry[index]->getAccountBalanceFl() + accountTransactionAmountFl;
              bankAccountsAry[index]->setAccountBalanceFl(accountTransactionAmountFl);
            }

            else if(accountTransactionIdStr == "check")
            {
              accountTransactionAmountFl = transactionsAry[count].getTransactionArgumentFl();
              static_cast<InterestCheckingAccountsClass*>(bankAccountsAry[index])->cashCheck(accountTransactionAmountFl);
            }

            else if(accountTransactionIdStr == "interest")
            {
              static_cast<InterestCheckingAccountsClass*>(bankAccountsAry[index])->calculateInterest();
            }

            cout << "Account after transaction" << endl <<
                    "=========================" << endl;
            static_cast<InterestCheckingAccountsClass*>(bankAccountsAry[index])->display();
        break;

        default:

            cout <<
              "*** Illegal account identification ***" << endl <<
              "Account Id        : " << accountIdStr << endl << endl;
     }
   }  
    
  /*$$*/
}//ProcessTransactions

bool getMatchingBankAccountIndexUns(
BankAccountsClass** &bankAccountsAry, unsigned  accountsCountUns, 
string              accountIdStr,     unsigned& bankAccountIndex) {
  bankAccountIndex = 0;
  for (; bankAccountIndex < accountsCountUns; ++bankAccountIndex)
    if (bankAccountsAry[bankAccountIndex]->getAcountIdStr() == accountIdStr) {
      return true;
    }
  return false;
}

void UpdateBankAccountsFile
(string bankAccountsFileNameStr,
 BankAccountsClass**& bankAccountsAry, unsigned accountsCountUns) {

  ofstream outFile(bankAccountsFileNameStr);
  CheckFileStreamOpen(bankAccountsFileNameStr, outFile);

  cout << "Updating " << bankAccountsFileNameStr << endl <<
          "-----------------------------" << endl;

  for (unsigned writeIndex = 0; writeIndex < accountsCountUns; ++writeIndex)
    outFile <<
    bankAccountsAry[writeIndex]->getAcountIdStr()      << "," <<
    bankAccountsAry[writeIndex]->getAccountOwnerStr()  << "," <<
    bankAccountsAry[writeIndex]->getAccountBalanceFl() << endl;

  outFile.close();
};

