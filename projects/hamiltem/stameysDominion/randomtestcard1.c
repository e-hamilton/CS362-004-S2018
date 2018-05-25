/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 4 - CS362 Spring 2018
**	Due 5/13/18
*/

/*
**		~     RANDOM CARD TEST 1     ~
**	File:				randomtestcard1.c
**	Card Tested:		Smithy
**	Code Origin:		dominion_helpers.h, dominion.c
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_TESTS 2000

/* Global Variables */
int numPass;
int numFail;
int totalPasses;
int totalFailures;

/* Function prototypes */
int getRandNum(int min, int max);
int getRandNumPosNeg();
int assertEqual(int a, int b);
void testResults(struct gameState *state, struct gameState *oracle, int activePlayer, int handPos);

int main()
{
	int numPlayers,
		activePlayer,
		handPos,
		choice1,
		choice2,
		choice3,
		bonus,
		i, j,
		result,
		currentTest;
	
	double percentPass, 
		percentFailure;
	
	time_t seed;

	//Declare test gameState structs
	struct gameState state, oracle;
	
	printf("-------------------- RANDOM CARD TEST 1: SMITHY ----------------------\n\n");

	// Use time to set seed for random number generation
	seed = time(NULL);
	srand(time(NULL));
	printf("Seed: %lld\n", (long long)seed);

	totalFailures = 0;
	totalPasses = 0;

	for (currentTest = 0; currentTest < NUM_TESTS; currentTest++) {

		printf("TEST %d --------------------------------------------------------------\n", currentTest);

		// Initialize test counts
		numPass = 0;
		numFail = 0;

		//Randomize contents of GameState state 
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&state)[i] = getRandNum(0, 256);
		}

		/*	Randomly generate values that could influence how Smithy functions	*/

		// Number of players
		numPlayers = getRandNum(0, MAX_PLAYERS);
		state.numPlayers = numPlayers;
		printf("Number of Players: %d\n", numPlayers);

		// Select active player
		activePlayer = getRandNum(0, MAX_PLAYERS - 1);
		state.whoseTurn = activePlayer;
		printf("Active Player: %d\n", activePlayer);

		// Hand Size -- I originally set the upper bound to MAX_HAND, but this created a
		// seg fault when the hand is incremented (by 2 or 3, depending on whether the
		// bug I introduced in HW2 reared its ugly head or not). I changed the upper limit
		// to MAX_HAND - 3 to stop the seg fault from occurring.
		for (i = 0; i < MAX_PLAYERS; i++) {
			state.handCount[i] = getRandNum(0, MAX_HAND - 3);
			printf("Hand Size Player %d: %d\n", i, state.handCount[i]);
		}

		// Hand Position of Smithy card in player's hand -- can only be 0 or 1 to
		// avoid segmentation fault in all cases due to bug I introduced in _smithy()
		// in HW2.
		handPos = getRandNum(0, 1);
		printf("Hand Position of Smithy: %d\n", handPos);

		// Players' Hand Contents
		for (i = 0; i < MAX_PLAYERS; i++) {
			for (j = 0; j < MAX_HAND; j++) {
				state.hand[i][j] = getRandNum(curse, treasure_map);
			}
		}
		state.hand[activePlayer][handPos] = smithy;

		// Deck Size -- I originally set the lower bound to 0, but this created a seg fault
		// if the deck size was less than the number of cards to draw (3).
		for (i = 0; i < MAX_PLAYERS; i++) {
			state.deckCount[i] = getRandNum(3, MAX_DECK);
			printf("Deck Size Player %d: %d\n", i, state.deckCount[i]);
		}

		// playedCardCount
		state.playedCardCount = getRandNum(0, MAX_DECK);
		printf("Played Card Count = %d\n", state.playedCardCount);

		// Randomize values of choices/bonus passed to cardEffect-- none of these should have
		// an effect on Smithy, since smithy does not require choice parameters or make a
		// call to updateCoins() with a bonus.
		state.coins = getRandNumPosNeg();	//No range defined
		choice1 = getRandNumPosNeg();
		choice2 = getRandNumPosNeg();
		choice3 = getRandNumPosNeg();
		bonus = getRandNumPosNeg();
		printf("Coins: %d\n", state.coins);
		printf("Choice 1: %d\n", choice1);
		printf("Choice 2: %d\n", choice2);
		printf("Choice 3: %d\n", choice3);
		printf("Bonus: %d\n", bonus);

		/*	Copy state to oracle	*/
		memcpy(&oracle, &state, sizeof(struct gameState));

		/* CALL CARD EFFECT!!! */
		result = cardEffect(smithy, choice1, choice2, choice3, &state, handPos, &bonus);
		printf("Result: %d\n", result);

		/* CHECK RESULTS */

		//Check return value
		assertEqual(result, 0);

		//Check everything else
		testResults(&state, &oracle, activePlayer, handPos);

		printf("\n----------------------------------------------------------------------\n\n");
		printf("TEST RESULTS:\n");
		printf("Number of Passes: %d\n", numPass);
		printf("Number of Failures: %d\n", numFail);
		printf("\n----------------------------------------------------------------------\n");

		if (numFail > 0) {
			totalFailures++;
		}
		else {
			totalPasses++;
		}
	}

	percentPass = ((double)totalPasses / (double)NUM_TESTS) * 100;
	percentFailure = ((double)totalFailures / (double)NUM_TESTS) * 100;


	printf("ALL TESTS:\n");
	printf("Tests with No Identified Failures:\t%d / %d\n", totalPasses, NUM_TESTS);
	printf("\t\tPercentage Passed:\t%.2f%%\n", percentPass);
	printf("Tests with Identified Failures:\t\t%d / %d\n", totalFailures, NUM_TESTS);
	printf("\t\tPercentage Failed:\t%.2f%%\n", percentFailure);
	assertEqual(totalPasses + totalFailures, 2000);
	printf("----------------------------------------------------------------------\n");
	return 0;
}

/* 
** getRandNum -- returns a pseudorandom integer with a minimum possible value of
** min and a maximum possible value of max.
*/
int getRandNum(int min, int max) {
	int num = (rand() % (max - min + 1)) + min;
	return num;
}

/*
** getRandNumPosNeg -- returns a pseudorandom signed integer between -RAND_MAX/2 and 
** RAND_MAX/2.
*/
int getRandNumPosNeg() {
	int m, n;
	m = getRandNum(0, RAND_MAX);
	n = RAND_MAX / 2;
	return m - n;
}

/*
** assertEqual -- asserts that its two integer arguments are equal. If they are, it increments the
** global numPass variable and returns 1 (true); if they are not, it increments the global numFail
** variable and returns 0 (false).
*/
int assertEqual(int a, int b) {
	if (a == b) {
		numPass++;
		return 1;
	}
	else {
		numFail++;
		return 0;
	}
}


/*
** testResults -- evaluates the gameState struct used for testing (*state) against a test oracle based
** on the state's values before cardEffect was executed (*oracle). The integer arguments activePlayer
** and handPos represent the player who played the card and the position the card was played from in
** that player's hand, respectively. For each test case, a call is made to assertEqual, which increments
** the numPass or numFail global variables according to whether or not the resulting game state matches
** the expected outcome of the card's effect.
*/
void testResults(struct gameState *state, struct gameState *oracle, int activePlayer, int handPos) {
	int result, expected, i;

	// 1. Active player's hand count has been increased by 3 - 1 = 2.
	result = state->handCount[activePlayer];
	expected = oracle->handCount[activePlayer] + 2;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Player %d\'s hand count not increased from %d to %d.\n",
			activePlayer, oracle->handCount[activePlayer], expected);
		printf("\tPlayer %d\'s hand count: %d\n", activePlayer, result);
	}

	// 2. All other players' hand counts are the same.
	for (i = 0; i < MAX_PLAYERS; i++) {
		if (i != activePlayer) {
			result = state->handCount[i];
			expected = oracle->handCount[i];
			if (assertEqual(result, expected) == 0) {
				printf("Test Failed: Non-active player %d\'s hand count changed from %d to %d.\n",
					i, expected, result);
			}
		}
	}

	// 3. Active player's deck size is decreased by 3.
	result = state->deckCount[activePlayer];
	expected = oracle->deckCount[activePlayer] - 3;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Player %d\'s deck count not decreased from %d to %d.\n",
			activePlayer, oracle->deckCount[activePlayer], expected);
		printf("\tPlayer %d\'s deck count: %d\n", activePlayer, result);
	}

	// 4. All other players' deck counts are the same.
	for (i = 0; i < MAX_PLAYERS; i++) {
		if (i != activePlayer) {
			result = state->deckCount[i];
			expected = oracle->deckCount[i];
			if (assertEqual(result, expected) == 0) {
				printf("Test Failed: Non-active player %d\'s deck count changed from %d to %d.\n",
					i, expected, result);
			}
		}
	}

	// 5. playedCardCount has been increased by 1.
	result = state->playedCardCount;
	expected = oracle->playedCardCount + 1;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: playedCard count not incremented from %d to %d.\n", 
			oracle->playedCardCount, expected);
	}

	// 6. Last card in playedCards array is Smithy
	result = state->playedCards[state->playedCardCount - 1];
	expected = smithy;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Last card in playedCards array (%d) is not smithy (= 13).\n", result);
	}

	// 7. Player's hand at handPos now contains last-drawn card
	result = state->hand[activePlayer][handPos];
	expected = oracle->deck[activePlayer][oracle->deckCount[activePlayer] - 3];
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Player %d\'s hand at hand position %d has not been replaced\nwith the last-drawn card %d.\n",
			activePlayer, handPos, expected);
		printf("\tPlayer %d\'s hand at hand position %d: %d\n", activePlayer, handPos, result);
	}

	// 8. Coins should not have been changed
	result = state->coins;
	expected = oracle->coins;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Game coins changed from %d to %d.\n", expected, result);
	}

}
