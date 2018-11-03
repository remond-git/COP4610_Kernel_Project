#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "wrappers.h"

int rnd(int min, int max) {
	return rand() % (max - min + 1) + min; //slight bias towards first k
}

int main(int argc, char **argv) {
	int type;
	int start;
	int dest;
	
	srand(time(0));

	if (argc != 1) {
		printf("wrong number of args\n");
		return -1;
	}
		
	type = rnd(0, 3);
	start = rnd(1, 10);
	do {
		dest = rnd(1, 10);
	} while(dest == start);
	
	long ret = issue_request(type, start, dest);
	printf("Issue (%d, %d, %d) returned %ld\n", type, start, dest, ret);

	return 0;
}
