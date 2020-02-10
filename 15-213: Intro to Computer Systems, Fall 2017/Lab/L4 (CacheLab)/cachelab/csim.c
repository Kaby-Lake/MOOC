#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "queue.h"



int help = 0;
int verbose = 0;
int setLength = 0;				// Number of set index bits
int associativity = 0;		// Associativity (number of lines per set)
int blockLength = 0;				// Number of block bits
char tracefile[50];

int main(int argc, char ** argv)
{
	int miss = 0, hits = 0, eviction = 0;
	int ch = 0;
	while ((ch = getopt(argc, argv, "hvs:E:b:t:")) != -1){
		switch (ch) {
			case 'h':
					help = 1;
					break;
			case 'v':
					verbose = 1;
					break;
			case 's':
					setLength = atoi(optarg);
					break;
			case 'E':
					associativity = atoi(optarg);
					break;
			case 'b':
					blockLength = atoi(optarg);
				break;
			case 't':
					strncpy(tracefile, optarg, 50);
				break;
			default:
					printf("Unknown argument.");
		}
	}

	if (setLength == 0 || associativity == 0 || blockLength == 0){
		printf("Miss argument.");
		return 1;
	}


	cacheLine cache[(int)pow(2, setLength)][associativity];
	for(int a = 0; a < (int)pow(2, setLength); a++){
		for(int b = 0; b < (associativity); b++){
			cache[a][b].index = b;
			cache[a][b].validBit = 0;
			cache[a][b].next = NULL;
		}
	}

	cacheQueue queue[(int)pow(2, setLength)];
	for(int a = 0; a < (int)pow(2, setLength); a++){
		queue[a].head = NULL;
		queue[a].tail = NULL;
		queue[a].size = 0;
	}

	FILE * fp = fopen(tracefile, "r");
	char identifier;
	unsigned address;
	int size; // Reading lines like " M 20,1" or "L 19,3"
	int tagLength = 64 - setLength - blockLength; // number of tag bits

	long setIndex;
	long tagIndex;

	while(fscanf(fp, " %c %x,%d", &identifier, &address, &size) > 0) {

		if (identifier == 'I')	continue;
		if (verbose == 1)	printf("%c %x,%d ", identifier, address, size);
		
		setIndex = address << tagLength >> (64 - setLength);
		tagIndex = address >> (64 - tagLength);
		start:
		if (identifier == 'L' || identifier == 'S' || identifier == 'M'){
			int hitFlag = 0;
			int evictionFlag = 0;
			for(int i = 0; i < associativity; i++) {
				if (cache[setIndex][i].tag == tagIndex && cache[setIndex][i].validBit == 1){
					
					q_remove_target((cacheQueue*)&queue[setIndex],i);
					q_insert_tail((cacheQueue*)&queue[setIndex],i);
					hitFlag = 1;
					hits++;
					break;
				}
			}
			if(!hitFlag){
				if(queue[setIndex].size == 0){ // when empty
					queue[setIndex].head = &cache[setIndex][0];
					queue[setIndex].tail = &cache[setIndex][0]; 
					queue[setIndex].size = 1;
					cache[setIndex][0].validBit = 1;
					cache[setIndex][0].tag = tagIndex;
					cache[setIndex][0].next = NULL;
					miss++;
				} else if (queue[setIndex].size == associativity) { // when need eviction
					int replaceIndex = (queue[setIndex].head) -> index;
					(queue[setIndex].head) -> validBit = 0;
					q_remove_head((cacheQueue*)&queue[setIndex]);
					q_insert_tail((cacheQueue*)&queue[setIndex],replaceIndex);
					cache[setIndex][replaceIndex].validBit = 1;
					cache[setIndex][replaceIndex].tag = tagIndex;
					miss++;
					eviction++;
					evictionFlag = 1;
				} else { // when normal
					int replaceIndex;
					for(int a = 0; a < associativity; a++) {
						if(cache[setIndex][a].validBit == 0)
							replaceIndex = a;
					}
					q_insert_tail((cacheQueue*)&queue[setIndex],replaceIndex);
					cache[setIndex][replaceIndex].validBit = 1;
					cache[setIndex][replaceIndex].tag = tagIndex;
					miss++;
				}
			}

			if (identifier == 'L' || identifier == 'S')
			{
				if (verbose == 1){
					if(hitFlag)	printf("hit ");
						else 	printf("miss ");
					if(evictionFlag == 1)
						printf("eviction");
					printf("\n");
				}
			} else { // identifier == 'M'
				if (verbose == 1){
					if(hitFlag)	printf("hit ");
						else 	printf("miss ");
					if(evictionFlag == 1)
						printf("eviction ");
				}
				identifier = 'S';
				goto start;
			}

		}

	}
		fclose(fp);
		
    printSummary(hits, miss, eviction);
    return 0;
}
