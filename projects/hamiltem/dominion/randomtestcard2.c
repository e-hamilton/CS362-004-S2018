/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 4 - CS362 Spring 2018
**	Due 5/13/18
*/

/*
**		~     RANDOM CARD TEST 2     ~
**	File:				randomtestcard2.c
**	Card Tested:		Embargo
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
void testResults(struct gameState *state, struct gameState *oracle, int activePlayer, int handPos, int embargoChoice, int validEmbargoPile);

int main()
{
	int numPlayers,
		activePlayer,
		handPos,
		choice1,
		choice2,
		choice3,
		bonus,
		i,
		validEmbargoPile,
		result,
		currentTest;
	
	double percentPass, 
		percentFailure;
	
	time_t seed;
	
	//Declare test gameState structs
	struct gameState state, oracle;

	printf("-------------------- RANDOM CARD TEST 2: EMBARGO ---------------------\n\n");

	// Use time to set seed for random number generation
	seed = time(NULL);
	srand(time(NULL));
	printf("Seed: %lld\n", (long long) seed);

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

		/*	Randomly generate values that could influence how Embargo functions	*/

		// Number of players
		numPlayers = getRandNum(0, MAX_PLAYERS);
		state.numPlayers = numPlayers;
		printf("Number of Players: %d\n", numPlayers);

		// Select active player
		activePlayer = getRandNum(0, MAX_PLAYERS - 1);
		state.whoseTurn = activePlayer;
		printf("Active Player: %d\n", activePlayer);

		// Players' Hand Counts: Since the player should not be acquiring any new cards
		// from playing Embargo, it is safe to set the upper limit to MAX_HAND.
		for (i = 0; i < MAX_PLAYERS; i++) {
			state.handCount[i] = getRandNum(0, MAX_HAND);
			printf("Hand Size Player %d: %d\n", i, state.handCount[i]);
		}

		// Hand Position of Embargo card in player's hand (maximum possible index is
		// MAX_HAND - 1; cannot be negative or it will cause memory access violations)
		handPos = getRandNum(0, MAX_HAND - 1);
		printf("Hand Position of Embargo: %d\n", handPos);
		state.hand[activePlayer][handPos] = embargo;

		// Supply counts: I chose to test random negative integers beyond -1, even though dominion.c
		// only considers a card to not be in play if the supplyCount == -1, not necessarily
		// if the supplyCount is negative. I'm not sure if this is a bug, but it's definitely
		// a vulnerability if the game somehow ends up in a bad state. However, doing this all the
		// time made it difficult to detect the cases where the supply count was -1, which Embargo
		// is expecting to look for. Therefore, I manipulated the probability so that if a supplyCount
		// is given a negative number, about half the time it will be -1 and the other half the time
		// it will be some garbage negative
		for (i = curse; i <= treasure_map; i++) {
			state.supplyCount[i] = getRandNumPosNeg();
			// Manipulate probability so about half of negative values are -1
			if (state.supplyCount[i] < 0) {
				if (getRandNum(0, 1) == 0) {
					state.supplyCount[i] = -1;
				}
			}
		}
				
		// Players' Discard Counts: I set the upper limit to MAX_DECK - 1 to allow room
		// for one additional card just in case Embargo isn't trashed correctly and added
		// to the player's discard array
		for (i = 0; i < MAX_PLAYERS; i++) {
			state.discardCount[i] = getRandNum(0, MAX_DECK - 1);
			printf("Discard Count Player %d: %d\n", i, state.discardCount[i]);
		}

		// playedCardCount: I set the upper limit to MAX_DECK - 1 to allow room for one
		// additional card just in case Embargo isn't trashed correctly and added to the 
		// playedCards array.
		state.playedCardCount = getRandNum(0, MAX_DECK - 1);
		printf("Played Card Count = %d\n", state.playedCardCount);

		// choice1 = embargo card choice (Must be a valid card as it is utilized in as
		// an array index in _embargo(). Using an invalid value would cause a memory
		// accessviolation and the program would crash.
		choice1 = getRandNum(curse, treasure_map);
		printf("Embargo Choice: %d\n", choice1);

		// Coins -- shouldn't matter what the value is initially
		state.coins = getRandNumPosNeg();

		// Randomize values of other choices/bonus passed to cardEffect-- none of these should have
		// an effect on Embargo, since it is only supposed to evaluate choice1
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

		// FIRST THINGS FIRST... check if choice1 has a valid supplyCount or not (i.e. nonnegative). 
		// This is how Embargo evaluates if the card is in play in the current game or not.
		printf("SupplyCount for choice1: %d\n", state.supplyCount[choice1]);
		if (state.supplyCount[choice1] >= 0) {
			// Card should be considered valid.
			validEmbargoPile = 1;
			printf("\tVALID: Supply count is non-negative.\n");
		}
		else {
			validEmbargoPile = 0;
			printf("\tNOT VALID: Supply count is negative.\n");
		}

		/* CALL CARD EFFECT!!! */
		result = cardEffect(embargo, choice1, choice2, choice3, &state, handPos, &bonus);
		printf("Result: %d\n", result);

		/* CHECK RESULTS */

		//Check return value -- Embargo should return 0 if the choice was valid and -1 otherwise
		if (validEmbargoPile == 1) {
			assertEqual(result, 0);
		}
		else {
			assertEqual(result, -1);
		}

		//Check everything else
		testResults(&state, &oracle, activePlayer, handPos, choice1, validEmbargoPile);

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
** that player's hand, respectively. The integers embargoChoice and validEmbargoPile represent the pile
** the player chose to embargo and whether or not that pile is valid in terms of supplyCount (it was
** initially given a nonnegative value). For each test case, a call is made to assertEqual, which 
** increments the numPass or numFail global variables according to whether or not the resulting game
** state matches the expected outcome of the card's effect.
*/
void testResults(struct gameState *state, struct gameState *oracle, int activePlayer, int handPos, int embargoChoice, int validEmbargoPile) {
	int result, expected, i;
	
	// Tests for a valid pile
	if (validEmbargoPile == 1) {
		// 1. Coins should be increased by 2.
		result = state->coins;
		expected = oracle->coins + 2;
		if (assertEqual(result, expected) == 0) {
			printf("Test Failed: Coins not increased from %d to %d.\n", oracle->coins, expected);
			printf("\tCurrent Coins: %d\n", result);
		}

		// 2. embargoTokens[embargoChoice] is incremented by 1.
		result = state->embargoTokens[embargoChoice];
		expected = oracle->embargoTokens[embargoChoice] + 1;
		if (assertEqual(result, expected) == 0) {
			printf("Test Failed: Embargo Tokens for choice %d were not incremented from %d to %d.\n",
				embargoChoice, oracle->embargoTokens[embargoChoice], expected);
			printf("\tembargoTokens[%d] = %d\n", embargoChoice, result);
		}

		// 3. embargoTokens for embargoChoice should be incremented by 1; every other index should be the same 
		for (i = 0; i <= treasure_map; i++) {
			if (i == embargoChoice) {
				result = state->embargoTokens[embargoChoice];
				expected = oracle->embargoTokens[embargoChoice] + 1;
				if (assertEqual(result, expected) == 0) {
					printf("Test Failed: Embargo Tokens for choice %d were not incremented from %d to %d.\n",
						embargoChoice, oracle->embargoTokens[embargoChoice], expected);
					printf("\tembargoTokens[%d] = %d\n", embargoChoice, result);
				}
			}
			else {
				result = state->embargoTokens[i];
				expected = oracle->embargoTokens[i];
				if (assertEqual(result, expected) == 0) {
					printf("Test Failed: embargoTokens at index %d changed unexpectedly from %d to %d.\n",
						i, expected, result);
				}
			}
		}

		// 4. Player's hand count was decremented; other players' hand counts were not.
		for (i = 0; i < MAX_PLAYERS; i++) {
			if (i == activePlayer) {
				result = state->handCount[activePlayer];
				expected = oracle->handCount[activePlayer] - 1;
				if (assertEqual(result, expected) == 0) {
					printf("Test Failed: Player %d\'s hand count was not decremented from %d to %d.\n",
						activePlayer, oracle->handCount[activePlayer], expected);
					printf("\tPlayer %d\'s hand count: %d\n", activePlayer, result);
				}
			}
			else {
				result = state->handCount[i];
				expected = oracle->handCount[i];
				if (assertEqual(result, expected) == 0) {
					printf("Test Failed: Player %d\'s hand count changed unexpectedly from %d to %d.\n",
						i, expected, result);
				}
			}
		}

		// 5.  Embargo card in player's hand at handPos has been replaced with the last card in the hand
		result = state->hand[activePlayer][handPos];
		expected = oracle->hand[activePlayer][oracle->handCount[activePlayer] - 1];
		if (assertEqual(result, expected) == 0) {
			printf("Test Failed: Player %d\'s hand at position %d has not been replaced.\n",
				activePlayer, handPos);
			printf("\thand[%d]: %d\n", handPos, result);
		}

	}
	// Tests for an invalid pile
	else {
		// 1. Coins are the same
		result = state->coins;
		expected = oracle->coins;
		if (assertEqual(result, expected) == 0) {
			printf("Test Failed: Coins increased unexpectedly from %d to %d.\n", oracle->coins, result);
		}

		// 2. embargoTokens at every index are still the same
		for (i = 0; i <= treasure_map; i++) {
			result = state->embargoTokens[i];
			expected = oracle->embargoTokens[i];
			if (assertEqual(result, expected) == 0) {
				printf("Test Failed: embargoTokens at index %d changed unexpectedly from %d to %d.\n",
					i, expected, result);
			}
		}

		// 3. All players' handCounts are the same
		for (i = 0; i < MAX_PLAYERS; i++) {
			result = state->handCount[i];
			expected = oracle->handCount[i];
			if (assertEqual(result, expected) == 0) {
				printf("Test Failed: Player %d\'s hand count changed unexpectedly from %d to %d.\n",
					i, expected, result);
			}
		}

		// 4. The player's hand at handPos is still embargo
		result = state->hand[activePlayer][handPos];
		expected = embargo;
		if (assertEqual(result, expected) == 0) {
			printf("Test Failed: Player %d\'s hand at position %d is no longer Embargo.\n",
				activePlayer, handPos);
			printf("\thand[%d]: %d\n", handPos, result);
		}
	}

	// For BOTH valid and invalid embargo piles: 
	
	// 1. No player's discardCount has changed.
	for (i = 0; i < MAX_PLAYERS; i++) {
		result = state->discardCount[i];
		expected = oracle->discardCount[i];
		if (assertEqual(result, expected) == 0) {
			printf("Test Failed: Player %d\'s discard count changed unexpectedly from %d to %d.\n",
				i, expected, result);
		}
	}

	// 2. The playedCardCount did not change
	result = state->playedCardCount;
	expected = oracle->playedCardCount;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: The played card count changed unexpectedly from %d to %d.\n",
			expected, result);
	}
}
