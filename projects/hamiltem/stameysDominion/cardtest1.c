/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

/*
**		~     CARD TEST 1     ~
**	File:				cardtest1.c
**	Card Tested:		Smithy
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

// Smithy Objective:
// 1. Increase player's hand size by 3
// 2. Call drawCard() to draw 3 cards from player's deck
// 3. Call discardCard() to remove played Smithy card from
//	  player's hand and move it to the end of the playedCards
//    array.

int main()
{
	printf("----------------------- CARD TEST 1: SMITHY -----------------------\n\n");

	int numPlayers = 2,
		activePlayer = 0,
		testHandCount = 2,
		handPos = 0,
		numPass = 0,
		numFail = 0,
		result,
		expected,
		i;
	char* cardname = "Smithy";

	//Declare test gameState structs
	struct gameState testState, oracle;

	// Specify kingdom cards to use. Which cards are chosen is irrelevant for this test,
	// as long as one of them is smithy.
	int k[10] = { adventurer, smithy, gardens, embargo, village, minion, mine, tribute,
		sea_hag, treasure_map };

	printf("-------------------------------------------------------------------\n\n");
	printf("Test Plan:\n\n");
	printf("For both hand positions 1 and 0...\n\n");
	printf("\t1. Correct player's hand size has been increased by 3 - 1 = 2.\n");
	printf("\t\t1.a All other players' hand sizes are unchanged.\n\n");
	printf("\t2. Correct player's deck size has been decreased by 3.\n");
	printf("\t\t2.a All other players' deck sizes are unchanged.\n\n");
	printf("\t3. playedCardCount has been incremented\n\n");
	printf("\t4. playedCards[playedCardCount - 1] = smithy\n\n");
	printf("-------------------------------------------------------------------\n\n");


	// For all possible numbers of players
	for (numPlayers = 2; numPlayers <= MAX_PLAYERS; numPlayers++) {
		
		printf("Beginning test with %d players...\n\n", numPlayers);

		// For hand positions 1-2...
		for (handPos = 0; handPos < testHandCount; handPos++) {
			
			printf("\n*****\n");
			printf("Testing Hand Position = %d...\n", handPos);
			printf("*****\n\n");
			
			//Initialize test game state
			printf("Initializing game...\n\n");
			initializeGame(numPlayers, k, SEED, &testState);
			//Give active player an initial handCount of maxHandCount
			//Give active player a smithy at handPos and fill the rest of the hand with coppers;
			testState.handCount[activePlayer] = 5;
			for (i = 0; i < testState.handCount[activePlayer]; i++)
				testState.hand[activePlayer][i] = copper;
			testState.hand[activePlayer][handPos] = smithy;
			//Copy all values to oracle
			memcpy(&oracle, &testState, sizeof(struct gameState));
			//showGameState(&testState, numPlayers);
			printf("\nGame initialized.\n\n");

			/*Optional State Printout to Console -----------------------------------------------*/
			if (SHOW_GAME_STATES) {
				printf("Test Hands:\n");
				printf("Player 0:\t");
				printHand(testState.hand[0], testState.handCount[0]);
				printf("Player 1:\t");
				printHand(testState.hand[1], testState.handCount[1]);
				printf("Player 2:\t");
				printHand(testState.hand[2], testState.handCount[2]);
				printf("Player 3:\t");
				printHand(testState.hand[3], testState.handCount[3]);
				printf("\n");
			}
			/*----------------------------------------------------------------------------------*/


			printf("PLAYER %d PLAYS %s FROM HAND POSITION %d IN %d-PLAYER GAME\n\n",
				activePlayer, cardname, handPos, numPlayers);

			cardEffect(smithy, -1, -1, -1, &testState, handPos, 0);
			/*Optional State Printout to Console -----------------------------------------------*/
			if (SHOW_GAME_STATES) {
				printf("Test Hand:\t");
				printHand(testState.hand[activePlayer], testState.handCount[activePlayer]);
				printf("Oracle Hand:\t");
				printHand(testState.hand[activePlayer], testState.handCount[activePlayer]);
			}
			/*----------------------------------------------------------------------------------*/

			// Test 1: Verify player's hand count
			printf("\nTEST 1:\n");
			result = testState.handCount[activePlayer];
			expected = oracle.handCount[activePlayer] + 2;
			printf("\tPlayer %d\'s hand count\n"
				"\tis expected value (%d):", activePlayer, expected);
			domAssert(result, expected, &numPass, &numFail);
			// Test 1a: Verify other players hand counts have not changed
			for (i = 1; i < numPlayers; i++) {
				result = testState.handCount[i];
				expected = oracle.handCount[i];
				printf("\tPlayer %d\'s hand count\n"
					"\thas not changed:", i);
				domAssert(result, expected, &numPass, &numFail);
			}

			// Test 2: Verify player's deck count
			printf("\nTEST 2:\n");
			result = testState.deckCount[activePlayer];
			expected = oracle.handCount[activePlayer] - 3;
			printf("\tPlayer %d\'s deck count\n"
				"\tis expected value (%d):", activePlayer, expected);
			domAssert(result, expected, &numPass, &numFail);
			// Test 2a: Verify other players hand counts have not changed
			for (i = 1; i < numPlayers; i++) {
				result = testState.deckCount[i];
				expected = oracle.deckCount[i];
				printf("\tPlayer %d\'s hand count\n"
					"\thas not changed:", i);
				domAssert(result, expected, &numPass, &numFail);
			}
			printf("\n");

			// Test 3: playedCardCount has been incremented
			printf("\nTEST 3:\n");
			result = testState.playedCardCount;
			expected = oracle.playedCardCount + 1;
			printf("\tplayedCardCount has been\n"
				"\tincremented to %d:", expected);
			domAssert(result, expected, &numPass, &numFail);

			// Test 4: Last item in playedCards array is smithy
			printf("\nTEST 4:\n");
			result = testState.playedCards[testState.playedCardCount - 1];
			expected = smithy;
			printf("\tLast item in playedCards\n"
				"\tarray is Smithy:");
			domAssert(result, expected, &numPass, &numFail);

		}

		printf("-------------------------------------------------------------------\n\n");
	}
	
	testComplete(&numPass, &numFail);

	return 0;
}
