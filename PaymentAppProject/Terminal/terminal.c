#include "terminal.h"

ST_terminalData_t terminalDataTestCases[]={
	{.maxTransAmount = 1000.0,.transAmount = 500.0},
	{.maxTransAmount = 1000.0,.transAmount = 5000.0},
	{.maxTransAmount = 1000.0,.transAmount = 0.0},
};

ST_cardData_t cardData[] = {
	{"Mohamed khaled eltokhy","8989374605436851","05/30"},
	{"mahmoud islam abdallah","1355698302145932","09/22"},
	{"magdy ibrahim shaarawy","3654468001010490","08/25"}};

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	static uint8 currDate [15] = __DATE__;
	for (uint8 ctr = 0; ctr < 10; ctr++) {
		switch (ctr) {
		case 0:
		case 1:
			termData->transactionDate[ctr] = currDate[ctr + 4];
			break;
		case 2:
		case 5:
			termData->transactionDate[ctr] = '/';
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			termData->transactionDate[ctr] = currDate[ctr + 1];
			break;
		}
	}
	switch (currDate[0]) {
	case 'J':
		if (currDate[1] == 'a') {
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '1';
		}
		else if (currDate[2] == 'n') {
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '6';
		}
		else {
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '7';
		}
		break;
	case 'F':
		termData->transactionDate[3] = '0';
		termData->transactionDate[4] = '2';
		break;
	case 'M':
		if (currDate[2] == 'r') {
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '3';
		}
		else {
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '5';
		}
		break;
	case 'A':
		if (currDate[1] == 'p') {
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '4';
		}
		else {
			termData->transactionDate[3] = '0';
			termData->transactionDate[4] = '8';
		}
		break;
	case 'S':
		termData->transactionDate[3] = '0';
		termData->transactionDate[4] = '9';
		break;
	case 'O':
		termData->transactionDate[3] = '1';
		termData->transactionDate[4] = '0';
		break;
	case 'N':
		termData->transactionDate[3] = '1';
		termData->transactionDate[4] = '1';
		break;
	case 'D':
		termData->transactionDate[3] = '1';
		termData->transactionDate[4] = '2';
		break;
	}
	termData->transactionDate[10] = '\0';
	return TERMINAL_OK;

	/* this code is in case we supply the transaction date manually:
	if (strlen(termData->transactionDate) != 10) {
	return WRONG_DATE;
	}
	uint8 ctr = 0;
	while (termData->transactionDate[ctr] != '\0') {
		switch (ctr) {
		case 2:
		case 5:
			if (termData->transactionDate[ctr] != '/') {
				return WRONG_DATE;
				}
			break;
		default:
			if (termData->transactionDate[ctr] < '0' || termData->transactionDate[ctr] > '9') {
				return WRONG_DATE;
				}
			break;
		}
		ctr++;
	}
	return TERMINAL_OK;
	*/
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData,ST_terminalData_t* termData) {
	if ((cardData->cardExpirationDate[3] < termData->transactionDate[8])
		|| ((cardData->cardExpirationDate[3] == termData->transactionDate[8])
			&& (cardData->cardExpirationDate[4]
				< termData->transactionDate[9]))) {
		return EXPIRED_CARD;
	}
	if ((cardData->cardExpirationDate[0] < termData->transactionDate[3])
		|| ((cardData->cardExpirationDate[0] == termData->transactionDate[3])
			&& (cardData->cardExpirationDate[1] < termData->transactionDate[4]))) {
		return EXPIRED_CARD;
	}
	return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	if (termData->transAmount <= 0.0) {
		return INVALID_AMOUNT;
	}
	return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) {
		return EXCEED_MAX_AMOUNT;
	}
	return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {
	if (maxAmount <= 0.0) {
		return INVALID_MAX_AMOUNT;
	}
	termData->maxTransAmount = maxAmount;
	return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {
	uint8 sum = 0, temp = 0, len = strlen(cardData->primaryAccountNumber);
	for (int8 digit = len - 1; digit >= 0; digit--) {
		if (cardData->primaryAccountNumber[digit] < '0' || cardData->primaryAccountNumber[digit] >'9') {
			return INVALID_CARD;
		}
		if ((len - digit - 1) % 2) {
			sum += (cardData->primaryAccountNumber[digit] - 48);
		}
		else {
			temp = (cardData->primaryAccountNumber[digit] - 48) * 2;
			sum += temp / 10;
			sum += temp % 10;
		}
	}
	if (sum % 10 != 0) {
		return INVALID_CARD;
	}
	return TERMINAL_OK;
}


void getTransactionDateTest(void) {
	for (uint8 ctr = 0; ctr < (sizeof(terminalDataTestCases) / sizeof(ST_terminalData_t)); ctr++) {
		getTransactionDate(&terminalDataTestCases[ctr]);
	}
	printf("The transaction date is today's date : %s\n\n\n", terminalDataTestCases[0].transactionDate);
	printf("--------------------\n");
	printf("\n\n");
}
void isCardExpriedTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : isCardExpried\n\n");
	for (uint8 n = 0; n < (sizeof(cardData) / sizeof(ST_cardData_t)); n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : %s as expiration date and %s as transaction date\n",cardData[n].cardExpirationDate,terminalDataTestCases[n].transactionDate);
		printf("Expected Result : ");
		if (n == 0 || n==2) {
			printf("Terminal OK \n");
		}
		else {
			printf("Expired Card \n");
		}
		printf("Actual Result : ");
		if (isCardExpired(&cardData[n], &terminalDataTestCases[n]) == EXPIRED_CARD) {
			printf("Expired Card\n");
		}
		else if(isCardExpired(&cardData[n], &terminalDataTestCases[n]) == TERMINAL_OK){
			printf("Terminal Ok\n");
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}
void getTransactionAmountTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : getTransactionAmount\n\n");
	for (uint8 n = 0; n < (sizeof(terminalDataTestCases) / sizeof(ST_terminalData_t)); n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : %.3f as transaction amount\n", terminalDataTestCases[n].transAmount);
		if (n == 0 || n == 1) {
			printf("Expected Result : Terminal OK \n");
		}
		else {
			printf("Expected Result : Invalid Amount \n");
		}
		printf("Actual Result : ");
		if (getTransactionAmount(&terminalDataTestCases[n]) == INVALID_AMOUNT) {
			printf("Invalid Amount\n");
		}
		else if (getTransactionAmount(&terminalDataTestCases[n]) == TERMINAL_OK) {
			printf("Terminal Ok\n");
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}
void isBelowMaxAmountTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : isBelowMaxAmount\n\n");
	for (uint8 n = 0; n < (sizeof(terminalDataTestCases) / sizeof(ST_terminalData_t)); n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : Max amount =%.3f and transaction amount =%.3f\n",terminalDataTestCases[n].maxTransAmount, terminalDataTestCases[n].transAmount);
		if (n == 0 || n == 2) {
			printf("Expected Result : Terminal OK \n");
		}
		else {
			printf("Expected Result : Exceed max amount \n");
		}
		printf("Actual Result : ");
		if (isBelowMaxAmount(&terminalDataTestCases[n]) == EXCEED_MAX_AMOUNT) {
			printf("Exceed max amount \n");
		}
		else if (isBelowMaxAmount(&terminalDataTestCases[n]) == TERMINAL_OK) {
			printf("Terminal Ok\n");
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}
void setMaxAmountTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : setMaxAmount\n\n");
	printf("Test Case 1 : \n");
	printf("Input Data : Set %.3f as max transaction amount\n", 1500.0);
	printf("Expected Result : Terminal OK \n");
	printf("Actual Result : ");
	if (setMaxAmount(&terminalDataTestCases[0], 1500.0) == TERMINAL_OK) {
		printf("Terminal Ok\n");
	}
	else {
		printf("Exceed max amount \n");
	}
	printf("--------------------\n");
	printf("Test Case 2 : \n");
	printf("Input Data : Set %.3f as max transaction amount\n", 0.0);
	printf("Expected Result : Invalid max amount \n");
	printf("Actual Result : ");
	if (setMaxAmount(&terminalDataTestCases[0], 0.0) == TERMINAL_OK) {
		printf("Terminal Ok\n");
	}
	else {
		printf("Invalid max amount \n");
	}
	printf("--------------------\n");
	printf("Test Case 3 : \n");
	printf("Input Data : Set %.3f as max transaction amount\n", -1000.0);
	printf("Expected Result : Invalid max amount \n");
	printf("Actual Result : ");
	if (setMaxAmount(&terminalDataTestCases[0], -1000.0) == TERMINAL_OK) {
		printf("Terminal Ok\n");
	}
	else {
		printf("Invalid max amount \n");
	}
	printf("--------------------\n");
	printf("\n\n");
}
void isValidCardPANTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : isValidCardPAN\n\n");
	for (uint8 n = 0; n < (sizeof(cardData) / sizeof(ST_cardData_t)); n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : %s as Card PAN\n", cardData[n].primaryAccountNumber);
		if (n == 0) {
			printf("Expected Result : Terminal OK \n");
		}
		else {
			printf("Expected Result : Invalid Card \n");
		}
		printf("Actual Result : ");
		if (isValidCardPAN(&cardData[n]) == INVALID_CARD) {
			printf("Invalid Card \n");
		}
		else if (isValidCardPAN(&cardData[n]) == TERMINAL_OK) {
			printf("Terminal Ok\n");
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}