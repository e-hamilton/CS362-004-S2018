/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 4 - CS362 Spring 2018
**	Due 5/13/18
*/

/*
**		~     RANDOM CARD TEST ADVENTURER     ~
**	File:				randomtestadventurer.c
**	Card Tested:		Adventurer
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
void testResults(struct gameState *state, struct gameState *oracle, int activePlayer, int handPos, int availableTreasureDeck);

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
		currentTest,
		availableTreasureDeck,
		availableTreasureDiscard,
		randTreasureCard;
	
	double percentPass, 
		percentFailure;
	
	time_t seed;

	//Declare test gameState structs
	struct gameState state, oracle;
	
	printf("-------------------- RANDOM CARD TEST ADVENTURER ---------------------\n\n");

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

		/*	Randomly generate values that could influence how Adventurer functions	*/

		// Number of players
		numPlayers = getRandNum(0, MAX_PLAYERS);
		state.numPlayers = numPlayers;
		printf("Number of Players: %d\n", numPlayers);

		// Select active player
		activePlayer = getRandNum(0, MAX_PLAYERS - 1);
		state.whoseTurn = activePlayer;
		printf("Active Player: %d\n", activePlayer);

		// Players' Hand Counts: I originally set the upper bounds of this range to MAX_HAND
		// - 2 to accommodate at most 2 additional treasure cards added to the player's hand,
		// but a bug in Adventurer (where the function does not check for the return value of
		// drawnCard) can lead cards from the player's hand to be moved from the player's hand
		// to tempHand, which has a size of 500 = MAX_HAND = MAX_DECK. Since cards can feasibly
		// be funneled into tempHand from the player's deck, hand, or discard arrays, I saw
		// the need to restrict the maximum possible count for each to MAX_HAND / 3. 
		for (i = 0; i < MAX_PLAYERS; i++) {
			state.handCount[i] = getRandNum(0, MAX_HAND / 3);
			printf("Hand Size Player %d: %d\n", i, state.handCount[i]);
		}

		// Hand Position of Adventurer card in player's hand (maximum possible index is
		// MAX_HAND - 1; cannot be negative or it will cause memory access violations)
		handPos = getRandNum(0, MAX_HAND - 1);
		printf("Hand Position of Adventurer: %d\n", handPos);

		// Players' Hand Contents
		for (i = 0; i < MAX_PLAYERS; i++) {
			for (j = 0; j < MAX_HAND; j++) {
				state.hand[i][j] = getRandNum(0, treasure_map);
			}
		}
		state.hand[activePlayer][handPos] = adventurer;
		
		// Players' Deck Counts: I originally defined the upper limit of deckCount to be
		// MAX_DECK, but due to the bug I described above for handCount, I had to restrict
		// it to MAX_HAND / 3. 
		for (i = 0; i < MAX_PLAYERS; i++) {
			state.deckCount[i] = getRandNum(0, MAX_HAND / 3);
			printf("Deck Size Player %d: %d\n", i, state.deckCount[i]);
		}

		// Deck contents for each player
		for (i = 0; i < MAX_PLAYERS; i++) {
			for (j = 0; j < MAX_DECK; j++) {
				state.deck[i][j] = getRandNum(0, treasure_map);
			}
		}

		// Players' Discard Counts: I originally defined the upper limit of deckCount to be
		// MAX_DECK, but due to the bug I described above for handCount, I had to restrict
		// it to MAX_HAND / 3. Additionally, in testing, I found that I still could not get
		// the while (drawntreasure < 2) {...} loop to terminate without potentially causing
		// undefined behavior later in the function (for instance, a negative handCount or
		// z > MAX_HAND), so I had to resort to forcing my test cases to include at least two
		// randomly selected treasure cards in the discard pile. I could, however, get the tests
		// to work if the player's deck did not have any treasure cards. The bare minimum
		// requirement to avoid a crash seems to be to have at least 2 treasure cards in the
		// active player's discard array. Therefore, the lower limit for discardCount is 2.
		for (i = 0; i < MAX_PLAYERS; i++) {
			state.discardCount[i] = getRandNum(2, MAX_HAND / 3);
			printf("Discard Count Player %d: %d\n", i, state.discardCount[i]);
		}

		// Discard contents for each player
		for (i = 0; i < MAX_PLAYERS; i++) {
			for (j = 0; j < MAX_DECK; j++) {
				state.discard[i][j] = getRandNum(0, treasure_map);
			}
		}
		
		// As per comment above, to avoid crashes, discard pile must contain at least two treasures
		i = getRandNum(1, 3);
		if (i == 1) {
			randTreasureCard = copper;
		}
		else if (i == 2) {
			randTreasureCard = silver;
		}
		else {
			randTreasureCard = gold;
		}
		i = getRandNum(0, state.discardCount[activePlayer] - 1);
		state.discard[activePlayer][i] = randTreasureCard;
		do {
			j = getRandNum(0, state.discardCount[activePlayer] - 1);
		} while (i == j);
		state.discard[activePlayer][j] = randTreasureCard;

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

		// Figure out how much treasure the player can possibly draw from the deck vs. the 
		// discard pile
		availableTreasureDeck = 0;
		availableTreasureDiscard = 0;
		for (i = 0; i < state.deckCount[activePlayer]; i++) {
			if (state.deck[activePlayer][i] >= copper && state.deck[activePlayer][i] <= gold) {
				availableTreasureDeck++;
			}
		}
		
		for (i = 0; i < state.discardCount[activePlayer]; i++) {
			if (state.discard[activePlayer][i] >= copper && state.deck[activePlayer][i] <= gold) {
				availableTreasureDiscard++;
			}
		}
		printf("Treasure available in Player %d\'s deck: %d\n", activePlayer, availableTreasureDeck);
		printf("Treasure available in Player %d\'s discard pile: %d\n", activePlayer, availableTreasureDiscard);

		/* CALL CARD EFFECT!!! */
		result = cardEffect(adventurer, choice1, choice2, choice3, &state, handPos, &bonus);
		printf("Result: %d\n", result);

		/* CHECK RESULTS */

		//Check return value -- Adventurer should always return 0.
		assertEqual(result, 0);

		//Check everything else
		testResults(&state, &oracle, activePlayer, handPos, availableTreasureDeck);

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
** that player's hand, respectively. The integer availableTreasureDeck represents the number of treasure
** cards the player's deck was randomly initialized with (nonnegative). For each test case, a call is
** made to assertEqual, which increments the numPass or numFail global variables according to whether
** or not the resulting game state matches the expected outcome of the card's effect.
*/
void testResults(struct gameState *state, struct gameState *oracle, int activePlayer, int handPos, int availableTreasureDeck) {
	int result, expected, i;
	
	// 1. Active player's hand count has been increased by 2 - 1 = 1. (+ 2 treasures, -1 played Adventure).
	// Originally, I intended to check the case for a player's deck and discard pile containing fewer than
	// 2 treasures, but as stated in above comments, these inputs contributed to program crashes. All tests
	// should cause the player to gain precisely 2 treasures (and lose the Adventure, but I know from my
	// Assignment 3 that this is not the case.)
	result = state->handCount[activePlayer];
	expected = oracle->handCount[activePlayer] + 1;
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
	
	// 3. Active player's deck count is modified appropriately. The deck count should be equal to the original
	// deck count minus the difference between the resulting discardCount and the original discardCount plus 2
	// (for the 2 treasures added to the hand).
	result = state->deckCount[activePlayer];
	expected = oracle->deckCount[activePlayer] - (state->discardCount[activePlayer] - oracle->discardCount[activePlayer] + 2);
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Deck count for Player %d was not modified correctly.\n", activePlayer);
		printf("\tExpected Deck Count: %d;\tResulting Deck Count: %d", expected, result);
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
	
	// 5. Active player's discardCount has been updated appropriately. If there were at least 2 treasures
	// in the player's deck, the discardCount should simply be the original discard count plus the difference 
	// between the original deck count and the current deck count minus the 2 treasures moved to the player's 
	// hand. Otherwise, since the discard pile was shuffled randomly into the player's deck, it's not possible
	// to tell how many cards it SHOULD have taken to locate the remaining necessary cards in the shuffed
	// discard-pile-turned-deck. I can only check to verify that no cards were "lost": current deck count + 
	// current discard count should equal the original deck count + original discard count - 2. This should
	// be true regardless of whether or not there were at least 2 treasure cards in the player's deck.
	if (availableTreasureDeck >= 2) {
		result = state->discardCount[activePlayer];
		expected = oracle->discardCount[activePlayer] + (oracle->deckCount[activePlayer] - state->deckCount[activePlayer] - 2);
		if (assertEqual(result, expected) == 0) {
			printf("Test Failed: Player %d\'s discard count not changed from %d to %d.\n",
				activePlayer, oracle->discardCount[activePlayer], expected);
			printf("\tResulting Discard Count: %d\n", result);
		}
	}
	result = state->deckCount[activePlayer] + state->discardCount[activePlayer];
	expected = oracle->deckCount[activePlayer] + oracle->discardCount[activePlayer] - 2;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Cards were lost.\n");
		printf("\tOriginal Discard Count + Deck Count: %d\n",
			oracle->deckCount[activePlayer] + oracle->discardCount[activePlayer]);
		printf("\tCurrent Discard Count + Deck Count (should be original - 2): %d\n",
			result);
	}
		
	// 6. playedCardCount has been increased by 1.
	result = state->playedCardCount;
	expected = oracle->playedCardCount + 1;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: playedCardCount not incremented from %d to %d.\n", 
			oracle->playedCardCount, expected);
	}

	
	// 7. Last card in playedCards array is Adventurer
	result = state->playedCards[state->playedCardCount - 1];
	expected = adventurer;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Last card in playedCards array (%d) is not adventurer (= 7).\n", result);
	}

	// 9. Coins should not have been changed
	result = state->coins;
	expected = oracle->coins;
	if (assertEqual(result, expected) == 0) {
		printf("Test Failed: Game coins changed from %d to %d.\n", expected, result);
	}

	// Unfortunately, I was unable to figure out a way to design a test that would uncover the
	// bug I introduced in assignment 2 that caused the first index of the discard array never
	// to be updated. Because inputs are random, it is difficult to predict what SHOULD be at
	// discard[player][0] and what shouldn't be. I found that simply testing whether it was a
	// valid card or not (between curse and treasure_map) was not sufficient to reveal the bug,
	// since the discard array was initialized with random valid cards. My best bet would be to
	// explicitly target that index in the array like I did in Assignment 3, but that doesn't
	// seem in keeping with the spirit of random testing.

}
