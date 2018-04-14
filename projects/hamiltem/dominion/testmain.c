/*
**
**		This is a weak test main method I used to make sure my refactored code
**		didn't crash. It is not part of the assignment, but I included it just
**		in case. -Emily Hamilton
**
*/

#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

int main(int argc, char** argv) {

	// GAME INITIALIZATION ///////////////////////////////////////
	struct gameState G;
	int k[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy };

	printf("Starting game.\n");

	initializeGame(2, k, atoi(argv[1]), &G);
	//////////////////////////////////////////////////////////////
	
	//Replace player's first card in hand with card to test
	G.hand[0][0] = embargo;
	playCard(0, gold, -1, -1, &G);
	printf("Embargo played by player 0.");
	
	// Original main method didn't free memory??? Not going to attempt it here.
	
	return 0;
}
