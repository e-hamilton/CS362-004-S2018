/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

/*
**		~     CARD TEST 4     ~
**	File:				cardtest4.c
**	Card Tested:		Great Hall
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

// Great Hall Objective:
// 1. Draw card
//		1a. Increment player's hand count by 1. After the
//			played great_hall card is discarded, the hand
//			count will decrease by 1. Therefore, hand count
//			should not change.
//		1b. Last card in player's hand becomes top of deck.
//			After the played great_hall card is discarded,
//			this card will be moved to the great_hall card's
//			old position. Therefore, hand[activePlayer][handPos]
//			should equal the original last card in the deck array.
// 2. Number of actions increases by 1.
// 3. Played great_hall is moved from hand to playedCards array;
//	  playedCardCount is incremented.

int main()
{
	printf("-------------------- CARD TEST 4: GREAT HALL ----------------------\n\n");

	int numPlayers = 2,
		activePlayer = 0,
		handPos = 0,
		numPass = 0,
		numFail = 0,
		testHandSize = 5,
		predictedDraw,
		result,
		expected,
		i;

	//Declare test gameState structs
	struct gameState testState, oracle;

	// Specify kingdom cards to use. Which cards are chosen is irrelevant for this test,
	// as long as one of them is embargo.
	int k[10] = { adventurer, smithy, great_hall, embargo, village, minion, mine, tribute,
		sea_hag, treasure_map };

	printf("-------------------------------------------------------------------\n\n");
	printf("Test Plan:\n\n");
	printf("Attempt to play Great Hall from each hand position, with a test\n"
		"hand size being 5.\n\n");
	printf("1. Verify hand count has not changed. (Hand count is increased\n"
		"by 1 when Great Hall is played, but decreased by 1 when the played\n"
		"card is discarded.)\n\n");
	printf("2. Verify player's hand at handPos is now occupied by the\n"
		"drawn card. (When the new card is drawn, it is added to the end\n"
		"of the hand. When the played Great Hall card is discarded, it is\n"
		"replaced by the last card in the hand.)\n\n");
	printf("3. Verify that numActions has been incremented by 1.\n\n");
	printf("4. Verify that playedCardCount has been incremented by 1.\n\n");
	printf("5. Verify that playedCards[playedCardCount - 1] = great_hall.\n\n");

	printf("-------------------------------------------------------------------\n\n");
	
	printf("Beginning test with %d players...\n\n", numPlayers);

	// For all possible hand positions...
	for (handPos = 0; handPos < testHandSize; handPos++) {
		printf("*******************************************************************\n\n");
		printf("TESTING HAND POSITION %d\n\n", handPos);

		// Initialize game
		printf("Initializing game...\n\n");
		initializeGame(numPlayers, k, SEED, &testState);

		// Give active player a great_hall at handPos and fill the rest of the hand with coppers;
		testState.handCount[activePlayer] = testHandSize;
		for (i = 0; i < testState.handCount[activePlayer]; i++)
			testState.hand[activePlayer][i] = copper;
		testState.hand[activePlayer][handPos] = great_hall;

		// Fill player's deck with sea hags to distinguish them from the cards already in the
		// player's hand
		testState.deckCount[activePlayer] = 5;
		for (i = 0; i < testState.deckCount[activePlayer]; i++)
			testState.deck[activePlayer][i] = sea_hag;

		// Copy all values to oracle
		memcpy(&oracle, &testState, sizeof(struct gameState));

		printf("\nHAND:\t");
		printHand(testState.hand[activePlayer], testState.handCount[activePlayer]);
		printf("\nGame initialized.\n\n");

		// Play card
		cardEffect(great_hall, -1, -1, -1, &testState, handPos, 0);
		printf("\n");
		
		// Test 1: Verify hand count has not changed
		printf("TEST 1: Verify hand count has not changed.\n\n");
		result = testState.handCount[activePlayer];
		expected = oracle.handCount[activePlayer];
		printf("\tHand Count Predicted:\t%d\n", expected);
		printf("\tActual Hand Count:\t%d\n\n", result);
		domAssert(result, expected, &numPass, &numFail);
		printf("\n");

		// Test 2: Verify player's hand at handPos now holds predictedDraw
		printf("TEST 2: Verify player's hand at handPos now holds drawn card.\n\n");
		// Predict card drawn by getting the card at the end of the player's deck array
		// from the oracle
		predictedDraw = oracle.deck[activePlayer][oracle.deckCount[activePlayer] - 1];
		result = testState.hand[activePlayer][handPos];
		expected = predictedDraw;
		printf("\tCard Predicted:\t%d\n", expected);
		printf("\tActual Card:\t%d\n\n", result);
		domAssert(result, expected, &numPass, &numFail);
		printf("\n");

		// Test 3: Verify that numActions has been incremented by 1
		printf("TEST 3: Verify numActions has been incremented by 1.\n\n");
		result = testState.numActions;
		expected = oracle.numActions + 1;
		printf("\tnumActions Predicted:\t%d\n", expected);
		printf("\tActual numActions:\t%d\n\n", result);
		domAssert(result, expected, &numPass, &numFail);
		printf("\n");

		// Test 4: Verify that playedCards[playedCardCount - 1] = great_hall.
		printf("TEST 4: Verify that playedCards[playedCardCount - 1] = great_hall.\n\n");
		result = testState.playedCards[testState.playedCardCount - 1];
		expected = great_hall;
		printf("\tCard Predicted:\t%d\n", expected);
		printf("\tActual card:\t%d\n\n", result);
		domAssert(result, expected, &numPass, &numFail);
		printf("\n");
	}
	
	testComplete(&numPass, &numFail);

	return 0;
}
