#include "app.h"

void appStart(void) {
	ST_cardData_t user;
	ST_terminalData_t transaction;
	ST_accountsDB_t accountRef;
	printf("Welcome\n");
	printf("-------\n");
	printf("Please enter Card Holder Name:\n");
	gets(user.cardHolderName);
	if (getCardHolderName(&user)) {
		printf("\n-------------------------------\n");
		printf("\nDeclined Wrong Card Holder Name\n");
		printf("\n-------------------------------\n");
		return;
	}
	printf("Please enter Card Expiration Date:\n");
	gets(user.cardExpirationDate);
	printf("Please enter Primary Account Number:\n");
	gets(user.primaryAccountNumber);
	getTransactionDate(&transaction);
	if (isCardExpired(&user, &transaction)) {
		printf("\n---------------------\n");
		printf("\nDeclined Expired Card\n");
		printf("\n---------------------\n");
		return;
	}
	setMaxAmount(&transaction,10000.0);
	printf("Please enter Transaction Amount:\n");
	scanf_s("%f", &transaction.transAmount);
	if (isBelowMaxAmount(&transaction)) {
		printf("\n-------------------------------\n");
		printf("\nDeclined Amount Exceeding Limit\n");
		printf("\n-------------------------------\n");
		return;
	}
	if (isValidAccount(&user, &accountRef)) {
		printf("\n------------------------\n");
		printf("\nDeclined Invalid Account\n");
		printf("\n------------------------\n");
		return;
	}
	if (isBlockedAccount(&accountRef)) {
		printf("\n--------------------\n");
		printf("\nDeclined Stolen Card\n");
		printf("\n--------------------\n");
		return;
	}
	if (isAmountAvailable(&transaction, &accountRef)) {
		printf("\n--------------------------\n");
		printf("\nDeclined Insuffecient Fund\n");
		printf("\n--------------------------\n");
		return;
	}
	ST_transaction_t transData = {.cardHolderData=user,.terminalData=transaction};
	accountRef.balance -= transaction.transAmount;
	saveTransaction(&transData);
	listSavedTransactions();
	return;
}