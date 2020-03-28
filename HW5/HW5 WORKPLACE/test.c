#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_WORD_LIMIT 32

int main(){
	
	bool scanHashSize = true;
	char readWord[MAX_WORD_LIMIT];
    int scanSize = 0;
	//hashtable *ht;
	char *word;

	do{
		
		//scan for hashsize
		if(scanHashSize == true){
            fgets(readWord, MAX_WORD_LIMIT, stdin);

            word = malloc(sizeof(readWord));

			//copy readWord[] to *word
			memcpy(word, readWord, sizeof(readWord));

            scanSize = atoi(word);


			//scanf("%d", &scanSize);
            //printf("Lets see how many times you here \n", scanSize);
            printf("The size we got is %d \n", scanSize);
			//init_table(ht, hashtableSize);
			scanHashSize = false;
			
		}else{
			//get the word from stdin
			fgets(readWord, MAX_WORD_LIMIT, stdin);
			
			//set the scan word to lowercase
			for(int i=0; i < strlen(readWord); i++){
				readWord[i] = tolower(readWord[i]);
			}	
			
			word = malloc(sizeof(readWord));

			//copy readWord[] to *word
			memcpy(word, readWord, sizeof(readWord));

			//add to hashtable
			//add_to_table(ht, word);
            int countWord = 1;
            printf("Word %d is : %s \n", countWord, word);

		}
		
		
	}while(!feof(stdin));

	//Print out everything in the hashtable
	//show_table(ht);

	return 0;
	
}