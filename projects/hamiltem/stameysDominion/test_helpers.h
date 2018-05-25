/*
**	Emily Hamilton (ONID: hamiltem, ID: 933281100)
**	Assignment 3 - CS362 Spring 2018
**	Due 4/29/18
*/

// I wrote these helper functions to help with my tests.

#ifndef _TEST_HELPERS_H
#define _TEST_HELPERS_H

#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>

#define SEED 1234

void showGameState(struct gameState* game, int numPlayers);
void getCardName(char** name, int card);
void resetSupply(struct gameState* game, int kingdomCards[10], int numPlayers);
void domAssert(int result, int expected, int* numPass, int* numFail);
void testComplete(int* numPass, int* numFail);
void printHand(int* hand, int handSize);
int arraysAreSame(int* a, int* b, int size);

#endif