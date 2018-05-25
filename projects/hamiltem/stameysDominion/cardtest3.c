/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

/*
**		~     CARD TEST 3     ~
**	File:				cardtest3.c
**	Card Tested:		Embargo
**	Code Origin:		dominion_helpers.h, dominion.c
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "test_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHOW_GAME_STATES 0

// Embargo Objective:
// 1. Increment current coin coint in game state by 2.
// 2. Add embargo token to pile of player's choice (choice1).
// 3. Played embargo card is trashed-- it does not show up in
//	  discardCount or discardArray.

int main()
{
	printf("---------------------- CARD TEST 3: EMBARGO -----------------------\n\n");

	int numPlayers = 2,
		activePlayer = 0,
		handPos = 0,
		numPass = 0,
		numFail = 0,
		embargoChoice,
		falseEmbargoCount = 0,
		isChoiceValid,
		returnVal,
		result,
		expected,
		i;

	//Declare test gameState structs
	struct gameState testState, oracle;

	// Specify kingdom cards to use. Which cards are chosen is irrelevant for this test,
	// as long as one of them is embargo.
	int k[10] = { adventurer, smithy, gardens, embargo, village, minion, mine, tribute,
		sea_hag, treasure_map };

	printf("-------------------------------------------------------------------\n\n");
	printf("Test Plan:\n\n");
	printf("Attempt to embargo every possible pile.\n\n");
	printf("1a. If the pile is valid, an embargo token is added,\n"
		"the gameState\'s coins count is increased by 2, and the\n"
		"function returns 0.\n\n"
		"1b. If the pile is invalid, nothing happens and the\n"
		"function returns -1.\n\n");
	printf("2. If the choice was valid, check that the played\n"
		"Embargo card is trashed from the player\'s hand: the\n"
		"player\'s handCount is decremented, the card at handPos\n"
		"is no longer Embargo, the player\'s discardCount has not\n"
		"been incremented, and the playedCardCount has not been\n"
		"incremented; if it was invalid, check that nothing has\n"
		"changed.\n\n");

	printf("-------------------------------------------------------------------\n\n");
	
	printf("Beginning test with %d players...\n\n", numPlayers);
	
	// For all possible cards
	for (embargoChoice = curse; embargoChoice <= treasure_map; embargoChoice++) {

		printf("*******************************************************************\n\n");
		printf("Testing Card %d\n\n", embargoChoice);

		// Check embargoChoice for validity
		isChoiceValid = 0;
		if (embargoChoice >= curse && embargoChoice < adventurer)
			isChoiceValid = 1;
		for (i = 0; i < 10; i++) {
			if (k[i] == embargoChoice)
				isChoiceValid = 1;
		}
		printf("\tValid?\t%d\n\n", isChoiceValid);

		// Initialize game
		printf("Initializing game...\n\n");
		initializeGame(numPlayers, k, SEED, &testState);

		//Give active player an embargo at handPos and fill the rest of the hand with coppers;
		testState.handCount[activePlayer] = 5;
		for (i = 0; i < testState.handCount[activePlayer]; i++)
			testState.hand[activePlayer][i] = copper;
		testState.hand[activePlayer][handPos] = embargo;

		//Copy all values to oracle
		memcpy(&oracle, &testState, sizeof(struct gameState));

		//showGameState(&testState, numPlayers);
		printf("\nGame initialized.\n\n");

		//Call embargo and store return value
		printf("Calling embargo()...\n\n");
		returnVal = cardEffect(embargo, embargoChoice, -1, -1, &testState, handPos, 0);

		// TEST 1
		printf("TEST 1: Embargo token is applied correctly.\n\n");
		if (isChoiceValid) {
			// Test 1a -- Check that:
			// 1. Embargo token was added to correct pile.
			//	1a. The rest of the embargo piles are the same.
			for (i = curse; i <= treasure_map; i++) {
				if (i == embargoChoice) {
					printf("\tEmbargo Token applied\n"
						"\tto embargoTokens[%d]:", embargoChoice);
					result = testState.embargoTokens[embargoChoice];
					expected = oracle.embargoTokens[embargoChoice] + 1;
					domAssert(result, expected, &numPass, &numFail);
					printf("\n");
				}
				else {
					if (testState.embargoTokens[i] != oracle.embargoTokens[i]) {
						printf("\tERROR: Embargo Token applied\n"
							"\tincorrectly at index %d!\n", i);
						falseEmbargoCount++;
						numFail++;
					}
				}
			}
			if (falseEmbargoCount == 0) {
				printf("\tEmbargo Tokens not applied\n"
					"\tto any other piles.\t\tPASSED\n\n");
			}
			// 2. Coins increased by 2
			result = testState.coins;
			expected = oracle.coins + 2;
			printf("\tCoins increased from %d\n"
				"\tto %d:\t\t", oracle.coins, expected);
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");
			// 3. Function returns 0.
			printf("\tFunction returned 0:");
			domAssert(returnVal, 0, &numPass, &numFail);
			printf("\n");
		}
		else {
			// Test 1b -- Check that nothing happened and function returned -1.
			result = arraysAreSame(testState.embargoTokens, oracle.embargoTokens, treasure_map + 1);
			expected = 1;
			printf("\tNo Embargo Tokens Applied:");
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");

			result = testState.coins;
			expected = oracle.coins;
			printf("\tCoins not increased:\t");
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");

			// 3. Function returns -1.
			printf("\tFunction returned -1:\t");
			domAssert(returnVal, -1, &numPass, &numFail);
			printf("\n");
		}

		// Test 2 -- Check that Embargo card was trashed correctly (if valid)
		// and ignored (if invalid).
		if (isChoiceValid) {
			printf("TEST 2: Embargo card is trashed successfully (choice is valid).\n\n");
			
			//Player's hand count was decremented
			result = testState.handCount[activePlayer];
			expected = oracle.handCount[activePlayer] - 1;
			printf("\tPlayer's hand count was\n"
				"\tdecremented:\t");
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");

			//The card at handPos is no longer embargo
			result = testState.hand[activePlayer][handPos];
			expected = copper;
			printf("\tCard at handPos is no\n"
				"\tlonger embargo:\t");
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");

		}
		else {
			printf("TEST 2: Embargo card is ignored (choice is invalid).\n\n");

			//Player's hand count is the same
			result = testState.handCount[activePlayer];
			expected = oracle.handCount[activePlayer];
			printf("\tPlayer's hand count is\n"
				"\tstill the same:\t");
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");

			//Card at handPos is still embargo
			result = testState.hand[activePlayer][handPos];
			expected = embargo;
			printf("\tCard at handPos is still\n"
				"\tembargo:\t");
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");
		}

		//Player's discard count has not been incremented
		result = arraysAreSame(testState.discardCount, oracle.discardCount, MAX_PLAYERS);
		expected = 1;
		printf("\tDiscard count has not been\n"
			"\tincremented for anyone:");
		domAssert(result, expected, &numPass, &numFail);
		printf("\n");

		//playedCardCount has not been incremented
		result = testState.playedCardCount;
		expected = oracle.playedCardCount;
		printf("\tplayedCardCount has not been\n"
			"\tincremented:\t");
		domAssert(result, expected, &numPass, &numFail);
		printf("\n");
	}
	
	testComplete(&numPass, &numFail);

	return 0;
}
