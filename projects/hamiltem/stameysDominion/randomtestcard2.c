#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>



#include "unittest.h"

int main(int argc, char** argv){ 
    srand(time(NULL));
	printf("Testing card remodel \n");
	
	for(int i = 0; i < 100; i++){
		for( int d = 0; d < 500; d ++){
			

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


			int numDraw = rand()%state->handCount[currentPlayer]+2;
			for (int n=0; n<numDraw; n++) {
				drawCard(state->whoseTurn, state);
			}
			int handPos = rand()%state->handCount[currentPlayer];
			int choice1 = rand()%state->handCount[currentPlayer];
			int choice2 = rand()%state->handCount[currentPlayer];
			int expectedReturn = 0;
			if ( (getCost(state->hand[currentPlayer][choice1]) + 2) > getCost(state->hand[currentPlayer][choice2]))
		  	{
		  	  expectedReturn = -1;
		  	}
			assertTrue(cardEffect(remodel, choice1, choice2, 0, state, handPos, &bonus) == expectedReturn, "Played an invalid card combo");
		}
	}


	completeTest();
  
}
