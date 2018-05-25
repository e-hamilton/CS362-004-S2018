/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

/*
**		~     UNIT TEST 1     ~
**	File:				unittest1.c
**	Function Tested:	isGameOver()
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
	printf("-------------------- UNIT TEST 1: isGameOver() --------------------\n\n");

	int numPlayers = 2;

	printf("Beginning test with %d players...\n\n", numPlayers);

	//Declare test gameState struct
	struct gameState testState;

	// Specify kingdom cards to use (I made sure to include the last 3 kingdom cards in
	// the CARD enum--salvager, sea_hag, and treasure_map-- to help ensure isGameOver is
	// looking at all of the piles).
	int k[10] = { adventurer, smithy, gardens, embargo, village, minion, mine, salvager,
		sea_hag, treasure_map };
	int result, expected, numPass = 0, numFail = 0;

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
	printf("Win Conditions:\n");
	printf("1. There are no more Province cards in the Supply.\n");
	printf("2. Three (3) or more Supply piles are empty (includes all piles).\n");
	printf("\n-------------------------------------------------------------------\n\n");


	//Possible permutations:
	//A. There are no more Province cards in the Supply, but fewer than 3 of the other piles are empty. (Expected = 1)
	//B. There are Province cards in the Supply, but at least 3 of the other piles are empty.	(Expected = 1)
	//C. There are no more Province cards in the Supply and at least 3 of the other piles are empty. (Expected = 1)
	//D. There Province cards in the Supply and fewer than 3 or the other piles are empty.	(Expected = 0)

	//Which piles to make empty?
	//First 3				(Expected = 1)
	//Last 3				(Expected = 1)
	//1 Victory (e.g. Estate), 1 Treasure (e.g. Copper), and 1 Action (e.g. Smithy)		(Expected = 1)
	//All piles		(Expected = 1)

	//How to define not empty?
	//Pile is full
	//Pile has 1 card

	// Case A - Only Province pile is empty.
	printf("Test 1: Only Province pile is empty.\n\n");
	testState.supplyCount[province] = 0;
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	// Case B - 3 Supply piles are empty.
	printf("Test 2: 3 Supply piles are empty...\n\n");

	printf("\t2.1 First 3 piles {curse, estate, duchy} empty.\n\n");
	testState.supplyCount[curse] = 0;
	testState.supplyCount[estate] = 0;
	testState.supplyCount[duchy] = 0;
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	printf("\t2.2 Last 3 piles {salvager, sea_hag, treasure_map} empty.\n\n");
	testState.supplyCount[salvager] = 0;
	testState.supplyCount[sea_hag] = 0;
	testState.supplyCount[treasure_map] = 0;
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	printf("\t2.3 1 Victory, 1 Treasure, 1 Action {copper, estate, smithy}\n\tempty.\n\n");
	testState.supplyCount[copper] = 0;
	testState.supplyCount[estate] = 0;
	testState.supplyCount[smithy] = 0;
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	printf("\t2.4 3 piles, including province, {estate, province, smithy}\n\tare empty.\n\n");
	testState.supplyCount[estate] = 0;
	testState.supplyCount[province] = 0;
	testState.supplyCount[smithy] = 0;
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	// Case C - 3 Supply piles are empty AND Province pile is empty
	printf("Test 3: 3 Supply piles are empty AND Province pile is empty...\n\n");

	printf("\t3.1 Province empty AND first 3 piles {curse, estate, duchy}\n\tempty.\n\n");
	testState.supplyCount[province] = 0;
	testState.supplyCount[curse] = 0;
	testState.supplyCount[estate] = 0;
	testState.supplyCount[duchy] = 0;
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	printf("\t3.2 Province empty AND last 3 piles {salvager, sea_hag,\n\ttreasure_map} empty.\n\n");
	testState.supplyCount[province] = 0;
	testState.supplyCount[salvager] = 0;
	testState.supplyCount[sea_hag] = 0;
	testState.supplyCount[treasure_map] = 0;
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	printf("\t3.3 Province empty AND 1 Victory, 1 Treasure, 1 Action\n\t{copper, estate, smithy} empty.\n\n");
	testState.supplyCount[province] = 0;
	testState.supplyCount[copper] = 0;
	testState.supplyCount[estate] = 0;
	testState.supplyCount[smithy] = 0;
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	printf("\t3.4 All piles empty.\n\n");
	int i;
	for (i = 0; i < treasure_map + 1; i++) {
		testState.supplyCount[i] = 0;
	}
	result = isGameOver(&testState);
	expected = 1;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	// Case D - <3 Supply piles are empty and Province pile is NOT empty (game not over)
	printf("Test 4: <3 Supply piles are empty and Province pile is NOT empty\n(game not over)...\n\n");

	printf("\t4.1 All piles are full.\n\n");
	result = isGameOver(&testState);
	expected = 0;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	printf("\t4.2 All piles have at least one card.\n\n");
	for (i = 0; i < treasure_map + 1; i++) {
		testState.supplyCount[i] = 1;
	}
	result = isGameOver(&testState);
	expected = 0;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	printf("\t4.3 Only 2 piles {adventurer, smithy} are empty.\n\n");
	testState.supplyCount[adventurer] = 0;
	testState.supplyCount[smithy] = 0;
	result = isGameOver(&testState);
	expected = 0;
	printf("\tExpected result: %d\n", expected);
	printf("\tResult: %d\n", result);
	domAssert(result, expected, &numPass, &numFail);
	resetSupply(&testState, k, numPlayers);	//reset
	printf("\n");

	testComplete(&numPass, &numFail);
	
	return 0;
}