#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int compare(const void* a, const void* b);

#include "unittest.h"

int main(int argc, char** argv){ 
    srand(time(NULL));
	printf("Random testing card council_room \n");
	
	for(int i = 0; i < 100; i++){
		struct gameState* state;
		state = newGame();
		int* arr = malloc(sizeof(int)*10);
		for(int j = 0; j < 10; j++){
			int done = 0;
			while(!done){
				int num = rand()%27;
				int ok = 1;
				for (int k = 0; k < j; k++){
					if (arr[k] == num){
						ok = 0;
						break;
					}
					
				}
				if(ok){
					arr[j] = num;
					done = 1;
				}
			}
		}
		
		int* kc = arr;//kingdomCards(1, 2, 3, 4, 5, 6, 7, 8, 10, 16);
		int numPlayers = rand()%(MAX_PLAYERS - 2) + 2;
		int randomSeed = rand();
		
		initializeGame(numPlayers, kc, randomSeed, state);
		
		int bonus = 0;
	    int currentPlayer = whoseTurn(state);

		

		int handCount[MAX_PLAYERS];

		for(int i = 0; i < state->numPlayers; i++)
			handCount[i] = state->handCount[i];
		
		cardEffect(council_room, 0, 0, 0, state, rand()%state->handCount[currentPlayer], &bonus);

	    for(int i = 0; i < state->numPlayers; i++){
	    	if(i != currentPlayer){
	    		assertTrue(handCount[i] + 1 == state->handCount[i], "Every except the current player should have gained one card");
	    	
	    	}else{
	    		assertTrue(handCount[i] + 3 == state->handCount[i], "current player should have gained three cards");
	    	}
		}
	}


	completeTest();
  
}
