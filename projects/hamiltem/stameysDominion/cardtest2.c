/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

/*
**		~     CARD TEST 2     ~
**	File:				cardtest2.c
**	Card Tested:		Adventurer
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

// Adventurer Objective:
// 1. Draw cards from player's deck until 2 treasure cards are uncovered.
// 2. Add these cards to the player's hand.
// 3. Add all other drawn cards to player's discard pile, incrementing
//	  discardCount accordingly.
// 4. Call discardCard() to move played Adventurer card to playedCards
//	  pile, incrementing playedCardCount accordingly

int main()
{
	printf("--------------------- CARD TEST 2: ADVENTURER ---------------------\n\n");

	int numPlayers = 2,
		activePlayer = 0,
		handPos = 0,
		numPass = 0,
		numFail = 0,
		deckOp = 0,
		drawnCards = 0,
		result,
		expected,
		i;

	char* cardname = "Adventurer";

	//Declare test gameState structs
	struct gameState testState, oracle;

	// Specify kingdom cards to use. Which cards are chosen is irrelevant for this test,
	// as long as one of them is adventurer.
	int k[10] = { adventurer, smithy, gardens, embargo, village, minion, mine, tribute,
		sea_hag, treasure_map };

	printf("-------------------------------------------------------------------\n\n");
	printf("Test Plan:\n\n");
	printf("For both a deck containing alternating treasures and\n"
		"non-treasures and a deck containing no treasure:\n\n");
	printf("\t1. Player\'s hand size has been increased by 2\n"
		"\tTreasures - 1 Adventurer = 1 Total Cards if using\n"
		"\tdeck with 2+ treasures; otherwise Player\'s hand size\n"
		"\tis decreased by 1 if no Treasures are found\n\n");
	printf("\t\t1.a All other players\' hand sizes are unchanged.\n\n");
	printf("\t2. Correct player\'s deck size has been decreased by\n"
		"\tat least 2.\n\n");
	printf("\t\t2.a All other players\' deck sizes are unchanged.\n\n");
	printf("\t3. Player\'s discardCount = # drawn cards - 2 if\n"
		"\tusing deck with 2+ treasures; otherwise Player\'s\n"
		"\tdiscardCount = # drawn cards.\n\n");
	printf("\t4. playedCardCount has been incremented\n\n");
	printf("\t5. Last item in playedCards array is adventurer\n\n");
	printf("-------------------------------------------------------------------\n\n");

	
	// For all possible numbers of players
	for (numPlayers = 2; numPlayers <= MAX_PLAYERS; numPlayers++) {

		printf("Beginning test with %d players...\n\n", numPlayers);

		deckOp = 0;
		while (deckOp < 2) {

			//Initialize player hand and deck

			if (deckOp == 0) {
				// Deck with Treasure
				printf("\n*****\n");
				printf("Testing Deck with 2+ Treasure Cards\n");
				printf("*****\n\n");

				//Initialize test game state
				printf("Initializing game...\n\n");
				initializeGame(numPlayers, k, SEED, &testState);

				//Give active player an adventurer at handPos and fill the rest of the hand with coppers;
				testState.handCount[activePlayer] = 5;
				for (i = 0; i < testState.handCount[activePlayer]; i++)
					testState.hand[activePlayer][i] = copper;
				testState.hand[activePlayer][handPos] = adventurer;

				//Fill player's deck with alternating sea hags and copper
				testState.deckCount[activePlayer] = 10;
				printf("Player's deck count: %d\n\n", testState.deckCount[activePlayer]);
				for (i = 0; i < testState.deckCount[activePlayer]; i++) {
					if (i % 2 == 0)
						testState.deck[activePlayer][i] = copper;
					else
						testState.deck[activePlayer][i] = sea_hag;
				}

				//Copy all values to oracle
				memcpy(&oracle, &testState, sizeof(struct gameState));
				
				//showGameState(&testState, numPlayers);
				printf("\nGame initialized.\n\n");
			}
			else {
				// Deck with no Treasure
				printf("\n*****\n");
				printf("Testing Deck with NO Treasure Cards\n");
				printf("*****\n\n");

				//Initialize test game state
				printf("Initializing game...\n\n");
				initializeGame(numPlayers, k, SEED, &testState);

				//Give active player a smithy at handPos and fill the rest of the hand with coppers;
				testState.handCount[activePlayer] = 5;
				for (i = 0; i < testState.handCount[activePlayer]; i++)
					testState.hand[activePlayer][i] = copper;
				testState.hand[activePlayer][handPos] = adventurer;
				
				//Fill player's deck with sea hags
				testState.deckCount[activePlayer] = 10;
				printf("Player's deck count: %d\n\n", testState.deckCount[activePlayer]);
				for (i = 0; i < testState.deckCount[activePlayer]; i++)
					testState.deck[activePlayer][i] = sea_hag;
				
				//Copy all values to oracle
				memcpy(&oracle, &testState, sizeof(struct gameState));
				
				//showGameState(&testState, numPlayers);
				printf("\nGame initialized.\n\n");
			}
			
			
			printf("PLAYER %d PLAYS %s FROM HAND POSITION %d IN %d-PLAYER GAME\n\n",
				activePlayer, cardname, handPos, numPlayers);

			cardEffect(adventurer, -1, -1, -1, &testState, handPos, 0);

			// Test 1: Players hand has been updated appropriately
			printf("\nTEST 1: Hand count updated appropriately\n");
			result = testState.handCount[activePlayer];
			if (deckOp == 0)
				expected = oracle.handCount[activePlayer] + 1;
			else
				expected = oracle.handCount[activePlayer] - 1;
			printf("\ttest handCount = %d\n", result);
			printf("\texpected handCount = %d\n\t\t\t", expected);
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");

			// Test 1a: Other players' hand counts were not changed
			for (i = 1; i < numPlayers; i++) {
				result = testState.handCount[i];
				expected = oracle.handCount[i];
				printf("\tPlayer %d\'s hand count\n"
					"\thas not changed:", i);
				domAssert(result, expected, &numPass, &numFail);
			}
			printf("\n");

			// Test 2: Player's deck size has been updated appropriately
			printf("TEST 2: Player %d\'s deck is decreased appropriately.\n", activePlayer);
			if (deckOp == 0) {
				result = (testState.deckCount[activePlayer] <= (oracle.deckCount[activePlayer] - 2));
				printf("\tPlayer %d\'s deck count\n"
					"\tdecreased by >= 2:", activePlayer);
				expected = 1;
			}
			else {
				result = testState.deckCount[activePlayer];
				printf("\tPlayer %d\'s deck count\n"
					"\texhausted (is now 0):", activePlayer);
				expected = 0;
			}
			domAssert(result, expected, &numPass, &numFail);

			// Test 2a: Other players' deck counts were not changed
			for (i = 1; i < numPlayers; i++) {
				result = testState.deckCount[i];
				expected = oracle.deckCount[i];
				printf("\tPlayer %d\'s deck count\n"
					"\thas not changed:", i);
				domAssert(result, expected, &numPass, &numFail);
			}
			printf("\n");

			// Test 3: Player's discardCount is updated appropriately
			printf("TEST 3: Player %d\'s discardCount is updated appropriately.\n", activePlayer);
			if (deckOp == 0) {
				drawnCards = oracle.deckCount[activePlayer] - testState.deckCount[activePlayer];
				result = testState.discardCount[activePlayer];
				expected = drawnCards - 2;
				printf("\tPlayer %d\'s discardCount\n"
					"\tequals # drawn cards - 2\n", activePlayer);
				printf("\t# drawn cards = %d\n", drawnCards);
				printf("\tdiscardCount = %d\n\t\t\t", testState.discardCount[activePlayer]);
				domAssert(result, expected, &numPass, &numFail);
				printf("\n");

				result = testState.discard[activePlayer][0];
				expected = sea_hag;
				printf("\tFirst discarded card\n"
					"\t%d == sea hag:", result);
				domAssert(result, expected, &numPass, &numFail);
				printf("\n");

				result = testState.discard[activePlayer][testState.discardCount[activePlayer] - 1];
				expected = sea_hag;
				printf("\tLast discarded card\n"
					"\t%d == sea hag:\t", result);
				domAssert(result, expected, &numPass, &numFail);
				printf("\n");
			}
			else {
				drawnCards = oracle.deckCount[activePlayer] - testState.deckCount[activePlayer];
				result = testState.discardCount[activePlayer];
				expected = drawnCards;
				printf("\tPlayer %d\'s discardCount\n"
					"\tequals # drawn cards\n", activePlayer);
				printf("\t# drawn cards = %d\n", drawnCards);
				printf("\tdiscardCount = %d\n\t\t\t", testState.discardCount[activePlayer]);
				domAssert(result, expected, &numPass, &numFail);
				printf("\n");
			}

			// Test 4: playedCardCount has been incremented
			printf("TEST 4: PlayedCardCount is updated appropriately.\n");
			result = testState.playedCardCount;
			expected = oracle.playedCardCount + 1;
			printf("\ttest playedCardCount = %d\n", result);
			printf("\texpected playedCardCount = %d\n\t\t\t", expected);
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");

			// Test 5: Last item in playedCards array is adventurer
			printf("TEST 5: Last item in playedCards array is %s.\n\t\t\t", cardname);
			if (testState.playedCardCount == 0)
				result = testState.playedCards[0];
			else
				result = testState.playedCards[testState.playedCardCount - 1];
			expected = adventurer;
			domAssert(result, expected, &numPass, &numFail);
			printf("\n");

			deckOp++;
		}

		printf("-------------------------------------------------------------------\n\n");
	}
	
	testComplete(&numPass, &numFail);

	return 0;
}
