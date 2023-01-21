#ifndef TERMINAL_TERMINAL_H_
#define TERMINAL_TERMINAL_H_

#include <stdio.h>
#include <string.h>
#include "../int_std_types.h"
#include "../Card/card.h"

typedef struct ST_terminalData_t {
	float transAmount;
	float maxTransAmount;
	uint8 transactionDate[11];
} ST_terminalData_t;

typedef enum EN_terminalError_t {
	TERMINAL_OK,
	WRONG_DATE,
	EXPIRED_CARD,
	INVALID_CARD,
	INVALID_AMOUNT,
	EXCEED_MAX_AMOUNT,
	INVALID_MAX_AMOUNT
} EN_terminalError_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount);
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);

// Test functions
void getTransactionDateTest(void);
void isCardExpriedTest(void);
void getTransactionAmountTest(void);
void isBelowMaxAmountTest(void);
void setMaxAmountTest(void);
void isValidCardPANTest(void);

ST_terminalData_t terminalDataTestCases[];
ST_cardData_t cardData[];

#endif /* TERMINAL_TERMINAL_H_ */
