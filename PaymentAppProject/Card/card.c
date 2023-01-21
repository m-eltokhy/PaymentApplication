#include "card.h"

ST_cardData_t cardDataTestCases[] = {
	{"Mohamed khaled eltokhy","8989374605436851","05/30"},
	{"omar khaled","8986%24605436851","02 28"},
	{"ahmed mohamed-mahmoud","89862 46605436851","01-26"},
	{"adham emad elshar2awy","24605436851","5/25"}
};

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	if (strlen(cardData->cardHolderName) < 20 || strlen(cardData->cardHolderName) > 24) {
		return WRONG_NAME;
	}
	uint8 ctr = 0;
	while (cardData->cardHolderName[ctr] != '\0') {
		if ((cardData->cardHolderName[ctr] < 'a' || cardData->cardHolderName[ctr] > 'z') && (cardData->cardHolderName[ctr] < 'A' || cardData->cardHolderName[ctr] > 'Z')
			&& cardData->cardHolderName[ctr] != ' ') {
			return WRONG_NAME;
		}
		ctr++;
	}
	return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	if (strlen(cardData->cardExpirationDate) != 5) {
		return WRONG_EXP_DATE;
	}
	uint8 ctr = 0;
	while (cardData->cardExpirationDate[ctr] != '\0') {
		switch (ctr) {
		case 2:
			if (cardData->cardExpirationDate[ctr] != '/') {
				return WRONG_EXP_DATE;
			}
			break;
		default:
			if (cardData->cardExpirationDate[ctr] < '0' || cardData->cardExpirationDate[ctr] > '9') {
				return WRONG_EXP_DATE;
			}
			break;
		}
		ctr++;
	}
	return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	if (strlen(cardData->primaryAccountNumber) < 16 || strlen(cardData->primaryAccountNumber) > 19) {
		return WRONG_PAN;
	}
	uint8 ctr = 0;
	while (cardData->primaryAccountNumber[ctr] != '\0') {
		if (cardData->primaryAccountNumber[ctr] < '0' || cardData->primaryAccountNumber[ctr] > '9') {
			return WRONG_PAN;
		}
		ctr++;
	}
	return CARD_OK;
}

// Test Functions
void getCardHolderNameTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : getCardHolderName\n\n");
	for (uint8 n = 0; n < (sizeof(cardDataTestCases)/sizeof(ST_cardData_t)); n++) {
		printf("Test Case %d : \n",n+1);
		printf("Input Data : %s\n",cardDataTestCases[n].cardHolderName);
		if (n == 0) {
			printf("Expected Result : Card OK \n");
		}
		else {
			printf("Expected Result : Wrong Name \n");
		}
		printf("Actual Result : ");
		if (getCardHolderName(&cardDataTestCases[n]) == CARD_OK) {
			printf("Card Ok\n");
		}
		else if(getCardHolderName(&cardDataTestCases[n]) == WRONG_NAME){
			printf("Wrong Name\n");
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}
void getCardExpiryDateTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : getCardExpiryDate\n\n");
	for (uint8 n = 0; n < (sizeof(cardDataTestCases) / sizeof(ST_cardData_t)); n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : %s\n", cardDataTestCases[n].cardExpirationDate);
		if (n == 0) {
			printf("Expected Result : Card OK \n");
		}
		else {
			printf("Expected Result : Wrong Expiration Date \n");
		}
		printf("Actual Result : ");
		if (getCardExpiryDate(&cardDataTestCases[n]) == CARD_OK) {
			printf("Card Ok\n");
		}
		else if (getCardExpiryDate(&cardDataTestCases[n]) == WRONG_EXP_DATE) {
			printf("Wrong Expiration Date\n");
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}
void getCardPANTest(void) {
	printf("Tester Name: Mohamed Eltokhy\nFunction Name : getCardPAN\n\n");
	for (uint8 n = 0; n < (sizeof(cardDataTestCases) / sizeof(ST_cardData_t)); n++) {
		printf("Test Case %d : \n", n + 1);
		printf("Input Data : %s\n", cardDataTestCases[n].primaryAccountNumber);
		if (n == 0) {
			printf("Expected Result : Card OK \n");
		}
		else {
			printf("Expected Result : Wrong PAN \n");
		}
		printf("Actual Result : ");
		if (getCardPAN(&cardDataTestCases[n]) == CARD_OK) {
			printf("Card Ok\n");
		}
		else if (getCardPAN(&cardDataTestCases[n]) == WRONG_PAN) {
			printf("Wrong PAN\n");
		}
		printf("--------------------\n");
	}
	printf("\n\n");
}