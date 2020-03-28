/*
* Author: Muhammad Khairi Norizan (Khairi)
* Purpose: HW5 Hash Table of Strings
* Language: C
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_WORD_LIMIT 32

typedef struct node{
	char wordString[MAX_WORD_LIMIT]; //value
	unsigned int count; //value
	struct node* next;
} Node;


typedef struct hashtable{
	struct node** dAray;
	unsigned int arraySize;
} hashtable;


/* Function prototypes */
void init_table(struct hashtable* T, unsigned size);
void add_to_table(struct hashtable* T, char* str);
void show_table(struct hashtable* T);


/* Hash doing the stuff */
unsigned long hash(char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; 

    return hash;
}

/* Initialize hash table */
void init_table(struct hashtable* T, unsigned size){

	/* Asignned T->array size to equal size */
	T->arraySize = size;

	//allocate every slot in hashtable  to  have  the size of Nodes
	T->dAray = malloc(sizeof(Node*) * size);

	/* set first node in each slot in the hash table to null */
	for(unsigned int i = 0; i < size; i++){
		T->dAray[i] = NULL;
	}

}
 
/* Add string to hashtable */
void add_to_table(struct hashtable* T, char* str){
	unsigned long hashRetVal;
	unsigned long modHashRetVal;

	hashRetVal = hash(str);
	modHashRetVal = hashRetVal % T->arraySize;

	//grab the first node in the hashtable at slot modHashRetVal
	Node *nodePtr = T->dAray[modHashRetVal];

	//Hashtable at slot 'hashRetVal' is empty
	if(nodePtr == NULL){

		Node *newNode = (Node *)malloc(sizeof(Node) * 1);

		//Insert value to the new node
		strcpy(newNode->wordString, str);

		newNode->count = 1;
		newNode->next = NULL;

		T->dAray[modHashRetVal] = newNode;
		
		return;
	}

	//When the slot is not empty
	Node *prevNode;

	//Search for *str in the hashtable at slot 'hashRetVal'
	while(nodePtr != NULL){
		if(strcmp(nodePtr->wordString, str) == 0){
			//printf("Found another %s in slot[%lu] \n", nodePtr->wordString, modHashRetVal);
			nodePtr->count += 1; 
			return;
		}

		/* Move to the next node */
		prevNode = nodePtr;
		nodePtr = prevNode->next;

	}

	/* Create new node */
	Node *newNode = (Node *)malloc(sizeof(Node) * 1);
	strcpy(newNode->wordString, str);
	newNode->count = 1;
	newNode->next = NULL;

	/* Point next node in slot[modHashRetVal] to point to prevNode */
	prevNode->next = newNode;

}

/* Display to standard output all nodes in a hash table  */
void show_table(struct hashtable* T){

	/* Grab the hashSize from T */
	unsigned int hashSize = T->arraySize;

	for(unsigned int i=0; i < hashSize; i++){

		/* Point *nodePtr to first node at slot[i] */
		Node *nodePtr = T->dAray[i];

		/* If Slot i is empty, then go the next slot */
		if(nodePtr == NULL){
			continue;
		}

		/* If the slot is not empty, print out all the nodes in the slot */
		while(nodePtr != NULL){
			fprintf(stdout, " %u %s \n", nodePtr->count, nodePtr->wordString);
			nodePtr = nodePtr->next;
		}

	}
	
}

int main(){
	
	bool scanHashSize = true;
	char readWord[MAX_WORD_LIMIT];
	struct hashtable *ht;
	char *word;
	unsigned int hashtableSize = 0;

	/* loop until the end of file */
	do{
		/* scan for hashsize only */
		
		if(scanHashSize == true){
			/* get the Hash Size from std in but in string */
			fgets(readWord, MAX_WORD_LIMIT, stdin);

			/* Allocate space to the word[] */
			word = malloc(sizeof(readWord));

			/* Copy readWord[] to *word */
			memcpy(word, readWord, sizeof(readWord));

			/* Convert word to int */
            hashtableSize = atoi(word);

			/* Allocate memory space to hash table */
			ht = malloc(sizeof(ht) * 1);

			/* Initialize Hash Table by calling init_table */
			init_table(ht, hashtableSize);

			/* turn scanHashSize false so wont trigger this if statement */
			scanHashSize = false;
			
		}else{
			/* scan for all the words */

			/* scan word from stdin */
			fgets(readWord, MAX_WORD_LIMIT, stdin);

			/* Replace word with next line character with null terminator */
			if(readWord[strlen(readWord)-1] == '\n' || readWord[strlen(readWord)-1] == '\r'){
				readWord[strlen(readWord)-1] = '\0';
				readWord[strlen(readWord)-1] = 0;
			}

			/* Replace word with next line character with null terminator */
			if(readWord[strlen(readWord)] == '\n' || readWord[strlen(readWord)] == '\r'){
				readWord[strlen(readWord)-1] = '\0';
				readWord[strlen(readWord)-1] = 0;
			} 

			//FOR TESTING CASE
			printf("len is: %d \n", strlen(readWord));

			printf("After Remove:%s!\n", readWord);
			//END OF TESTING CASE
			
			/* set the scan word to lowercase */
			for(int i=0; i < strlen(readWord); i++){
				readWord[i] = tolower(readWord[i]);

			}
			
			/* Allocate space with a sizeof readWord */
			word = malloc(sizeof(readWord));

			/* copy readWord[] to *word  */
			memcpy(word, readWord, sizeof(readWord));

			/* add to hashtable */
			add_to_table(ht, word);

		}
		
		
	}while(!feof(stdin));

	/* Display out everything in the hashtable */
	show_table(ht);

	return 0;
	
}
