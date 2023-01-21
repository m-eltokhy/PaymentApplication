#ifndef CARD_CARD_H_
#define CARD_CARD_H_

#include <stdio.h>
#include <string.h>
#include "../int_std_types.h"

typedef struct ST_cardData_t {
	uint8 cardHolderName[30];
	uint8 primaryAccountNumber[20];
	uint8 cardExpirationDate[6];
} ST_cardData_t;

typedef enum EN_cardError_t {
	CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
} EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

// Test Functions
void getCardHolderNameTest(void);
void getCardExpiryDateTest(void);
void getCardPANTest(void);

// Test Array of Card Datas

ST_cardData_t cardDataTestCases[];

#endif /* CARD_CARD_H_ */
