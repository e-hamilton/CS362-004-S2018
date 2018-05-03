/*
**	NAME:		Emily Hamilton (hamiltem)
**	DATE:		due 5/6/18
**	COURSE:		CS362-400-S2018
**	ASSIGNMENT:	Random Testing Quiz
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

// Some constants...
#define MAX_CHAR 126	// Maximum ASCII value to test = '~'
#define MIN_CHAR 32	// Minimum ASCII value to test = ' '
#define STR_LEN 5	// String length to test (does NOT include null terminator)

char inputChar()
{
	// Randomly generate an ASCII character between MIN_CHAR (32 = Space)
	// and MAX_CHAR (126 = ~)
	int range = MAX_CHAR - MIN_CHAR;
	char c = MIN_CHAR + rand() % (range + 1);
	return c;
}

char *inputString()
{
	// Allocate memory for null-terminated string with 5 chars
	char* s = malloc(sizeof(char) * (STR_LEN + 1));

	// To meet the 5 minute time constraint while still using some characters
	// outside the typical range used in the function testme(), I decided that
	// 2/3 of the time, I would randomly generate the string using characters
	// from the full range of ASCII 32 (' ') through 126 ('~'); the other
	// 1/3 of the time, I would generate the string by randomly selecting
	// characters only from the set { r, R, e, E, s, S, t, T }.

	enum setType {LONG_SET, SHORT_SET};
	
	// Determine which input set to use
	enum setType inputValueSet;
	int i;
	i = rand() % (3);
	if (i == 0)
		inputValueSet = SHORT_SET;
	else
		inputValueSet = LONG_SET;

	char shortSet[8] = { 'r', 'R', 'e', 'E', 's', 'S', 't', 'T' };

	if (inputValueSet == SHORT_SET) {
		// Randomly assign each index in s a character from the shortSet 
		// array
		for (i = 0; i < STR_LEN; i++) {
			int setIndex = rand() % 8;
			s[i] = shortSet[setIndex];
		}
	}
	else {
		// Use inputChar() to assign each index of s a random char
		for (i = 0; i < STR_LEN; i++) {
			s[i] = inputChar();
		}
	}
	
	s[STR_LEN] = '\0';

	return s;
}

void testme()
{
	// Added some time variables to print out time elapsed.
	time_t start, current, elapsed;
	start = time(NULL);
	
	int tcCount = 0;
	char *s;
	char c;
	int state = 0;
	while (1)
	{
		tcCount++;
		c = inputChar();
		s = inputString();
		printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

		if (c == '[' && state == 0) state = 1;
		if (c == '(' && state == 1) state = 2;
		if (c == '{' && state == 2) state = 3;
		if (c == ' ' && state == 3) state = 4;
		if (c == 'a' && state == 4) state = 5;
		if (c == 'x' && state == 5) state = 6;
		if (c == '}' && state == 6) state = 7;
		if (c == ')' && state == 7) state = 8;
		if (c == ']' && state == 8) state = 9;
		if (s[0] == 'r' && s[1] == 'e'
			&& s[2] == 's' && s[3] == 'e'
			&& s[4] == 't' && s[5] == '\0'
			&& state == 9)
		{
			printf("error ");

			//Print time elapsed
			current = time(NULL);
			elapsed = current - start;
			printf("\ntestme() executed in %.2f seconds / %.2f minutes.\n", (float)elapsed, (float)elapsed / 60);

			exit(200);
		}
	}
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
	testme();
	return 0;
}
