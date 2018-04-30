/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

/*
**		~     UNIT TEST 3     ~
**	File:				unittest3.c
**	Function Tested:	discardCard()
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
	printf("-------------------- UNIT TEST 3: discardCard() --------------------\n\n");

	int numPlayers = 2, 
		activePlayer = 0, 
		numPass = 0, 
		numFail = 0,
		returnVal, 
		handPos, 
		selectedCard, 
		originalDiscardCount, 
		originalHandCount, 
		originalPlayedCardsCount,
		expectedCard;

	printf("Beginning test with %d players...\n\n", numPlayers);

	//Declare test gameState struct
	struct gameState testState;

	// Specify kingdom cards to use
	int k[10] = { adventurer, smithy, gardens, embargo, village, minion, mine, salvager,
		sea_hag, treasure_map };

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
	printf("1. DISCARD - Trash flag is set to false. Card is meant to be\n"
		"discarded, not played. In all below cases, the correct card is\n"
		"added to the end of the player's discard array and the discard count\n"
		"for the correct player has been incremented by 1.\n\n");
	printf("\t1a. Discard last card in hand (handPos = handCount[player]-1).\n\n");
	printf("\t1b. Discard first card in hand (handPos = 0) until hand is\n"
		"\tempty.\n\n");
	
	printf("2. PLAY - Trash flag is set to false. Card is meant to be played,\n"
		"not discarded. In all below cases, the correct card is added to\n"
		"the end of the playedCards array (to be discarded later during\n"
		"the cleanup phase) and the played card count has been incremented\n"
		"by 1.\n\n");
	printf("\t1a. Discard last card in hand (handPos = handCount[player]-1).\n\n");
	printf("\t1b. Discard first card in hand (handPos = 0) until hand is\n"
		"\tempty.\n\n");

	printf("3. TRASH - Trash flag is set to true. In all below cases, the correct\n"
		"card is simply overwritten (i.e. the player's Discard pile, discard\n"
		"count, Played Cards pile, and played card count are not modified.\n\n");
	printf("\t2a. Trash last card in hand (handPos = handCount[player]-1).\n\n");
	printf("\t2b. Trash first card in hand (handpos = 0) until hand is\n"
		"\tempty.\n\n");
	printf("-------------------------------------------------------------------\n\n");
	
	// Test 1: True Discard
	printf("Test 1: Discard with intent to move card to discard pile.\n\n");
	handPos = testState.handCount[activePlayer] - 1;
	selectedCard = testState.hand[activePlayer][handPos];
	originalDiscardCount = testState.discardCount[activePlayer];
	originalHandCount = testState.handCount[activePlayer];

	printf("\tDiscard card %d (last card) in player %d\'s hand...\n", handPos, activePlayer);
	returnVal = discardCard(handPos, activePlayer, &testState, 0);

	printf("\t\tFunction returns 0:");
	domAssert(returnVal, 0, &numPass, &numFail);

	printf("\t\tHand count decremented:");
	domAssert(testState.handCount[activePlayer], originalHandCount - 1, &numPass, &numFail);

	printf("\t\tPosition in hand = -1:");
	domAssert(testState.hand[activePlayer][handPos], -1, &numPass, &numFail);

	printf("\t\tPlayer's discardCount\n\t\tincremented:\t");
	domAssert(testState.discardCount[activePlayer], originalDiscardCount + 1, &numPass, &numFail);

	printf("\t\tSelected card is last\n"
		"\t\titem in player's discard\n"
		"\t\tpile matches card:");
	domAssert(testState.discard[activePlayer][testState.discardCount[activePlayer] - 1], selectedCard, &numPass, &numFail);
	printf("\n");

	// Start at the beginning and keep discarding cards until hand is empty
	handPos = -1;
	while (testState.handCount[activePlayer] > 0) {
		handPos++;
		originalDiscardCount = testState.discardCount[activePlayer];
		originalHandCount = testState.handCount[activePlayer];
		if (testState.handCount[activePlayer] > 1) {
			expectedCard = testState.hand[activePlayer][originalHandCount - 1];
		}
		else {
			expectedCard = -1;
		}
		printf("\tDiscard card %d in player %d\'s hand...\n", handPos, activePlayer);
		returnVal = discardCard(handPos, activePlayer, &testState, 0);
		printf("\t\tFunction returns 0:");
		domAssert(returnVal, 0, &numPass, &numFail);
		printf("\t\tHand count decremented:");
		domAssert(testState.handCount[activePlayer], originalHandCount - 1, &numPass, &numFail);
		printf("\t\tPosition in hand\n"
			"\t\treplaced with last\n"
			"\t\tcard (%d):\t", expectedCard);
		domAssert(testState.hand[activePlayer][handPos], expectedCard, &numPass, &numFail);
		printf("\t\tPlayer's discardCount\n\t\tincremented:\t");
		domAssert(testState.discardCount[activePlayer], originalDiscardCount + 1, &numPass, &numFail);
		printf("\t\tSelected card is last\n"
			"\t\titem in player's discard\n"
			"\t\tpile matches card:");
		domAssert(testState.discard[activePlayer][testState.discardCount[activePlayer] - 1], selectedCard, &numPass, &numFail);
		printf("\n");
	}

	// Optional State Printout to Console -----------------------------------------------
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	// ----------------------------------------------------------------------------------

	printf("Resetting game...\n\n");
	initializeGame(numPlayers, k, SEED, &testState);
	printf("\nGame reset.\n\n");

	// Optional State Printout to Console -----------------------------------------------
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	// ----------------------------------------------------------------------------------
	

	// Test 2: Discard to Played Cards Pile
	printf("Test 2: Discard with intent to move card to played cards pile.\n\n");
	handPos = testState.handCount[activePlayer] - 1;
	selectedCard = testState.hand[activePlayer][handPos];
	originalPlayedCardsCount = testState.playedCardCount;
	originalHandCount = testState.handCount[activePlayer];

	printf("\tDiscard card %d (last card) in player %d\'s hand...\n", handPos, activePlayer);
	returnVal = discardCard(handPos, activePlayer, &testState, 0);

	printf("\t\tFunction returns 0:");
	domAssert(returnVal, 0, &numPass, &numFail);

	printf("\t\tHand count decremented:");
	domAssert(testState.handCount[activePlayer], originalHandCount - 1, &numPass, &numFail);

	printf("\t\tPosition in hand = -1:");
	domAssert(testState.hand[activePlayer][handPos], -1, &numPass, &numFail);

	printf("\t\tPlayed Card Count\n\t\tincremented:\t");
	domAssert(testState.playedCardCount, originalPlayedCardsCount + 1, &numPass, &numFail);

	printf("\t\tSelected card is last\n"
		"\t\titem in played cards\n"
		"\t\tpile:\t\t");
	domAssert(testState.playedCards[testState.playedCardCount - 1], selectedCard, &numPass, &numFail);
	printf("\n");
	
	// Start at the beginning and keep discarding cards until hand is empty
	handPos = -1;
	while (testState.handCount[activePlayer] > 0) {
		handPos++;
		selectedCard = testState.hand[activePlayer][handPos];
		originalPlayedCardsCount = testState.playedCardCount;
		originalHandCount = testState.handCount[activePlayer];
		if (testState.handCount[activePlayer] > 1) {
			expectedCard = testState.hand[activePlayer][originalHandCount - 1];
		}
		else {
			expectedCard = -1;
		}
		printf("\tDiscard card %d in player %d\'s hand...\n", handPos, activePlayer);
		returnVal = discardCard(handPos, activePlayer, &testState, 0);
		printf("\t\tFunction returns 0:");
		domAssert(returnVal, 0, &numPass, &numFail);
		printf("\t\tHand count decremented:");
		domAssert(testState.handCount[activePlayer], originalHandCount - 1, &numPass, &numFail);
		printf("\t\tPosition in hand\n"
			"\t\treplaced with last\n"
			"\t\tcard (%d):\t", expectedCard);
		domAssert(testState.hand[activePlayer][handPos], expectedCard, &numPass, &numFail);
		printf("\t\tPlayed Card Count\n\t\tincremented:\t");
		domAssert(testState.playedCardCount, originalPlayedCardsCount + 1, &numPass, &numFail);
		printf("\t\tSelected card is last\n"
			"\t\titem in played cards\n"
			"\t\tpile:\t\t");
		domAssert(testState.playedCards[testState.playedCardCount - 1], selectedCard, &numPass, &numFail);
		printf("\n");
	}
	
	// Optional State Printout to Console -----------------------------------------------
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	// ----------------------------------------------------------------------------------

	printf("Resetting game...\n\n");
	initializeGame(numPlayers, k, SEED, &testState);
	printf("\nGame reset.\n\n");

	// Optional State Printout to Console -----------------------------------------------
	if (SHOW_GAME_STATES) {
		printf("-- GAME STATE TO TEST ---------------------------------------\n");
		showGameState(&testState, numPlayers);
	}
	// ----------------------------------------------------------------------------------


	// Test 3: Trash Card
	printf("Test 3: Trash card\n\n");
	handPos = testState.handCount[activePlayer] - 1;
	selectedCard = testState.hand[activePlayer][handPos];
	originalPlayedCardsCount = testState.playedCardCount;
	originalDiscardCount = testState.discardCount[activePlayer];
	originalHandCount = testState.handCount[activePlayer];

	printf("\tTrash card %d (last card) in player %d\'s hand...\n", handPos, activePlayer);
	returnVal = discardCard(handPos, activePlayer, &testState, 1);

	printf("\t\tFunction returns 0:");
	domAssert(returnVal, 0, &numPass, &numFail);

	printf("\t\tHand count decremented:");
	domAssert(testState.handCount[activePlayer], originalHandCount - 1, &numPass, &numFail);

	printf("\t\tPosition in hand = -1:");
	domAssert(testState.hand[activePlayer][handPos], -1, &numPass, &numFail);

	printf("\t\tPlayed Card Count\n\t\tNOT incremented:");
	domAssert(testState.playedCardCount, originalPlayedCardsCount, &numPass, &numFail);

	printf("\t\tDiscard Count\n\t\tNOT incremented:");
	domAssert(testState.discardCount[activePlayer], originalDiscardCount, &numPass, &numFail);

	// Start at the beginning and keep discarding cards until hand is empty
	handPos = -1;
	while (testState.handCount[activePlayer] > 0) {
		handPos++;
		selectedCard = testState.hand[activePlayer][handPos];
		originalPlayedCardsCount = testState.playedCardCount;
		originalDiscardCount = testState.discardCount[activePlayer];
		originalHandCount = testState.handCount[activePlayer];
		if (testState.handCount[activePlayer] > 1) {
			expectedCard = testState.hand[activePlayer][originalHandCount - 1];
		}
		else {
			expectedCard = -1;
		}
		printf("\tTrash card %d in player %d\'s hand...\n", handPos, activePlayer);
		returnVal = discardCard(handPos, activePlayer, &testState, 1);
		printf("\t\tFunction returns 0:");
		domAssert(returnVal, 0, &numPass, &numFail);
		printf("\t\tHand count decremented:");
		domAssert(testState.handCount[activePlayer], originalHandCount - 1, &numPass, &numFail);
		printf("\t\tPosition in hand\n"
			"\t\treplaced with last\n"
			"\t\tcard (%d):\t", expectedCard);
		domAssert(testState.hand[activePlayer][handPos], expectedCard, &numPass, &numFail);
		printf("\t\tPlayed Card Count\n\t\tNOT incremented:");
		domAssert(testState.playedCardCount, originalPlayedCardsCount, &numPass, &numFail);
		printf("\t\tDiscard Count NOT\n\t\tincremented:\t");
		domAssert(testState.discardCount[activePlayer], originalDiscardCount, &numPass, &numFail);
		printf("\n");
	}

	testComplete(&numPass, &numFail);

	return 0;
}