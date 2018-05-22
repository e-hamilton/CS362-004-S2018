#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int compare(const void* a, const void* b);

#include "unittest.h"

int main(int argc, char** argv){ 

	printf("Testing card council_room \n");
	struct gameState* state;
	state = newGame();
	int* kc = kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 10, 16);
	int numPlayers = 3;
	int randomSeed = 4;
	
	initializeGame(numPlayers, kc, randomSeed, state);
	
	int bonus = 0;
    int currentPlayer = whoseTurn(state);

	

	int handCount[MAX_PLAYERS];

	for(int i = 0; i < state->numPlayers; i++)
		handCount[i] = state->handCount[i];
	
	cardEffect(council_room, 0, 0, 0, state, 0, &bonus);

    for(int i = 0; i < state->numPlayers; i++){
    	if(i != currentPlayer){
    		assertTrue(handCount[i] + 1 == state->handCount[i], "Every except the current player should have gained one card");
    	
    	}else{
    		assertTrue(handCount[i] + 3 == state->handCount[i], "current player should have gained three cards");
    	}
	}

	completeTest();
  
}

 