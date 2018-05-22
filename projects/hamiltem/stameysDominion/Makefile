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
	
unitTest1: unittest1.c dominion.o rngs.o
	gcc -o unitTest1 unittest1.c -g dominion.o rngs.o $(CFLAGS)

unitTest2: unittest2.c dominion.o rngs.o
	gcc -o unitTest2 unittest2.c -g dominion.o rngs.o $(CFLAGS)
	
unitTest3: unittest3.c dominion.o rngs.o
	gcc -o unitTest3 unittest3.c -g dominion.o rngs.o $(CFLAGS)

unitTest4: unittest4.c dominion.o rngs.o
	gcc -o unitTest4 unittest4.c -g dominion.o rngs.o $(CFLAGS)

cardTest1: cardTest1.c dominion.o rngs.o
	gcc -o cardTest1 cardTest1.c -g dominion.o rngs.o $(CFLAGS)
	
cardTest2: cardTest2.c dominion.o rngs.o
	gcc -o cardTest2 cardTest2.c -g dominion.o rngs.o $(CFLAGS)

cardTest3: cardTest3.c dominion.o rngs.o
	gcc -o cardTest3 cardTest3.c -g dominion.o rngs.o $(CFLAGS)

cardTest4: cardTest4.c dominion.o rngs.o
	gcc -o cardTest4 cardTest4.c -g dominion.o rngs.o $(CFLAGS)	

randomtestcard1: randomtestcard1.c dominion.o rngs.o
	gcc -o randomtestcard1 randomtestcard1.c -g dominion.o rngs.o $(CFLAGS)

randomtestcard1.out: randomtestcard1
	rm -f randomtestcard1.out
	rm -f dominion.gcda
	./randomtestcard1 >>randomtestcard1.out
	gcov -fb dominion.c >>randomtestcard1.out
	grep "Function[ ][']cardCouncil_room[']" -A 4 randomtestcard1.out

randomtestcard2: randomtestcard2.c dominion.o rngs.o
	gcc -o randomtestcard2 randomtestcard2.c -g dominion.o rngs.o $(CFLAGS)

randomtestcard2.out: randomtestcard2
	rm -f randomtestcard2.out
	rm -f dominion.gcda
	./randomtestcard2 >>randomtestcard2.out
	gcov -fb dominion.c >>randomtestcard2.out
	grep "Function[ ][']cardRemodel[']" -A 4 randomtestcard2.out

randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c -g dominion.o rngs.o $(CFLAGS)

randomtestadventurer.out: randomtestadventurer
	rm -f randomtestadventurer.out
	rm -f dominion.gcda
	./randomtestadventurer >>randomtestadventurer.out
	gcov -fb dominion.c >>randomtestadventurer.out
	grep "Function[ ][']cardAdventurer[']" -A 4 randomtestadventurer.out
		
testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittestresults.out: unitTest1 unitTest2 unitTest3 unitTest4 cardTest1 cardTest2 cardTest3 cardTest4
	rm -f unittestresults.out
	rm -f dominion.gcda
	./unitTest1 >>unittestresults.out
	gcov dominion.c >>unittestresults.out
	#rm dominion.gcda
	./unitTest2 >>unittestresults.out
	gcov dominion.c >>unittestresults.out
	#rm dominion.gcda
	./unitTest3 >>unittestresults.out
	gcov dominion.c >>unittestresults.out
	#rm dominion.gcda
	./unitTest4 >>unittestresults.out
	gcov dominion.c >>unittestresults.out
	#rm dominion.gcda
	./cardTest1 >>unittestresults.out
	gcov dominion.c >>unittestresults.out
	#rm dominion.gcda
	./cardTest2 >>unittestresults.out
	gcov dominion.c >>unittestresults.out
	#rm dominion.gcda
	./cardTest3 >>unittestresults.out
	gcov dominion.c >>unittestresults.out
	#rm dominion.gcda
	./cardTest4 >>unittestresults.out
	gcov -fb dominion.c	>>unittestresults.out
 
player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
