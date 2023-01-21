#include "server.h"


ST_accountsDB_t accountsDB[255] = {
		{20000.0, RUNNING, "8989374605436851"},
		{10000.0, BLOCKED, "5807007076043875"},
		{800000.0, RUNNING, "4357000002203005"},
		{25800.0, RUNNING, "1355698002145932"},
		{330000.0, RUNNING, "3654448001010490"},
		{500.0, RUNNING, "3669001475800021"},
		{16200.0, RUNNING, "4300214078915223"},
		{20060.0, BLOCKED, "5123054400360017"},
		{4900.0, RUNNING, "5266032598801400"},
		{722300.0, RUNNING, "55014720336952104"},
};

ST_transaction_t transactionTestCase[] = {
	{.cardHolderData = {"Mohamed khaled eltokhy","8989374605436851","05/30"},.terminalData = {1000.0,5000.0,"20/01/2023"},.transState=APPROVED},
	{.cardHolderData = {"mahmoud islam abdallah","5807007076043875","09/29"},.terminalData = {10000.0,50000.0,"20/01/2023"},.transState = DECLINED_STOLEN_CARD},
	{.cardHolderData = {"magdy ibrahim shaarawy","1355698002845932","08/25"},.terminalData = {1200.0,8000.0,"20/01/2023"},.transState = FRAUD_CARD},
	{.cardHolderData = {"hassan metwaly marzouk","3669001475800021","01/24"},.terminalData = {600.0,1000.0,"20/01/2023"},.transState = DECLINED_INSUFFECIENT_FUND}
};

ST_transaction_t transactionsDB[255] = {0};
uint8 transactionsCounter = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	for (uint8 ctr = 0; ctr < 255; ctr++) {
		if (!strcmp(transData->cardHolderData.primaryAccountNumber, accountsDB[ctr].primaryAccountNumber)) {
			if (accountsDB[ctr].state == BLOCKED) {
				return DECLINED_STOLEN_CARD;
			}
			else if (transData->terminalData.transAmount > accountsDB[ctr].balance) {
				return DECLINED_INSUFFECIENT_FUND;
			}
			else {
				if (transactionsCounter == 255) {
					return INTERNAL_SERVER_ERROR;
				}
				else {
					accountsDB[ctr].balance -= transData->terminalData.transAmount;
					return APPROVED;
				}
			}
		}
	}
	return FRAUD_CARD;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
	for (uint8 ctr = 0; ctr < 10; ctr++) {
		if (!strcmp(cardData->primaryAccountNumber, accountsDB[ctr].primaryAccountNumber)) {
			*accountRefrence = accountsDB[ctr];
			return SERVER_OK;
		}
	}
	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	if (accountRefrence->state == BLOCKED) {
		return BLOCKED_ACCOUNT;
	}
	return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
	if (termData->transAmount > accountRefrence->balance) {
		return LOW_BALANCE;
	}
	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	transactionsDB[transactionsCounter].cardHolderData = transData->cardHolderData;
	transactionsDB[transactionsCounter].terminalData = transData->terminalData;
	transactionsDB[transactionsCounter].transactionSequenceNumber = transactionsCounter+1;
	transactionsDB[transactionsCounter].transState = transData->transState;
	transactionsCounter++;
	return SERVER_OK;

}

void listSavedTransactions(void) {
	if (transactionsCounter == 0) {
		printf("\n#############\n");
		printf("\nList is Empty\n");
		printf("\n#############\n\n");
	}
	for (uint8 ctr =0 ; ctr < transactionsCounter; ctr++) {
		printf("\n#########################\n");
		printf("Transaction Sequence Number: %d \n",transactionsDB[ctr].transactionSequenceNumber);
		printf("Transaction Date: %s \n", transactionsDB[ctr].terminalData.transactionDate);
		printf("Transaction Amount: %.3f \n", transactionsDB[ctr].terminalData.transAmount);
		printf("Transaction State: ");
		switch (transactionsDB[ctr].transState) {
		case APPROVED:
			printf("Approved\n");
			break;
		case INTERNAL_SERVER_ERROR:
			printf("Internal Server Error\n");
			break;
		case DECLINED_INSUFFECIENT_FUND:
			printf("declined insuffecient fund\n");
			break;
		case DECLINED_STOLEN_CARD:
			printf("Declined stolen card\n");
			break;
		case FRAUD_CARD:
			printf("Fraud Card\n");
			break;
		}
		printf("Terminal Max Amount: %.3f \n", transactionsDB[ctr].terminalData.maxTransAmount);
		printf("Cardholder Name: %s \n", transactionsDB[ctr].cardHolderData.cardHolderName);
		printf("PAN: %s \n", transactionsDB[ctr].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s \n", transactionsDB[ctr].cardHolderData.cardExpirationDate);
		printf("\n#########################\n");
	}
}

void recieveTransactionDataTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : recieveTransactionData\n\n");
	for (uint8 n = 0; n < (sizeof(transactionTestCase) / sizeof(ST_transaction_t)); n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : PAN = %s and transaction ammount = %.3f\n",transactionTestCase[n].cardHolderData.primaryAccountNumber,transactionTestCase[n].terminalData.transAmount);
		printf("Expected Result : ");
		switch (n)
		{
		case 0:
			printf("Approved transaction \n");
			break;
		case 1:
			printf("Declined stolen card \n");
			break;
		case 2:
			printf("Fraud Card \n");
			break;
		case 3:
			printf("Declined Insuffecient Fund \n");
			break;
		default:
			break;
		}
		printf("Actual Result : ");
		switch (recieveTransactionData(&transactionTestCase[n]))
		{
		case DECLINED_STOLEN_CARD:
			printf("Declined stolen card \n");
			break;
		case DECLINED_INSUFFECIENT_FUND:
			printf("Declined Insuffecient Fund \n");
			break;
		case APPROVED:
			printf("Approved transaction \n");
			break;
		case FRAUD_CARD:
			printf("Fraud Card \n");
			break;
		default:
			break;
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}

void isValidAccountTest(void) {
	ST_accountsDB_t account;
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : isValidAccount\n\n");
	for (uint8 n = 0; n < (sizeof(transactionTestCase) / sizeof(ST_transaction_t)); n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : PAN = %s\n", transactionTestCase[n].cardHolderData.primaryAccountNumber);
		printf("Expected Result : ");
		switch (n)
		{
		case 0:
			printf("balance = 19000.0 and status of account = Running\n");
			break;
		case 1:
			printf("balance = 10000.0 and status of account = Blocked\n");
			break;
		case 2:
			printf("Account not found\n");
			break;
		case 3:
			printf("balance = 500.0 and status of account = Running\n");
			break;
		default:
			break;
		}
		printf("Actual Result : ");
		switch (isValidAccount(&(transactionTestCase[n].cardHolderData), &account))
		{
		case SERVER_OK:
			printf("balance = %.1f and status of account = ", account.balance);
			if (account.state == RUNNING) {
				printf("Running\n");
			}
			else {
				printf("Blocked\n");
			}
			break;
		case ACCOUNT_NOT_FOUND:
			printf("Account not found\n");
			break;
		default:
			break;
		}
		printf("--------------------\n");
	}
	printf("\n\n");

}

void isBlockedAccountTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : isBlockedAccount\n\n");
	for (uint8 n = 0; n < 10; n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : PAN = %s\n", accountsDB[n].primaryAccountNumber);
		printf("Expected Result : ");
		switch (n)
		{
		case 1:
		case 7:
			printf("Blocked Account\n");
			break;
		default:
			printf("Running Account\n");
			break;
		}
		printf("Actual Result : ");
		if (isBlockedAccount(&accountsDB[n]) == BLOCKED_ACCOUNT) {
			printf("Blocked Account\n");
		}
		else {
			printf("Running Account\n");
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}

void isAmountAvailableTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : isAmountAvailable\n\n");
	for (uint8 n = 0; n < (sizeof(transactionTestCase)/sizeof(ST_transaction_t)); n++) {
		if (n == 2) {
			continue;
		}
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : transaction amount = %.1f and balance = ", transactionTestCase[n].terminalData.transAmount);
		switch (n) {
		case 0:
			printf("20000.0\n");
			break;
		case 1:
			printf("10000.0\n");
			break;
		case 3:
			printf("500.0\n");
			break;
		default:
			break;
		}
		printf("Expected Result : ");
		switch (n) {
		case 0:
		case 1:
			printf("Transaction accepted\n");
			break;
		case 3:
			printf("Low Balance\n");
			break;
		default:
			break;
		}
		printf("Actual Result : "); //isAmountAvailable(&transactionTestCase[n].terminalData, &accountsDB[])
		switch (n)
		{
		case 0:
			if (isAmountAvailable(&transactionTestCase[n].terminalData, &accountsDB[0]) == LOW_BALANCE) {
				printf("Low Balance\n");
			}
			else {
				printf("Transaction accepted\n");
			}
			break;
		case 1:
			if (isAmountAvailable(&transactionTestCase[n].terminalData, &accountsDB[1]) == LOW_BALANCE) {
				printf("Low Balance\n");
			}
			else {
				printf("Transaction accepted\n");
			}
			break;
		case 3:
			if (isAmountAvailable(&transactionTestCase[n].terminalData, &accountsDB[5]) == LOW_BALANCE) {
				printf("Low Balance\n");
			}
			else {
				printf("Transaction accepted\n");
			}
			break;
		default:
			break;
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}

void saveTransactionTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : saveTransaction\n\n");
	for (uint8 n = 0; n < (sizeof(transactionTestCase) / sizeof(ST_transaction_t)); n++) {
		saveTransaction(&transactionTestCase[n]);
	}
}

void listSavedTransactionsTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : listSavedTransactions\n\n");
	listSavedTransactions();
}
