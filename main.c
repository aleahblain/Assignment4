/* COP 3502C Programming Assignment 4
This program is written by: Aleah Blain*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

typedef struct trieNode{
	int wordFreq;
	int sumFreq;
	int maxFreq;
	struct trieNode* next[26];
}trieNode;

struct trieNode* init() {

    // Create the struct, not a word.
    trieNode* myTree = malloc(sizeof(trieNode));
    myTree->wordFreq = 0;
		myTree->sumFreq = 0;
		myTree->maxFreq = 0;

    // Set each pointer to NULLL.
    int i;
    for (i = 0 ; i < 26; i++)
        myTree->next[i] = NULL;
    // Return a pointer to the new root.
    return myTree;
}

void insert(struct trieNode* tree, char word[], int x, int times) {

		tree->sumFreq += times;

    // Down to the end, insert the word.
    if (x == strlen(word)) {
        tree->wordFreq += times;
        return;
    }

    // See if the next place to go exists, if not, create it.
    int nextIndex = word[x] - 'a';
    if (tree->next[nextIndex] == NULL){
        tree->next[nextIndex] = init();
		}	

		//
		int nextFreq = tree->next[nextIndex]->sumFreq + times;

		// Replace max if needed
		if(tree->maxFreq < nextFreq)
			tree->maxFreq = nextFreq;

		// Recursive call
    insert(tree->next[nextIndex], word, x + 1, times);
}

char* query(struct trieNode* tree, char *word, int x){

	// Variables
	int max = 0, pos = 0, nextIndex = word[x] - 'a';

	// Continue until end of string
	if(x == strlen(word)){
		if(tree->maxFreq == 0)
			return NULL;

		// Get the number of how many nodes have max freq
		for(int i = 0; i < 26; i++)	{
			if(tree->next[i] == NULL)
				continue;
			if(tree->next[i]->sumFreq == tree->maxFreq)
				max++;
		}

		char* c = malloc((max + 1)*sizeof(char));
		c[max] = '\0';

		for(int i = 0; i < 26; i++){
			if(tree->next[i] == NULL)
				continue;

			// Add to string if max
			if(tree->next[i]->sumFreq == tree->maxFreq)
				c[pos++] = (char)('a' + i);
		
		}
		return c;
	}

	if(tree->next[nextIndex] == NULL)
		return NULL;

	// Recursive call
	return query(tree->next[nextIndex], word, x + 1);
}

void freeTree(struct trieNode* tree){
	// Free children
	for(int i = 0; i < 26; i++){
		if(tree->next[i] == NULL)
			continue;
		freeTree(tree->next[i]);
	}

	free(tree);
}

int main(void) {
	atexit(report_mem_leak);
	
	int command = 0, insertions;
  struct trieNode* myTree = init();

  int i, n, j;
  FILE *fp = fopen("in.txt", "r");
	FILE *fp2 = fopen("out.txt", "w");
  fscanf(fp, "%d", &n);

  // Read file and execute appropriate protocol depending on command given
  for (i = 0; i < n; i++) {

		fscanf(fp, "%d", &command);
		char* word = malloc(sizeof(char));

		if(command == 1){
			fscanf(fp, "%s %d", word, &insertions);
			insert(myTree, word, 0, insertions);
		} else {
			fscanf(fp, "%s", word);
			char* c = query(myTree, word, 0);

			if(c != NULL){
				fprintf(fp2, "%s\n", c);
				free(c);
			} else {
				fprintf(fp2, "unknown word\n");
			}

		}
		free(word);
  }
	freeTree(myTree);
}