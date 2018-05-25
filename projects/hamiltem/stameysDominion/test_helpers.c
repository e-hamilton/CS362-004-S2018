#include "test_helpers.h"
#include <stdio.h>
#include <stdlib.h>

//	showGameState
//	Args:	game state, number of players
//	Output:	Prints game state's values to the console.
void showGameState(struct gameState* game, int numPlayers) {
	int i, j;
	char *name;
	printf("TURN DATA ------------------------\n");
	printf("Whose turn?.............Player %d\n", game->whoseTurn);
	printf("Phase...................%d\n", game->phase);
	printf("# Actions...............%d\n", game->numActions);
	printf("# Buys..................%d\n", game->numBuys);
	printf("GAME DATA ------------------------\n");
	printf("# Players...............%d\n", game->numPlayers);
	printf("Outpost Played..........%d\n", game->outpostPlayed);
	printf("Outpost Turn............%d\n", game->outpostTurn);
	printf("Coins...................%d\n", game->coins);
	printf("Played Card Count.......%d\n", game->playedCardCount);
	//SupplyCount
	printf("Supply Count:\n");
	for (i = 0; i < treasure_map + 1; i++) {
		if (game->supplyCount[i] > -1) {
			getCardName(&name, i);
			printf("\t%s%d\n", name, game->supplyCount[i]);
		}
	}
	//EmbargoTokens
	printf("Embargo Tokens in Play (if any):\n");
	for (i = 0; i < treasure_map + 1; i++) {
		if (game->embargoTokens[i] != 0) {
			getCardName(&name, i);
			printf("\t%s%d\n", name, game->embargoTokens[i]);
		}
	}
	//HandCount
	printf("Hand Count for Each Player:\n");
	for (i = 0; i < numPlayers; i++) {
		printf("\tPlayer %d:\t%d\n", i, game->handCount[i]);
	}
	//Hand
	printf("Each Player's Hand:\n");
	for (i = 0; i < numPlayers; i++) {
		printf("\tPlayer %d:\n", i);
		for (j = 0; j < game->handCount[i]; j++) {
			getCardName(&name, game->hand[i][j]);
			printf("\t\t%s\n", name);
		}
	}
	//Deck: Not going to print out all cards in deck...
	//DeckCount
	printf("Deck Count for Each Player:\n");
	for (i = 0; i < numPlayers; i++) {
		printf("\tPlayer %d:\t%d\n", i, game->deckCount[i]);
	}
	//Discard: Not going to print out all cards in discard pile...
	//DiscardCount
	printf("Discard Count for Each Player:\n");
	for (i = 0; i < numPlayers; i++) {
		printf("\tPlayer %d:\t%d\n", i, game->discardCount[i]);
	}
	printf("\n");
}

// getCardName
// Args:	pointer to a string (name) and the number of the card to name
// Output:	Stores the name of the card followed by a colon and one or two
//			tabs-- intended for use in showGameState to display data in an
//			organized way.
void getCardName(char** name, int card) {
	switch (card) {
	case 0:
		*name = "Curse:\t\t";
		break;
	case 1:
		*name = "Estate:\t\t";
		break;
	case 2:
		*name = "Duchy:\t\t";
		break;
	case 3:
		*name = "Province:\t";
		break;
	case 4:
		*name = "Copper:\t\t";
		break;
	case 5:
		*name = "Silver:\t\t";
		break;
	case 6:
		*name = "Gold:\t\t";
		break;
	case 7:
		*name = "Adventurer:\t";
		break;
	case 8:
		*name = "Council Room:\t";
		break;
	case 9:
		*name = "Feast:\t\t";
		break;
	case 10:
		*name = "Gardens:\t";
		break;
	case 11:
		*name = "Mine:\t\t";
		break;
	case 12:
		*name = "Remodel:\t";
		break;
	case 13:
		*name = "Smithy:\t\t";
		break;
	case 14:
		*name = "Village:\t";
		break;
	case 15:
		*name = "Baron:\t\t";
		break;
	case 16:
		*name = "Great Hall:\t";
		break;
	case 17:
		*name = "Minion:\t\t";
		break;
	case 18:
		*name = "Steward:\t";
		break;
	case 19:
		*name = "Tribute:\t";
		break;
	case 20:
		*name = "Ambassador:\t";
		break;
	case 21:
		*name = "Cutpurse:\t";
		break;
	case 22:
		*name = "Embargo:\t";
		break;
	case 23:
		*name = "Outpost:\t";
		break;
	case 24:
		*name = "Salvager:\t";
		break;
	case 25:
		*name = "Sea Hag:\t";
		break;
	case 26:
		*name = "Treasure Map:\t";
		break;
	default:
		*name = "UNDEFINED~~~~~~";
	}
}

// resetSupply
// Args:	game state, the same array of kingdomCards used to initialize the game,
//			the number of players in the game
// Output:	This is essentially a copy of the code from initializeGame() in dominion.c,
//			except instead of initializing the whole game, it just resets the values
//			for the supplyCount array.
void resetSupply(struct gameState* game, int kingdomCards[10], int numPlayers) {
	//set number of Curse cards
	if (numPlayers == 2)
	{
		game->supplyCount[curse] = 10;
	}
	else if (numPlayers == 3)
	{
		game->supplyCount[curse] = 20;
	}
	else
	{
		game->supplyCount[curse] = 30;
	}

	//set number of Victory cards
	if (numPlayers == 2)
	{
		game->supplyCount[estate] = 8;
		game->supplyCount[duchy] = 8;
		game->supplyCount[province] = 8;
	}
	else
	{
		game->supplyCount[estate] = 12;
		game->supplyCount[duchy] = 12;
		game->supplyCount[province] = 12;
	}

	//set number of Treasure cards
	game->supplyCount[copper] = 60 - (7 * numPlayers);
	game->supplyCount[silver] = 40;
	game->supplyCount[gold] = 30;

	//set number of Kingdom cards
	int i, j;
	for (i = adventurer; i <= treasure_map; i++)       	//loop all cards
	{
		for (j = 0; j < 10; j++)           		//loop chosen cards
		{
			if (kingdomCards[j] == i)
			{
				//check if card is a 'Victory' Kingdom card
				if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens)
				{
					if (numPlayers == 2) {
						game->supplyCount[i] = 8;
					}
					else { game->supplyCount[i] = 12; }
				}
				else
				{
					game->supplyCount[i] = 10;
				}
				break;
			}
			else    //card is not in the set choosen for the game
			{
				game->supplyCount[i] = -1;
			}
		}
	}
}

// domAssert
// Args:	an integer result, an expected value to compare it to, a pointer
//			to an int holding the number of passes in the test, a pointer to
//			an int holding the number of failures in the test
// Output:	This function is a very rudimentary assertion that compares the int
//			in result with the int in expected. If they are the same, it prints
//			two tabs and then "PASSED" to the console; if they are not the same
//			it prints "***FAILED***" instead.
void domAssert(int result, int expected, int* numPass, int* numFail) {
	if (result == expected) {
		printf("\t\tPASSED\n");
		(*numPass)++;
	}
	else {
		printf("\t\t***FAILED***\n");
		(*numFail)++;
	}
}

// testComplete
// Args:	pointer to an int holding the number of passes in the test, a pointer
//			to an int holding the number of failures in the test
// Outcome:	testComplete will print out "TESTING COMPLETE" to the console and the
//			number of failures. If *numFail == 0, it will say that there were no
//			failures.
void testComplete(int* numPass, int* numFail) {
	printf("-------------------------------------------------------------------\n\n");
	printf("TESTING COMPLETE\n");
	if (*numFail > 1)
		printf("There were %d failures.\n\n", *numFail);
	else if (*numFail == 1)
		printf("There was %d failure.\n\n", *numFail);
	else
		printf("SUCCESS -- No failures!\n\n");
	printf("-------------------------------------------------------------------\n\n");
}

// printHand
// Args:	A pointer to an int (i.e. hand array name), hand size
// Output: 	This function is provided in interface.c as well, but I wrote my own
//			for the purposes of this assignment. printHand prints out the given
//			player's hand on a single line.
void printHand(int* hand, int handSize) {
	int i;
	for (i = 0; i < handSize - 1; i++) {
		printf("%d\t", hand[i]);
	}
	printf("%d\n", hand[i]);
}

// arraysAreSame
// Args: 	two arrays of integers and their length (in elements)
// Output:	arraysAreSame returns 1 if all elements in array a
//			equal all elements in array b.
int arraysAreSame(int* a, int* b, int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}