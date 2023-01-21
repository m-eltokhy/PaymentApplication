#include <stdio.h>
#include"Card/card.h"
#include"Terminal/terminal.h"
#include"Server/server.h"
#include"Application/app.h"


int main() {
	//test functions for card file
	//getCardHolderNameTest();
	//getCardExpiryDateTest();
	//getCardPANTest();

	// Test functions for terminal file
	//getTransactionDateTest();
	//isCardExpriedTest();
	//getTransactionAmountTest();
	//isBelowMaxAmountTest();
	//setMaxAmountTest();
	//isValidCardPANTest();

	// Test functions for server file
	//recieveTransactionDataTest();
	//isValidAccountTest();
	//isBlockedAccountTest();
	//isAmountAvailableTest();
	//saveTransactionTest();
	//listSavedTransactionsTest();


	// Application
	appStart();
	return 0;
}