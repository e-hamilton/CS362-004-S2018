/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

/*
**		~     UNIT TEST 4     ~
**	File:				unittest4.c
**	Function Tested:	updateCoins()
**	Function Origin:	dominion_helpers.h, dominion.c
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "test_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

#define SHOW_GAME_STATES 0

#define MAX_BONUS 100
// Why 100?
// Cards that provide coin bonuses:
//		Tribute:	0-4
//		Baron:		4
//		Feast:		5
//		Minion:		0-2
//		Steward:	0-2
//		Cutpurse:	2
//		Embargo:	2
//		Salvager:	1 * max cost = 8
// In the extremely unlikely (and probably impossible) event that a player's hand
// contains all 8 Tributes, all 8 Salvagers, and 8 Provinces (cost = 8) before the
// isGameOver() function ends the game for having 3 empty Supply piles or an empty
// Province pile, the player could feasibly play the 8 Tributes in succession, 
// amassing +9 Actions after they are all played. The player could then play the 8 
// Salvagers to trash 8 Provinces, gaining 8*8 = +64 coins. The additional action 
// could potentially be used by the next-highest bonus card, Feast to provide +5
// coins for a total bonus of +69 coins. While this extreme is very unlikely, it
// illustrates that bonuses could enter the double digits for determined players.
// Therefore, I selected 100 as the upper limit for bonus test inputs.

int main()
{
	printf("-------------------- UNIT TEST 4: updateCoins() --------------------\n\n");

	int numPlayers = 2,
		activePlayer = 0,
		numPass = 0,
		numFail = 0,
		expectedCoins,
		handSize,
		bonusSize,
		handTestCount,
		bonusTestCount,
		i;

	// Define Test Values
	int numBonusTests = 6;
	int* bonusTests = malloc(numBonusTests * sizeof(int));
	bonusTests[0] = 0;
	bonusTests[1] = 1;
	bonusTests[2] = 5;
	bonusTests[3] = 10;
	bonusTests[4] = MAX_BONUS;
	bonusTests[5] = -1;
	int numHandTests = 5;
	int* handTests = malloc(numHandTests * sizeof(int));
	handTests[0] = 0;
	handTests[1] = 1;
	handTests[2] = 5;
	handTests[3] = 10;
	handTests[4] = MAX_HAND;

	printf("Beginning test with %d players...\n\n", numPlayers);

	//Declare test gameState struct
	struct gameState testState;

	// Specify kingdom cards to use. Which cards are chosen is irrelevant for this test.
	int k[10] = { adventurer, smithy, gardens, embargo, village, minion, mine, tribute,
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
	printf("For each test bonus value in the set {0, 1, 5, 10, MAX_BONUS, -1}...\n\n");
	printf("\tFor each hand size in the set handCount[activePlayer] =\n"
		"\t{0, 1, 5, 10, MAX_HAND}...\n\n");
	printf("\t\t1. Set hand to all copper.\n\n");
	printf("\t\t2. Set hand to all silver.\n\n");
	printf("\t\t3. Set hand to all gold.\n\n");
	printf("\t\t4. Set hand to no treasure.\n\n");
	printf("-------------------------------------------------------------------\n\n");

	//For each test bonus value in the test set...
	for (bonusTestCount = 0; bonusTestCount < numBonusTests; bonusTestCount++) {
		bonusSize = bonusTests[bonusTestCount];
		printf("-- BONUS = %d -----------------------------------------------------\n\n", bonusSize);
		//For each test hand size in the test set...
		for (handTestCount = 0; handTestCount < numHandTests; handTestCount++) {
			handSize = handTests[handTestCount];
			printf("\t HAND SIZE = %d\n\n", handSize);

			//Set hand size in test game state.
			testState.handCount[activePlayer] = handSize;

			//TEST 1: ALL COPPER
			for (i = 0; i < testState.handCount[activePlayer]; i++)
				testState.hand[activePlayer][i] = copper;
			printf("\t\tALL COPPER\n");
			expectedCoins = (handSize * 1) + bonusSize;
			printf("\t\tExpected Coins = %d * 1 + %d = %d\n", handSize, bonusSize, expectedCoins);
			updateCoins(activePlayer, &testState, bonusSize);
			printf("\t\tResult Coins = %d\n", testState.coins);
			domAssert(testState.coins, expectedCoins, &numPass, &numFail);
			printf("\n");

			//TEST 2: ALL SILVER
			for (i = 0; i < testState.handCount[activePlayer]; i++)
				testState.hand[activePlayer][i] = silver;
			printf("\t\tALL SILVER\n");
			expectedCoins = (handSize * 2) + bonusSize;
			printf("\t\tExpected Coins = %d * 2 + %d = %d\n", handSize, bonusSize, expectedCoins);
			updateCoins(activePlayer, &testState, bonusSize);
			printf("\t\tResult Coins = %d\n", testState.coins);
			domAssert(testState.coins, expectedCoins, &numPass, &numFail);
			printf("\n");

			//TEST 3: ALL GOLD
			for (i = 0; i < testState.handCount[activePlayer]; i++)
				testState.hand[activePlayer][i] = gold;
			printf("\t\tALL GOLD\n");
			expectedCoins = (handSize * 3) + bonusSize;
			printf("\t\tExpected Coins = %d * 3 + %d = %d\n", handSize, bonusSize, expectedCoins);
			updateCoins(activePlayer, &testState, bonusSize);
			printf("\t\tResult Coins = %d\n", testState.coins);
			domAssert(testState.coins, expectedCoins, &numPass, &numFail);
			printf("\n");

			//TEST 4: NO TREASURE
			for (i = 0; i < testState.handCount[activePlayer]; i++)
				testState.hand[activePlayer][i] = smithy;
			printf("\t\tNO TREASURE\n");
			expectedCoins = bonusSize;
			printf("\t\tExpected Coins = %d\n", expectedCoins);
			updateCoins(activePlayer, &testState, bonusSize);
			printf("\t\tResult Coins = %d\n", testState.coins);
			domAssert(testState.coins, expectedCoins, &numPass, &numFail);
			printf("\n");
		}
	}

	free(bonusTests);
	free(handTests);

	testComplete(&numPass, &numFail);

	return 0;
}