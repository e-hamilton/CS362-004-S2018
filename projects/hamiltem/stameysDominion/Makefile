CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

test_helpers.o: test_helpers.h test_helpers.c
	gcc -c test_helpers.c -g $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

default: tests.out

unittestresults.out: unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c dominion.o test_helpers.o rngs.o
	echo "Result for running Dominion tests:" > unittestresults.out
	
	echo "unittest1.c" >> unittestresults.out
	gcc -o unittest1 unittest1.c dominion.o test_helpers.o rngs.o $(CFLAGS)
	unittest1 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	
	echo "unittest2.c" >> unittestresults.out
	gcc -o unittest2 unittest2.c dominion.o test_helpers.o rngs.o $(CFLAGS)
	unittest2 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	
	echo "unittest3.c" >> unittestresults.out
	gcc -o unittest3 unittest3.c dominion.o test_helpers.o rngs.o $(CFLAGS)
	unittest3 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	
	echo "unittest4.c" >> unittestresults.out
	gcc -o unittest4 unittest4.c dominion.o test_helpers.o rngs.o $(CFLAGS)
	unittest4 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	
	echo "cardtest1.c" >> unittestresults.out
	gcc -o cardtest1 cardtest1.c dominion.o test_helpers.o rngs.o $(CFLAGS)
	cardtest1 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	
	echo "cardtest2.c" >> unittestresults.out
	gcc -o cardtest2 cardtest2.c dominion.o test_helpers.o rngs.o $(CFLAGS)
	cardtest2 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	
	echo "cardtest3.c" >> unittestresults.out
	gcc -o cardtest3 cardtest3.c dominion.o test_helpers.o rngs.o $(CFLAGS)
	cardtest3 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	
	echo "cardtest4.c" >> unittestresults.out
	gcc -o cardtest4 cardtest4.c dominion.o test_helpers.o rngs.o $(CFLAGS)
	cardtest4 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out

randomtestcard1.out: randomtestcard1.c dominion.o rngs.o
	echo "Results for running tests on randomtestcard1.c:" > randomtestcard1.out
	gcc -g -o randomtestcard1 randomtestcard1.c dominion.o rngs.o $(CFLAGS)
	randomtestcard1 >> randomtestcard1.out
	echo "Coverage Results for dominion.c" >> randomtestcard1.out
	gcov -b -f dominion.c >> randomtestcard1.out
	echo "Coverage Results for randomtestcard1.c" >> randomtestcard1.out
	gcov -b -f randomtestcard1.c >> randomtestcard1.out

randomtestcard2.out: randomtestcard2.c dominion.o rngs.o
	echo "Results for running tests on randomtestcard2.c:" > randomtestcard2.out
	gcc -g -o randomtestcard2 randomtestcard2.c dominion.o rngs.o $(CFLAGS)
	randomtestcard2 >> randomtestcard2.out
	echo "Coverage Results for dominion.c" >> randomtestcard2.out
	gcov -b -f dominion.c >> randomtestcard2.out
	echo "Coverage Results for randomtestcard2.c" >> randomtestcard2.out
	gcov -b -f randomtestcard2.c >> randomtestcard2.out

randomtestadventurer.out: randomtestadventurer.c dominion.o rngs.o
	echo "Results for running tests on randomtestadventurer.c:" > randomtestadventurer.out
	gcc -g -o randomtestadventurer randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
	randomtestadventurer >> randomtestadventurer.out
	echo "Coverage Results for dominion.c" >> randomtestadventurer.out
	gcov -b -f dominion.c >> randomtestadventurer.out
	echo "Coverage Results for randomtestadventurer.c" >> randomtestadventurer.out
	gcov -b -f randomtestadventurer.c >> randomtestadventurer.out

clean:
	rm -f *.o *.exe *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 randomtestcard1 randomtestcard2 randomtestadventurer
