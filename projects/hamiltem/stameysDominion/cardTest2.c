#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int compare(const void* a, const void* b);

#include "unittest.h"

int main(int argc, char** argv){ 

	printf("Testing card adventurer \n");
	struct gameState* state;
	state = newGame();
	int* kc = kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 10, 16);
	int numPlayers = 2;
	int randomSeed = 4;
	
	initializeGame(numPlayers, kc, randomSeed, state);
	
	int bonus = 0;
    int currentPlayer = whoseTurn(state);
	int coins = 0;
	for (int i = 0; i < state->handCount[currentPlayer]; i++){
		if (state->hand[currentPlayer][i] == gold || state->hand[currentPlayer][i] == copper || state->hand[currentPlayer][i] == silver)
			coins++;
	}
	printf("coins = %d\n", coins );
	cardEffect(adventurer, 0, 0, 0, state, 0, &bonus);
	int coins2 = 0;

	for (int i = 0; i < state->handCount[currentPlayer]; i++){
		if (state->hand[currentPlayer][i] == gold || state->hand[currentPlayer][i] == copper || state->hand[currentPlayer][i] == silver)
			coins2++;
	}
	printf("coins = %d\n", coins2 );

	assertTrue(coins + 2 == coins2, "I didnt gain two treasure cards, Adventurer should have given me two treasures");
	
	completeTest();
  
}

 