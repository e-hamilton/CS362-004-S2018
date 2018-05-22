#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int compare(const void* a, const void* b);

#include "unittest.h"

int main(int argc, char** argv){ 

	printf("Testing card remodel \n");
	struct gameState* state;
	state = newGame();
	int* kc = kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 10, 16);
	int numPlayers = 2;
	int randomSeed = 4;
	
	
	
	int bonus = 0;
    int currentPlayer = whoseTurn(state);

	int i;
	int choice1 = 1;
	int choice2 = 2;

	for (int i = 0; i < 10; i++){
		initializeGame(numPlayers, kc, randomSeed, state);
		int expectedReturn = 0;
		if ( (getCost(state->hand[currentPlayer][choice1]) + 2) > getCost(state->hand[currentPlayer][choice2]))
	  	{
	  	  expectedReturn = -1;
	  	}
		assertTrue(cardEffect(remodel, choice1, choice2, 0, state, 0, &bonus) == expectedReturn, "Played an invalid card combo");
	}
	completeTest();
  
}

 