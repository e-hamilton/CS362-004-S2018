#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int compare(const void* a, const void* b);

#include "unittest.h"

int main(int argc, char** argv){ 

	printf("Testing card smithy \n");
	struct gameState* state;
	state = newGame();
	int* kc = kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 10, 16);
	int numPlayers = 2;
	int randomSeed = 4;
	
	initializeGame(numPlayers, kc, randomSeed, state);
	
	int bonus = 0;
    int currentPlayer = whoseTurn(state);
	int hc = state->handCount[currentPlayer];
	//printf("%d\n", hc);
	cardEffect(smithy, 0, 0, 0, state, 0, &bonus);
	int hc2 = state->handCount[currentPlayer];
	//printf("%d\n", hc);
	assertTrue(hc + 2 == hc2, "I didnt gain two cards, Smithy should have given me two cards");
	
	completeTest();
  
}

 