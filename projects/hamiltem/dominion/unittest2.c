/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

/*
**		~     UNIT TEST 2     ~
**	File:				unittest2.c
**	Function Tested:	gainCard()
**	Function Origin:	dominion_helpers.h, dominion.c
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "test_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

#define SHOW_GAME_STATES 0

int main()
{
	printf("-------------------- UNIT TEST 2: gainCard() --------------------\n\n");

	int numPlayers = 2;

	printf("Beginning test with %d players...\n\n", numPlayers);

	//Declare test gameState struct
	struct gameState testState;

	// Specify kingdom cards to use (I made sure to include the last 3 kingdom cards in
	// the CARD enum--salvager, sea_hag, and treasure_map-- to help ensure gainCard can
	// draw a card from the beginning of the CARD enum through the end.
	int k[10] = { adventurer, smithy, gardens, embargo, village, minion, mine, salvager,
		sea_hag, treasure_map };
	int returnVal, numPass = 0, numFail = 0, activePlayer = 0;

	printf("Initializing game...\n\n");
	initializeGame(numPlayers, k, SEED, &testState);
	printf("\nGame initialized.\n\n");

	/*Optional State Printout to Console -----------------------------------------------*/
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	/*----------------------------------------------------------------------------------*/

	printf("-------------------------------------------------------------------\n\n");
	printf("Possible Inputs and Outputs:\n\n");
	printf("1. Discard card (toFlag = 0) -> Card at supplyPos is added to the\n"
		"player's discard pile, incrementing the player's discard count. The\n"
		"Supply Count for the appropriate pile is decremented.\n\n");
	printf("2. Add card to deck (toFlag = 1) -> Card at supplyPos is added to\n"
		"the player's deck, incrementing the player's deck count. The Supply\n"
		"Count for the appropriate pile is decremented.\n\n");
	printf("3. Add card to hand (toFlag = 2) -> Card at supplyPos is added to\n"
		"the player's hand, incrementing the player's hand count. The Supply\n"
		"Count for the appropriate pile is decremented.\n\n");
	printf("4. Invalid Choice (Pile at supplyPos is empty) -> Return -1. The\n"
		"player's discard pile, deck, and hand remain unchanged. The Supply\n"
		"Count has not been decremented.\n\n");
	printf("-------------------------------------------------------------------\n\n");
	
	// Test 1: toFlag = 0
	printf("Test 1: Gain card to player's discard pile (toFlag = 0)\n\n");
	int i, j = 0, originalSupply;
	char *name;
	for (i = 0; i < treasure_map + 1; i++) {
		if (testState.supplyCount[i] != -1) {
			getCardName(&name, i);
			printf("\tGaining %s\n", name);
			originalSupply = testState.supplyCount[i];
			returnVal = gainCard(i, &testState, 0, activePlayer);
			//Check that function returned successfully.
			printf("\t\tFunction Returns 0:");
			domAssert(returnVal, 0, &numPass, &numFail);
			//Check that discard count was incremented for correct player.
			printf("\t\tDiscard Count for\n\t\tPlayer %d = %d:\t", activePlayer, j + 1);
			domAssert(testState.discardCount[activePlayer], j + 1, &numPass, &numFail);
			//Check that discard[activePlayer][j] = i
			printf("\t\tCard Type %d Added\n\t\tto Discard Pile:", i);
			domAssert(testState.discard[activePlayer][j], i, &numPass, &numFail);
			//Check that supplyCount has been decremented;
			printf("\t\tSupply Count (%d)\n\t\tDecremented by 1 (%d):", originalSupply, originalSupply - 1);
			domAssert(testState.supplyCount[i], originalSupply - 1, &numPass, &numFail);
			printf("\n");
			j++;
		}
	}

	/*Optional State Printout to Console -----------------------------------------------*/
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	/*----------------------------------------------------------------------------------*/

	printf("Resetting game...\n\n");
	initializeGame(numPlayers, k, SEED, &testState);
	printf("\nGame reset.\n\n");

	/*Optional State Printout to Console -----------------------------------------------*/
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	/*----------------------------------------------------------------------------------*/

	// Test 2: toFlag = 1
	printf("Test 2: Gain card to player's deck (toFlag = 1)\n\n");
	j = testState.deckCount[activePlayer];
	for (i = 0; i < treasure_map + 1; i++) {
		if (testState.supplyCount[i] != -1) {
			getCardName(&name, i);
			printf("\tGaining %s\n", name);
			originalSupply = testState.supplyCount[i];
			returnVal = gainCard(i, &testState, 1, activePlayer);
			//Check that function returned successfully.
			printf("\t\tFunction Returns 0:");
			domAssert(returnVal, 0, &numPass, &numFail);
			//Check that deck count was incremented for correct player.
			printf("\t\tDeck Count for\n\t\tPlayer %d = %d:\t", activePlayer, j + 1);
			domAssert(testState.deckCount[activePlayer], j + 1, &numPass, &numFail);
			//Check that deck[activePlayer][j] = i
			printf("\t\tCard Type %d Added\n\t\tto Player\'s Deck:", i);
			domAssert(testState.deck[activePlayer][j], i, &numPass, &numFail);
			//Check that supplyCount has been decremented;
			printf("\t\tSupply Count (%d)\n\t\tDecremented by 1 (%d):", originalSupply, originalSupply - 1);
			domAssert(testState.supplyCount[i], originalSupply - 1, &numPass, &numFail);
			printf("\n");
			j++;
		}
	}

	/*Optional State Printout to Console -----------------------------------------------*/
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	/*----------------------------------------------------------------------------------*/

	printf("Resetting game...\n\n");
	initializeGame(numPlayers, k, SEED, &testState);
	printf("\nGame reset.\n\n");

	/*Optional State Printout to Console -----------------------------------------------*/
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	/*----------------------------------------------------------------------------------*/

	// Test 3: toFlag = 2
	printf("Test 3: Gain card to player's hand (toFlag = 2)\n\n");
	j = testState.handCount[activePlayer];
	for (i = 0; i < treasure_map + 1; i++) {
		if (testState.supplyCount[i] != -1) {
			getCardName(&name, i);
			printf("\tGaining %s\n", name);
			originalSupply = testState.supplyCount[i];
			returnVal = gainCard(i, &testState, 2, activePlayer);
			//Check that function returned successfully.
			printf("\t\tFunction Returns 0:");
			domAssert(returnVal, 0, &numPass, &numFail);
			//Check that hand count was incremented for correct player.
			printf("\t\tHand Count for\n\t\tPlayer %d = %d:\t", activePlayer, j + 1);
			domAssert(testState.handCount[activePlayer], j + 1, &numPass, &numFail);
			//Check that hand[activePlayer][j] = i
			printf("\t\tCard Type %d Added\n\t\tto Player\'s Hand:", i);
			domAssert(testState.hand[activePlayer][j], i, &numPass, &numFail);
			//Check that supplyCount has been decremented;
			printf("\t\tSupply Count (%d)\n\t\tDecremented by 1 (%d):", originalSupply, originalSupply - 1);
			domAssert(testState.supplyCount[i], originalSupply - 1, &numPass, &numFail);
			printf("\n");
			j++;
		}
	}

	/*Optional State Printout to Console -----------------------------------------------*/
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	/*----------------------------------------------------------------------------------*/

	printf("Resetting game...\n\n");
	initializeGame(numPlayers, k, SEED, &testState);
	printf("\nGame reset.\n\n");

	/*Optional State Printout to Console -----------------------------------------------*/
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	/*----------------------------------------------------------------------------------*/

	// Test 4: Invalid Choice; pile at supplyPos is empty
	printf("Test 4: Attempt to draw card from empty pile (Curse)\n\n");
	testState.supplyCount[curse] = 0;
	originalSupply = testState.supplyCount[curse];
	int originalDeck = testState.deckCount[activePlayer];
	printf("\tAttempt to gain card\n\trejected:\t");
	returnVal = gainCard(curse, &testState, 1, activePlayer);
	domAssert(returnVal, -1, &numPass, &numFail);
	printf("\tSupply Count unchanged:");
	domAssert(testState.supplyCount[curse], originalSupply, &numPass, &numFail);
	printf("\tDeck Count unchanged:");
	domAssert(testState.deckCount[activePlayer], originalDeck, &numPass, &numFail);

	testComplete(&numPass, &numFail);
		
	return 0;
}
