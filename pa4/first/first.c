#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

void buildCache();
void run(FILE*, bool);    // FILE*: file | bool: true = prefetching, false = no prefetching
void prefetch(unsigned int, unsigned int, unsigned long);    // int: address
bool checkHit(unsigned int, unsigned int, unsigned long, bool);    // int: address | bool: true = prefetching, false = no prefetching
void writeCache(unsigned int, unsigned int, unsigned long);    // int: address
void printCounters();
void freeCache();

struct CacheBlock {
    unsigned int valid;
    unsigned long int tag;
    unsigned int time;
};

struct CacheBlock** cache;
int cacheSize, setSize, assocSize, blockSize;
bool cachePolicy;
int prefetchSize;

int memRead = 0, memWrite = 0, cacheHit = 0, cacheMiss = 0;    // Counters
unsigned int max = 0; // Counter for cache policy



int main(int argc, char** argv){
    // Checks for correct number of elements in argument
    if(argc != 7){
        return 0;
    }
    // Parses argument of format: ./first <cache size><block size><cache policy><associativity><prefetch size><trace file>
    // C = SAB
    cacheSize = atoi(argv[1]);
    blockSize = atoi(argv[2]);
    // cachePolicy: true = fifo, false = lru
    cachePolicy = (strcmp(argv[3], "fifo") == 0);
    if(!cachePolicy &&  strcmp(argv[3], "lru") != 0){    // If not fifo and not lru, then error
        printf("error\n");
        return 0;
    }
    // assocSize, setSize
    if(strcmp(argv[4], "direct") == 0){
        assocSize = 1;
        setSize = cacheSize / blockSize;
    }
    else if(strcmp(argv[4], "assoc") == 0){
        setSize = 1;
        assocSize = cacheSize / blockSize;
    }
    else{
        sscanf(argv[4], "assoc:%d", &assocSize);
        setSize = cacheSize / assocSize / blockSize;
    }
    prefetchSize = atoi(argv[5]);
    FILE *fp = fopen(argv[6], "r");

    // Checks if cacheSize, blockSize, and assocSize are all powers of 2 and if fp exists
    if(ceil(log2(cacheSize)) != floor(log2(cacheSize)) || 
       ceil(log2(blockSize)) != floor(log2(blockSize)) || 
       ceil(log2(assocSize)) != floor(log2(assocSize)) ||
       fp == NULL){
        printf("error\n");
        return 0;
    }

    // Simulates cache hits / misses for each address given in trace file
    // Runs without prefetching
    buildCache();
    run(fp, false);    // false = run without prefetching
    printf("no-prefetch\n");
    printCounters();
    fclose(fp);
    freeCache();
    
    // Reinitializes counters to 0 and reopens file
    memRead = 0, memWrite = 0, cacheHit = 0, cacheMiss = 0;
    FILE *fpPre = fopen(argv[6], "r");

    // Runs with prefetching
    buildCache();
    run(fpPre, true);    // true = run with prefetching
    printf("with-prefetch\n");
    printCounters();
    fclose(fpPre);
    freeCache();

    return 0;
}


// For each address, runs simulation and updates counters
void run(FILE* fp, bool prefetching){
    char operation;    // R or W char from file
    unsigned int address;    // Given hex address from file
    int numBlockBits = log2(blockSize), numSetBits = log2(setSize);

    while(fscanf(fp, "%c %x\n", &operation, &address) == 2 && operation != '#'){
	// Extracts set index and tag
	unsigned int index = (address >> numBlockBits) & ((1 << numSetBits) - 1);
	unsigned long tag = address >> (numBlockBits + numSetBits);

	// Updates counters
        if(checkHit(address, index, tag, false)){    // Checks if address is a hit in cache; false since non-prefetched address
            cacheHit++;
            if(operation == 'W'){
                memWrite++;
            }
        }
        else{
            cacheMiss++;
            memRead++;
            if(operation == 'W'){
                memWrite++;
                writeCache(address, index, tag);
            }
            if(prefetching){
		prefetch(address, index, tag);
            }
	}
    }
}


// Prefetches adjacent addresses
void prefetch(unsigned int address, unsigned int index, unsigned long tag){
    unsigned int prefetchAddress = address, i;
    for(i = 0; i < prefetchSize; i++){
        prefetchAddress += blockSize;
        if(!checkHit(prefetchAddress, index, tag, true)){    // true: prefetching
            writeCache(prefetchAddress, index, tag);
            memRead++;
        }
    }
}

// Checks if given address in cache and returns bool
bool checkHit(unsigned int address, unsigned int index, unsigned long tag, bool prefetching){
    // Iterates through each block in the index set to check for a match
    int i;
    for(i = 0; i < assocSize; i++){
        if(cache[index][i].valid != 0 && cache[index][i].tag == tag){
//            if(!cachePolicy && !prefetching){    // If LRU and non-prefetch address, update counter
//                cache[index][i].time = max++;
//            }
            return true;
        }
    }
    return false;
}

// Inserts address into cache if miss and write/prefetch
void writeCache(unsigned int address, unsigned int index, unsigned long tag){
    // Fill in empty block or overwrite block with lowest time variable
    int min = cache[index][0].time, minIndex = 0, i;
    
    // Searches for empty block or min value
    for(i = 0; i < assocSize; i++){
        if(cache[index][i].valid == 0){    // Empty found: set tag, valid, time and return
	    cache[index][i].tag = tag;
            cache[index][i].valid = 1;
            cache[index][i].time = max++;    // Not dependent on policy, so no FIFO/LRU distinction
            return;
	}
	if(cache[index][i].time < min){
            min = cache[index][i].time;
            minIndex = i;
        }
    }
    // Overwrites tag and replaces time with new max
    cache[index][minIndex].time = max++;
    cache[index][minIndex].tag = tag;
}

// Prints out counters in designated format
void printCounters(){
    printf("Memory reads: %d\n", memRead);
    printf("Memory writes: %d\n", memWrite);
    printf("Cache hits: %d\n", cacheHit);
    printf("Cache misses: %d\n", cacheMiss);
}

// Allocates memory for the cache as a 2D array of CacheBlocks
void buildCache(){
    cache = malloc(setSize * sizeof(struct CacheBlock*));
    int i, j;
    for(i = 0; i < setSize; i++){
        cache[i] = malloc(assocSize * sizeof(struct CacheBlock));
	for(j = 0; j < assocSize; j++){
	    cache[i][j].valid = 0;
	    cache[i][j].tag = 0;
	    cache[i][j].time = 0;
	}
    }
}

// Frees up cache as a 2D array of CacheBlocks
void freeCache(){
    int i;
    for(i = 0; i < setSize; i++){
        free(cache[i]);
    }
    free(cache);
}
